#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define BUFSIZE 1024
#define PORT 8080
#define SERVER_IP "172.17.0.3"

// Vary NUM_PROC to determine how many child processes to fork
// Note that if NUM_PROC is too high, the server
// may ignore requests in `listen(..., MAX_QUEUE_LEN)` [see line 71 for listen call]
// so, in order to not overload the server, you might want to
// increase MAX_QUEUE_LEN in the server.

// NUM_PROC = 5, together with MAX_QUEUE_LEN = 3 shows how the counting semaphore
// which is initialized with MAX_CONN = 3 limits requests to the server
#define NUM_PROC 5

int main() {

  struct sockaddr_in server_address;

  // Receiver buffer
  char buf[BUFSIZE];

  // Sender buffer
  char message[BUFSIZE];

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);

  // Converting string representation of IP to network format, stores it in
  // server_address.sin_addr
  if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0) {
    perror("Invalid address / address not supported");
    return -1;
  }

  pid_t child_pids[NUM_PROC];
  memset(child_pids, 0, NUM_PROC);

  for (int k = 0; k < NUM_PROC; k++) {
    int sock = 0;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("Socket creation failed");
      return -1;
    }
    pid_t pid = fork();
    if (pid < 0) {
      perror("Forking failed");
    } else if (pid == 0) {
      pid_t cp = getpid();
      pid_t pp = getppid();
      printf("In child process [%d], parent is [%d]\n", cp, pp);

      // we are in the child process
      // Connecting to the server on the socket
      // created above
      int status = connect(sock, (struct sockaddr *)&server_address,
                           sizeof(server_address));

      printf("[%d]: Connection to server status: %d\n", cp, status);
      if (status < 0) {
        perror("connection failed");
        return -1;
      }

      printf("[%d]: Connected to server.\n", cp);

      srand(time(NULL) ^ cp);

      long rand = random();
      printf("[%d]: sending %ld\n", cp, rand);
      sprintf(message, "%ld", rand);

      send(sock, message, sizeof(message), 0);

      // This makes it so that the for loop ends here. if not, child process
      // will continue forking
      exit(0);

    } else {
      // we are in the parent process
      child_pids[k] = pid;
    }
  }

  // cleanup, so we dont have zombies

  for (int k = 0; k < NUM_PROC; k++) {
    int status;
    if (child_pids[k] == 0) {
      break;
    }
    waitpid(child_pids[k], &status, 0);
  }

  return 0;
}