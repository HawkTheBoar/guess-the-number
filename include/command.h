#ifndef COMMAND_H
#include <stddef.h>
typedef struct {
  char *command;
  size_t command_len;
  char *args;
  size_t args_len;
} command;

void command_send(int sockfd, char *cmd, char *args);
command *command_recv(int sockfd);
void command_free(command *c);
#endif // !COMMAND_H
