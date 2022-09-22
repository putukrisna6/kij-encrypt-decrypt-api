#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

class Server
{
    private:
        int server_fd, \
            new_socket, \
            valread, \
            opt, \
            addrlen;
        struct sockaddr_in address;
        char buffer[1024] = { 0 };

    public:
        Server() {
            this->opt = 1;
            this->addrlen = sizeof(address);

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

            this->valread = read(this->new_socket, this->buffer, 1024);
            printf("%s\n", this->buffer);
            send(this->new_socket, "hello server", strlen("hello server"), 0);
            printf("Hello message sent\n");
        
            // closing the connected socket
            close(this->new_socket);
            // closing the listening socket
            shutdown(this->server_fd, SHUT_RDWR);
        }
};

int main(int argc, char const* argv[]) {
    Server server;
    return 0;
}