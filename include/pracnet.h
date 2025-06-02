#include <sys/types.h>
#ifndef PRACNET_H
int server_create(int port);
int client_connect(const char *ip, int port);
int server_accept(int server_fd);
int recv_all(int sockfd, void *buffer, int N, int flags);
#endif // !PRACNET_H
