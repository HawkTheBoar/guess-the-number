#include "command.h"
#include "pracnet.h"
#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
// TODO: Handle send errors
void command_send(int sockfd, char *cmd, char *args) {
  command c;
  c.args = args;
  c.command = cmd;
  c.args_len = htonl(strlen(args) + 1);
  c.command_len = htonl(strlen(cmd) + 1);

  send(sockfd, &c.command_len, sizeof(uint32_t), 0);
  send(sockfd, c.command, strlen(cmd) + 1, 0);
  send(sockfd, &c.args_len, sizeof(uint32_t), 0);
  send(sockfd, c.args, strlen(args) + 1, 0);
}
// TODO: handle errors from recv
command *command_recv(int sockfd) {
  command *c = malloc(sizeof(command));
  // receive the length
  recv_all(sockfd, &c->command_len, sizeof(uint32_t), 0);
  // reorder the bytes to host order
  c->command_len = ntohl(c->command_len);
  // allocate the buffer
  c->command = malloc(c->command_len);
  // recv the buffer
  recv_all(sockfd, c->command, c->command_len, 0);
  c->command[c->command_len - 1] = '\0';
  // repeat
  recv_all(sockfd, &c->args_len, sizeof(uint32_t), 0);
  c->args_len = ntohl(c->args_len);
  c->args = malloc(c->args_len);
  recv_all(sockfd, c->args, c->args_len, 0);
  c->args[c->args_len - 1] = '\0';
  return c;
}
void command_free(command *c) {
  free(c->command);
  free(c->args);
  free(c);
}
