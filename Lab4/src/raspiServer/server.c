#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define PORT 8080

int main() {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int addr_len = sizeof(address);

  char buffer[BUFFER_SIZE];

  // Creates a socket with TCP, IPv4 configuration
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("Socket creation failed.");
    exit(EXIT_FAILURE);
  }

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
      exit(0); // exit child process when done
    } else {
      // we are in the parent
      close(new_socket);
      // TODO: should implement wait for the parent to wait for child, lest they
      // become zombie processes
    }
  }
  close(server_fd);

  return 0;
}