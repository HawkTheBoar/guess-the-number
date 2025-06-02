#include "client.h"
#include "pracnet.h"
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
void run_client(char *ip, int port) {
  int client_fd;
  client_fd = client_connect(ip, port);
  char buffer[1024];
  ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
  if (bytes_received > 0) {
    buffer[bytes_received] = '\0';
    printf("Recieved bytes: %s", buffer);
  } else if (bytes_received == 0) {
    printf("Connection closed by peer.\n");
  } else {
    perror("recv failed");
  }
  close(client_fd);
}
