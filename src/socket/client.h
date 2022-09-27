#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <exception>

#define PORT 8080
#define BUFFER_SIZE 1024

class Client
{
    private:
        int sock, \
            valread, \
            client_fd;
        struct sockaddr_in serv_addr;
        char buffer[1024] = { 0 };
        bool isListen;

    public:
        Client() {
            // Clients should send first
            this->isListen = false;
            if ((this->sock =socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                perror("socket creation error");
                exit(EXIT_FAILURE);
            }

            this->serv_addr.sin_family = AF_INET;
            this->serv_addr.sin_port = htons(PORT);

            if (inet_pton(AF_INET, "127.0.0.1", &this->serv_addr.sin_addr) <= 0) {
                perror("Invalid address/ Address not supported");
                exit(EXIT_FAILURE);
            }

            if ((this->client_fd = connect(this->sock, (struct sockaddr*)&this->serv_addr, sizeof(this->serv_addr))) < 0) {
                perror("Connection Failed");
                exit(EXIT_FAILURE);
            }
        }

        ~Client() {
            clientEnd();
        }

        std::string getBuffer() {
            auto& str = this->buffer;
            string s(std::begin(str), std::end(str));
        
            size_t i = s.length();
            for (; i >= 0; i--) {
                if (s[i] != '\0') break;
            }
            s = s.substr(0, i+1);
            return s;
        }

        void clientListen() {
            try {
                if (!isListen) {
                    throw std::logic_error("not allowed to listen");
                }
                memset(this->buffer, 0, sizeof(this->buffer));
                this->valread = read(this->sock, this->buffer, BUFFER_SIZE);
            } catch (const std::exception& e) {
                std::cout << e.what() << std::endl;
            }
            isListen = false;
        }

        void clientTransmit(std::string message) {
            try {
                if (isListen) {
                    throw std::logic_error("not allowed to transmit");
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
            } catch (const std::exception& e) {
                std::cout << e.what() << std::endl;
            }
            isListen = true;
        }

        void clientEnd() {
            close(this->client_fd);
        }
};