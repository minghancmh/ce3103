#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <semaphore.h>
#include <fcntl.h>           // For O_* constants
#include <sys/stat.h>        // For mode constants

#define Q_LEN 3
#define CONN 3

void error(const char *msg) {
    perror(msg);
    exit(1);
}

sem_t *thread_sem;  // Semaphore to limit the number of active threads

// Thread function to handle client communication
void *handle_client(void *sockfd_ptr) {
    int sockfd = *(int *)sockfd_ptr;
    free(sockfd_ptr);  // Free the dynamically allocated memory
    char buffer[256];
    int n, number;

    while (1) {
        // Clear the buffer and read data from the client
        memset(buffer, 0, sizeof(buffer));
        n = read(sockfd, buffer, sizeof(buffer) - 1);
        if (n <= 0) break;  // Client disconnected or error

        // If the client sends "exit", close the connection
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Client disconnected.\n");
            break;
        }

        // Convert the message to an integer and multiply by 5
        number = atoi(buffer);
        number *= 5;

        // Send the result back to the client
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "%d", number);  // Format number into string
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) error("ERROR writing to socket");
    }

    // Close the client socket
    close(sockfd);

    // Post to semaphore to signal thread completion
    sem_post(thread_sem);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int sockfd, portno, clilen;
    struct sockaddr_in serv_addr, cli_addr;

    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    // Initialize named semaphore to limit number of concurrent connections to MAX_CONN
    sem_unlink("/thread_sem");  // Unlink any existing semaphore with the same name
    thread_sem = sem_open("/thread_sem", O_CREAT | O_EXCL, 0644, CONN);  // Allow up to MAX_CONN threads
    if (thread_sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    // Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    // Clear the server address structure and set fields for serv_addr
    memset(&serv_addr, 0, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP
    serv_addr.sin_port = htons(portno);      // Convert port number to network byte order

    // Bind the socket to the address and port
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    // Listen for connections, with a queue length of Q_LEN
    listen(sockfd, Q_LEN);
    clilen = sizeof(cli_addr);

    printf("Server is listening on port %d with max queue length %d and max concurrent connections %d\n", 
           portno, Q_LEN, CONN);

    // Infinite loop to keep accepting client connections
    while (1) {
        // Accept a connection from a client
        int *newsockfd = malloc(sizeof(int));
        if (newsockfd == NULL) error("ERROR allocating memory");

        *newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
        if (*newsockfd < 0) {
            free(newsockfd);
            error("ERROR on accept");
        }

        // Wait on semaphore before creating a new thread
        sem_wait(thread_sem);  // This will block if CONN threads are already running

        // Create a thread to handle the client communication
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void *)newsockfd) != 0) {
            perror("ERROR creating thread");
            close(*newsockfd);
            free(newsockfd);
            sem_post(thread_sem);  // Release semaphore if thread creation fails
            continue;
        }

        // Detach the thread so that its resources are automatically cleaned up after termination
        pthread_detach(thread_id);
    }

    // Close the semaphore and unlink it
    sem_close(thread_sem);
    sem_unlink("/thread_sem");

    // Close the main socket
    close(sockfd);

    return 0;
}