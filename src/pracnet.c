#include "pracnet.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
// recieves all N bytes, returns -1 on error, 0 on peer disconnected,
// return total bytes received on success
int recv_all(int sockfd, void *buffer, int N, int flags) {
  if (N <= 0 || buffer == NULL)
    return -1;
  int received_bytes = 0, total = 0;
  int expected_bytes = N;
  char *buff = (char *)buffer;
  while (total < expected_bytes) {
    received_bytes = recv(sockfd, buff + total, expected_bytes - total, flags);
    total += received_bytes;
    if (received_bytes > 0) {
      continue;
    }
    if (received_bytes == 0) {
      perror("Peer disconnected while receiving data.\n");
      return -2;
    }
    // error receiving data
    if (received_bytes == -1) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        continue; // Non-blocking mode: retry (if flags allow)
      }
      perror("Error receiving data.\n");
      return -1;
    }
  }
  return total;
}
// Starts a server on the desired port
int server_create(int port) {
  int server_fd;
  struct sockaddr_in address;
  int opt = 1;

  // Create socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Set socket options to reuse port
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("setsockopt");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Configure server address
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY; // Listen on all network interfaces
  address.sin_port = htons(port);       // Convert port to network byte order

  // Bind the socket to the specified port
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Start listening for connections (backlog queue of 5)
  if (listen(server_fd, 5) < 0) {
    perror("listen");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d\n", port);
  return server_fd;
}
// accepts an incoming connection, and returns its sock_fd, otherwise returns -1
int server_accept(int server_fd) {
  int client_fd;
  struct sockaddr_in client_addr;
  socklen_t addr_len = sizeof(client_addr);
  char client_ip[INET_ADDRSTRLEN];

  // Accept incoming connection
  if ((client_fd =
           accept(server_fd, (struct sockaddr *)&client_addr, &addr_len)) < 0) {
    perror("accept failed");
    return -1;
  }

  // Convert client IP address to string
  inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
  printf("New connection from %s:%d\n", client_ip, ntohs(client_addr.sin_port));

  return client_fd;
}
// connects a client to the desired ip and port, returns the fd, or -1 on error
int client_connect(const char *ip, int port) {
  int client_fd;
  struct sockaddr_in server_addr;

  // Create socket file descriptor
  if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket creation failed");
    return -1;
  }

  // Configure server address structure
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);

  // Convert IPv4 address from text to binary form
  if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
    perror("invalid address/address not supported");
    close(client_fd);
    return -1;
  }

  // Connect to the server
  if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    perror("connection failed");
    close(client_fd);
    return -1;
  }

  printf("Connected to server %s:%d\n", ip, port);
  return client_fd;
}
