#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 200
#define PORT 8080
#define SERVER_IP "172.17.0.3"

int main() {

  int sock = 0;
  struct sockaddr_in server_address;
  char buf[BUFSIZE];
  char message[BUFSIZE];

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Socket creation failed");
    return -1;
  }

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);

  if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0) {
    perror("Invalid address / address not supported");
    return -1;
  }

  // Connecting to the server
  if (connect(sock, (struct sockaddr *)&server_address,
              sizeof(server_address)) < 0) {
    perror("connection failed");
    return -1;
  }

  printf("Connected to server. Type a message and press Enter: \n");

  // int i;

  while (1) {
    fgets(message, BUFSIZE, stdin);
    message[strcspn(message, "\n")] = '\0';

    int bytes_sent = send(sock, message, strlen(message), 0);

    if (bytes_sent < 0) {
      perror("Send failed");
    }

    printf("bytes_sent: %d\n", bytes_sent);

    if (strcmp(message, "exit") == 0) {
      printf("Exiting the client...\n");
      break;
    }

    printf("Message sent by client: %s\n", message);

    recv(sock, buf, BUFSIZE, 0);

    printf("%s\n", buf);
  }

  return 0;
}