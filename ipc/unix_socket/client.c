#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 128

int main(int argc, char *argv[]){
    struct sockaddr_un addr;
    int ret;
    char buffer[BUFFER_SIZE];
    ssize_t numRead;

    int client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    
    if (client_socket == -1){
        perror("client_socket");
    }

    printf("Client socket fd = %d\n", client_socket);

    memset(&addr, 0, sizeof(struct sockaddr_un));

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);
    
    if (connect(client_socket, (struct sockaddr *) &addr,sizeof(struct sockaddr_un)) == -1){
        perror("connect");
    }

    while ((numRead = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0) {
        if (write(client_socket, buffer, numRead) != numRead) {
            perror("write failure");
        }
    }

    if (numRead == -1) {
        perror("read");
    }

    exit(EXIT_SUCCESS);

}
