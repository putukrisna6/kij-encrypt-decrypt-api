#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <exception>
#include "../algorithms/helpers/convertion.h"

#define PORT 8080
#define BUFFER_SIZE 1024

using namespace std;

class Socket
{
    protected:
        int sock, \
            valread, \
            fd;
        struct sockaddr_in address;
        char buffer[BUFFER_SIZE] = { 0 };
        string content;
        bool isListen;

        void __listen() {
            try {
                if (!isListen) {
                    throw logic_error("not allowed to listen");
                }

                // Get content length
                memset(this->buffer, 0, sizeof(this->buffer));
                read(this->sock, this->buffer, BUFFER_SIZE);
                string res = convertToString(this->buffer);
                size_t contentLength = stoi(convertToString(this->buffer));

                // Get content
                memset(this->buffer, 0, sizeof(this->buffer));
                this->valread = read(this->sock, this->buffer, BUFFER_SIZE);

                // Remove trash byte and set content
                string s(buffer, contentLength);
                content = s;
            } catch (const exception& e) {
                cout << e.what() << endl;
            }
            isListen = false;
        }

        void __transmit(string message) {
            try {
                if (isListen) {
                    throw logic_error("not allowed to transmit");
                }

                // Get content length
                size_t messageLength = message.length();
                if (messageLength > BUFFER_SIZE) {
                    throw invalid_argument("Transmited message size is bigger than buffer size");
                }

                // Get content
                char buffer[message.length()] = {0};
                memcpy(&buffer[0], &message[0], message.length());

                // Send content length
                send(
                    this->sock, 
                    to_string(messageLength).c_str(), 
                    BUFFER_SIZE, 
                    0
                );

                // Send content
                send(this->sock, buffer, BUFFER_SIZE, 0);
            } catch (const exception& e) {
                cout << e.what() << endl;
            }
            isListen = true;
        }

    public:
        string getBuffer() {
            return this->content;
        }

        virtual void end() = 0;
};

#endif