/*
 *
 * server.c
 * ascgif 2014
 *
 */

#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include <unistd.h>

#include "server.h"

int sockfd; // The server's socket pointer
void cleanExit() { close(sockfd); exit(0); }

#define BUFFSIZE 4096
#define DEFAULT_PORT 5050

int main(int argc, char *argv[]) {
    signal(SIGTERM, cleanExit);
    signal(SIGINT, cleanExit);

    int newsockfd;       // The socket to get with a connection
    int port;            // The port to listen on (default 5050)
    unsigned int clilen; // The length of the client's address
    int yes = 1;

    struct sockaddr_in serv_addr; // The server's address
    struct sockaddr_in cli_addr;  // The client's address

    char buffer[BUFFSIZE]; // A buffer to read results to.
    int n;                 // Number of bytes read

    char* output; // Stores the output to be returned in the browser

    clilen = sizeof(cli_addr);

    // Allocate sockfd and set the port number
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket could not be opened.");
        return EXIT_FAILURE;
    }
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&yes, sizeof(int) );
    port = (argc > 1) ? atoi(argv[1]) : DEFAULT_PORT;

    // Zero out and build the socket structure
    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons((u_short)port);

    // Bind the host address and listen for a client
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Host address could not be properly bound.");
        return EXIT_FAILURE;
    }

    listen(sockfd, 10);

    while(1) {
        if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) < 0) {
            perror("Could not accept connection from client.");
            return EXIT_FAILURE;
        }

        if ((n = read(newsockfd, buffer, BUFFSIZE)) > 0) {
            output = processFile(buffer);
            printf("%s\n", output);
            write(newsockfd, output, strlen(output));
            bzero(buffer, BUFFSIZE);
            free(output);
        }

        close(newsockfd);
    }

    close(sockfd);
    exit(0);
}
