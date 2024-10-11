#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define PORT 8080

int num_connections = 0;

void handle_sigchild(int sig) {
  (void)sig;
  int status;
  pid_t pid;

  while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    num_connections--;
    pid_t pp = getpid();
    printf("[%d]: Num Connections = %d\n", pp, num_connections);
  }
}

int main() {
  int server_fd, new_socket;
  sem_t *sema;
  struct sockaddr_in address;
  int addr_len = sizeof(address);

  char buffer[BUFFER_SIZE];

  // Initialize the semaphore
  sema = sem_open("concurrent_conn_sema", O_CREAT, 0644, 3);

  if (sema == SEM_FAILED) {
    perror("Counting semaphore initialization failed");
    exit(EXIT_FAILURE);
  }

  // Creates a socket with TCP, IPv4 configuration
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("Socket creation failed.");
    exit(EXIT_FAILURE);
  }

  signal(SIGCHLD, handle_sigchild);

  address.sin_family = AF_INET;

  // bind to all available network interfaces on the machine
  address.sin_addr.s_addr = INADDR_ANY;

  // host endian to network endian conversion
  address.sin_port = htons(PORT);

  // Binds the address specified in &address to the socket created previously
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Bind failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // listen for incoming connections
  // max len of queue for incoming connections (3 connections max, anything
  // beyond that, we throw ERRCONNREFUSED)
  if (listen(server_fd, 3) < 0) {
    perror("Listen failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  while (1) {
    pid_t pp = getpid();

    printf("[%d]: Server listening on port %d\n", pp, PORT);

    // Accept new incoming connetions
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addr_len)) < 0) {
      perror("accepting new incoming connection failed");
      continue;
    }

    // Counting semaphone implementation so that server sleeps when it has 3
    // incoming connections Will only wake up when one of those incoming
    // connections terminates
    // Down the semaphore when we want to fork
    printf("[%d]: trying to attain the semaphore\n", pp);
    sem_wait(sema);
    printf("[%d]: semaphore attained\n", pp);

    num_connections++;
    printf("[%d]: Num Connections = %d\n", pp, num_connections);

    // Fork so that we can handle multiple connections
    pid_t pid = fork();
    if (pid < 0) {
      perror("Forking failed");
    } else if (pid == 0) {
      // we are in the child
      close(server_fd);
      pid_t p = getpid();

      printf("[%d]: Connection established with client!\n", p);
      char *server_ack_msg = "Server Acknowledged!";

      while (1) {
        int bytes_read = recv(new_socket, buffer, BUFFER_SIZE, 0);

        printf("[%d]: bytes read: %d\n", p, bytes_read);
        if (bytes_read <= 0) {
          perror("read failed, or connection closed");
          break;
        }
        buffer[bytes_read] = '\0';
        printf("[%d]: Message received from client: %s\n", p, buffer);

        int bytes_sent =
            send(new_socket, server_ack_msg, strlen(server_ack_msg), 0);
        if (bytes_sent > 0) {
          printf("[%d]: server succesfully sent ack\n", p);
        }
      }
      close(new_socket);

      // Up the semaphore when child process exits
      sem_post(sema);
      exit(0); // exit child process when done
    } else {
      // we are in the parent
      close(new_socket);
      // TODO: should implement wait for the parent to wait for child, lest they
      // become zombie processes
    }
  }
  close(server_fd);
  sem_close(sema);

  return 0;
}