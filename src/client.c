#include "client.h"
#include "command.h"
#include "pracnet.h"
#include "string.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
void run_client(char *ip, int port) {
  int client_fd;
  command *c;
  client_fd = client_connect(ip, port);
  if (client_fd == -1) {
    perror("connection failed.\n");
    exit(EXIT_FAILURE);
  }
  c = command_recv(client_fd);
  if (strcmp(c->command, "GAMESTART")) {
    printf("Game not started.");
    close(client_fd);
    exit(EXIT_FAILURE);
  }
  command_free(c);
  printf("--- Guess the number i am thinking ---\n");
  int guess;
  char buffer[33];
  do {
    printf("Your guess: ");
    scanf("%d", &guess);
    sprintf(buffer, "%d", guess);
    command_send(client_fd, "GUESS", buffer);
    c = command_recv(client_fd);
    printf("%s: %s\n", c->command, c->args);
    if (!strcmp(c->command, "GAMEEND")) {
      break;
    }
    command_free(c);
  } while (1);
  command_free(c);
  close(client_fd);
}
