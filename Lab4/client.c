#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);

    // Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    // Get the server by its hostname
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    // Clear the server address structure
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    // Copy the server's address from the hostent structure
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    // Set the server port
    serv_addr.sin_port = htons(portno);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    // Continuously prompt the user for input until they type "exit"
    while (1) {
        // Clear the buffer and request input from the user
        printf("Please enter a number (or type 'exit' to quit): ");
        memset(buffer, 0, 256);
        fgets(buffer, 255, stdin);

        // Check if the user wants to exit the program
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Exiting...\n");
            break;
        }

        // Write the message to the socket
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) 
            error("ERROR writing to socket");

        // Read the server's response
        memset(buffer, 0, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0) 
            error("ERROR reading from socket");

        // Display the server's response
        printf("Server response: %s\n", buffer);
    }

    // Close the socket before exiting
	close(sockfd);

    return 0;
}