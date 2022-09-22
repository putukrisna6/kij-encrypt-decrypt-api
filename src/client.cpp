#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 8080

class Client
{
    private:
        int sock, \
            valread, \
            client_fd;
        struct sockaddr_in serv_addr;
        char buffer[1024] = { 0 };

    public:
        Client() {
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

            send(this->sock, "hello client", strlen("hello client"), 0);
            printf("Hello message sent\n");
            this->valread = read(this->sock, this->buffer, 1024);
            printf("%s\n", this->buffer);
 
            // closing the connected socket
            close(this->client_fd);
        }
};

int main(int argc, char const* argv[])
{
    Client client;
    return 0;
}