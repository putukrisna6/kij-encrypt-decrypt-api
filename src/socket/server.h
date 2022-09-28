#ifndef SERVER_H
#define SERVER_H

#include "socket.h"

class Server : Socket
{
    private:
        int opt, addrlen;

    public:
        Server() {
            this->opt = 1;
            this->addrlen = sizeof(address);

            // Servers should listen first
            this->isListen = true;

            // Creating socket file descriptor
            if ((this->fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                perror("socket failed");
                exit(EXIT_FAILURE);
            }

            // set socket opt
            if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &this->opt, sizeof(this->opt))) {
                perror("setsockopt failure");
                exit(EXIT_FAILURE);
            }

            this->address.sin_family = AF_INET;
            this->address.sin_addr.s_addr = INADDR_ANY;
            this->address.sin_port = htons(PORT);

            // Bind to port
            if (bind(this->fd, (struct sockaddr*)&this->address, sizeof(this->address)) < 0) {
                perror("bind failed");
                exit(EXIT_FAILURE);
            }
            if (listen(this->fd, 3) < 0) {
                perror("listen failed");
                exit(EXIT_FAILURE);
            }
            if ((this->sock = accept(this->fd, (struct sockaddr*)&this->address, (socklen_t*)&this->addrlen)) < 0) {
                perror("accept failed");
                exit(EXIT_FAILURE);
            }
        }

        string getBuffer() {
            return Socket::getBuffer();
        }

        void serverListen() {
            __listen();
        }

        void serverTransmit(string message) {
            __transmit(message);
        }

        void end() {
            close(this->sock);
            shutdown(this->fd, SHUT_RDWR);
        }
};

#endif