#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 200
#define PORT 8080

int main() {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int addr_len = sizeof(address);

  char buffer[BUFFER_SIZE];

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("Socket creation failed.");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Bind failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // listen for incoming connections
  if (listen(server_fd, 3) < 0) {
    perror("Listen failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d\n", PORT);

  // Accept new incoming connetions
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                           (socklen_t *)&addr_len)) < 0) {
    perror("accepting new incoming connection failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Connection established with client!\n");
  char *server_ack_msg = "Server Acknowledged!";

  while (1) {
    int bytes_read = recv(new_socket, buffer, BUFFER_SIZE, 0);
    printf("bytes read: %d\n", bytes_read);
    if (bytes_read <= 0) {
      perror("read failed, or connection closed");
      break;
    }
    buffer[bytes_read] = '\0';
    printf("Message received from client: %s\n", buffer);

    int bytes_sent =
        send(new_socket, server_ack_msg, strlen(server_ack_msg), 0);
    if (bytes_sent > 0) {
      printf("server succesfully sent ack\n");
    }
  }

  return 0;
}