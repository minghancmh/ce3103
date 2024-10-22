#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <time.h>

void error(const char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Error in fork");
            exit(1);
        } else if (pid == 0) {
            // Child process
            int sockfd, portno, n;
            struct sockaddr_in serv_addr;
            struct hostent *server;
            char buffer[256];

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
            memset(&serv_addr, 0, sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;

            // Copy the server's address from the hostent structure
            memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

            // Set the server port
            serv_addr.sin_port = htons(portno);

            // Connect to the server
            if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
                error("ERROR connecting");

            // Generate a random number
            srand(time(NULL) ^ (getpid()<<16));
            int random_number = rand() % 100;  // Random number between 0 and 99

            // Send the random number to the server
            memset(buffer, 0, sizeof(buffer));
            sprintf(buffer, "%d", random_number);

            // Write the message to the socket
            n = write(sockfd, buffer, strlen(buffer));
            if (n < 0)
                error("ERROR writing to socket");

            // Read the server's response
            memset(buffer, 0, sizeof(buffer));
            n = read(sockfd, buffer, sizeof(buffer) - 1);
            if (n < 0)
                error("ERROR reading from socket");

            // Display the server's response
            printf("Child PID %d: Sent %d, Server response: %s\n", getpid(), random_number, buffer);

            // Close the socket before exiting
            close(sockfd);

            exit(0);
        }
        // Parent process continues to fork next child
    }

    // Parent process waits for all child processes to finish
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }

    return 0;
}