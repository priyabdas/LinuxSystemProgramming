/* Socket Server
 * 
 * Server has the below flow 
 * socket ()        << Creates the file descriptor for client
 * bind ()          << Bind the addr to the socket fd 
 * listen ()        << listen for client
 * accept ()        << accept client
 *
 *
 *  Created by Priyabrata Das on 07/06/21
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 128

#define BACKLOG 10

int main (int argc, char *argv[]){
    struct sockaddr_un addr;
   

    //unlink if the same socket was there
    unlink(SOCKET_NAME);


    //create a unix socket fd with AF_UNIX and SOCK_STREAM
    //connection socket 
    int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    
    printf("Server socket fd = %d\n", socket_fd);
    
    //sanity check for connection socket
    if (socket_fd == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("Server connection socket created\n");


    //set the addr 
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    //Bind the addr to the connection socket .
    //This is a well known address where client can connect to 

    if (bind(socket_fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) ==-1){
        perror("Bind failure");
        exit(EXIT_FAILURE);
    }

    printf("server bind() successful \n");


    //prepare to listen from client 
    
    if (listen(socket_fd, BACKLOG) == -1){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    //we need to serve client data
    //client data will be present in another socket
    
    ssize_t numRead;


    while(1){
    
        printf("Waiting for client connection\n");
        int data_fd = accept(socket_fd, NULL, NULL);
        printf("accepted client %d\n",data_fd); 
        
        char buffer[BUFFER_SIZE];
        memset(buffer, 0 , BUFFER_SIZE);

        while((numRead = read(data_fd, buffer, BUFFER_SIZE)) > 0){
            if (write(STDOUT_FILENO, buffer, numRead) != numRead) {
                perror("partial/failed write");
            }
        }

        if (numRead == -1) {
            perror("numRead");
        }

        if (close(data_fd) == -1) {
            perror("close");
        }    
    
    }

}
