#include "client.h"
#include "pracnet.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
void run_client(char *ip, int port) {
  int client_fd;
  client_fd = client_connect(ip, port);
  if (client_fd == -1) {
    perror("connection failed.\n");
    exit(EXIT_FAILURE);
  }
  printf("--- Guess the number i am thinking ---\n");
  printf("Your guess: ");
  u_int32_t num;
  recv_all(client_fd, &num, sizeof(num), 0);
  num = ntohl(num);
  printf("Received: %u", num);
  close(client_fd);
}
