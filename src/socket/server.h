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

class Server
{
    private:
        int server_fd, \
            new_socket, \
            valread, \
            opt, \
            addrlen;
        struct sockaddr_in address;
        char buffer[BUFFER_SIZE] = { 0 };
        bool isListen;

    public:
        Server() {
            this->opt = 1;
            this->addrlen = sizeof(address);
            // Servers should listen first
            this->isListen = true;

            // Creating socket file descriptor
            if ((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                perror("socket failed");
                exit(EXIT_FAILURE);
            }

            // set socket opt
            if (setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &this->opt, sizeof(this->opt))) {
                perror("setsockopt failure");
                exit(EXIT_FAILURE);
            }

            this->address.sin_family = AF_INET;
            this->address.sin_addr.s_addr = INADDR_ANY;
            this->address.sin_port = htons(PORT);

            // Bind to port
            if (bind(this->server_fd, (struct sockaddr*)&this->address, sizeof(this->address)) < 0) {
                perror("bind failed");
                exit(EXIT_FAILURE);
            }
            if (listen(this->server_fd, 3) < 0) {
                perror("listen failed");
                exit(EXIT_FAILURE);
            }
            if ((this->new_socket = accept(this->server_fd, (struct sockaddr*)&this->address, (socklen_t*)&this->addrlen)) < 0) {
                perror("accept failed");
                exit(EXIT_FAILURE);
            }
        }

        ~Server() {
            serverEnd();
        }

        char* getBuffer() {
            return this->buffer;
        }

        void serverListen() {
            try {
                if (!isListen) {
                    throw std::logic_error("not allowed to listen");
                }

                this->valread = read(this->new_socket, this->buffer, BUFFER_SIZE);
            } catch (const std::exception& e) {
                std::cout << e.what() << std::endl;
            }
            isListen = false;
        }

        void serverTransmit(std::string message) {
            try {
                if (isListen) {
                    throw std::logic_error("not allowed to transmit");
                }

                send(this->new_socket, message.c_str(), strlen(message.c_str()), 0);
            } catch (const std::exception& e) {
                std::cout << e.what() << std::endl;
            }
            isListen = true;
        }

        void serverEnd() {
            close(this->new_socket);
            shutdown(this->server_fd, SHUT_RDWR);
        }
};