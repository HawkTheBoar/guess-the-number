#include "server.h"
#include "pracnet.h"
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
void run_server(int port) {
  int server_fd, client_fd;
  printf("Attempting to run server on port: %d\n", port);
  server_fd = server_create(port);
  client_fd = server_accept(server_fd);
  uint32_t num = 6969;
  num = htonl(num);
  send(client_fd, &num, sizeof(num), 0);
  close(client_fd);
  close(server_fd);
}
