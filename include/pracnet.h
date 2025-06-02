#ifndef PRACNET_H
int server_create(int port);
int client_connect(const char *ip, int port);
int server_accept(int server_fd);
#endif // !PRACNET_H
