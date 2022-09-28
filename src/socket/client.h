#ifndef CLIENT_H
#define CLIENT_H

#include "socket.h"

class Client : Socket
{
    public:
        Client() {
            // Clients should send first
            this->isListen = false;
            if ((this->sock =socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                perror("socket creation error");
                exit(EXIT_FAILURE);
            }

            this->address.sin_family = AF_INET;
            this->address.sin_port = htons(PORT);

            if (inet_pton(AF_INET, "127.0.0.1", &this->address.sin_addr) <= 0) {
                perror("Invalid address/ Address not supported");
                exit(EXIT_FAILURE);
            }

            if ((this->fd = connect(this->sock, (struct sockaddr*)&this->address, sizeof(this->address))) < 0) {
                perror("Connection Failed");
                exit(EXIT_FAILURE);
            }
        }

        void clientListen() {
            __listen();
        }

        void clientTransmit(string message) {
            __transmit(message);
        }

        string getBuffer() {
            return Socket::getBuffer();
        }

        void end() {
            close(this->fd);
        }
};

#endif