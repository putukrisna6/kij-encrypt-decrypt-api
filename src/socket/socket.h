#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <exception>

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <exception>

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
        bool isListen;

        void __listen() {
            try {
                if (!isListen) {
                    throw logic_error("not allowed to listen");
                }
                memset(this->buffer, 0, sizeof(this->buffer));
                this->valread = read(this->sock, this->buffer, BUFFER_SIZE);
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

                char buffer[message.length()] = {0};
                strcpy(buffer, message.c_str());
        
                size_t currIndex = strlen(buffer);
                while (currIndex < message.length()) {
                    currIndex += 1;
        
                    size_t diff = message.length() - currIndex;
                    memcpy(&buffer[currIndex], &message[currIndex], diff);
                    currIndex += diff;
                }
                 
                send(this->sock, buffer, sizeof(buffer), 0);
            } catch (const exception& e) {
                cout << e.what() << endl;
            }
            isListen = true;
        }

    public:
        string getBuffer() {
            auto &str = this->buffer;
            string s(std::begin(str), std::end(str));
        
            size_t i = s.length();
            for (; i >= 0; i--) {
                if (s[i] != '\0') break;
            }
            s = s.substr(0, i+1);
            return s;
        }

        virtual void end() = 0;
};

#endif