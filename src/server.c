#include "server.h"
#include "command.h"
#include "pracnet.h"
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
void run_server(int port) {
  int server_fd, client_fd;
  printf("Attempting to run server on port: %d\n", port);
  server_fd = server_create(port);
  client_fd = server_accept(server_fd);
  command_send(client_fd, "GAMESTART", "Starting game.");
  srand(time(NULL));
  int num = rand() % 100 + 1;
  printf("The number i am thinking of is %d.\n", num);
  command *c;
  long guess;
  do {
    c = command_recv(client_fd);
    guess = strtol(c->args, NULL, 10);
    if (guess > num) {
      command_send(client_fd, "GAMEINFO",
                   "Your guess is higher than the number i am thinking.\n");
    } else if (guess < num) {
      command_send(client_fd, "GAMEINFO",
                   "Your guess is less than the number i am thinking.\n");
    } else {
      command_send(client_fd, "GAMEEND",
                   "Your guess is right!\nCongratulations you won!");
    }
    command_free(c);
  } while (guess != num);
  close(client_fd);
  close(server_fd);
}
