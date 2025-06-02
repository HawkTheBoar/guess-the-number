#include "server.h"
#include "pracnet.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
void run_server(int port) {
  int server_fd, client_fd;
  printf("Attempting to run server on port: %d\n", port);
  server_fd = server_create(port);
  client_fd = server_accept(server_fd);
  char *msg = "Hello from server!\n";
  send(client_fd, msg, strlen(msg), 0);
  close(client_fd);
}
