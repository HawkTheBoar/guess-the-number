#include "client.h"
#include "getopt.h"
#include "server.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#define RUN_CLIENT (1 << 0)
#define RUN_SERVER (1 << 1)
#define PORT_DEFAULT 6969
void print_usage(char *appname) {
  printf("Invalid usage.\n Example usage: %s -c -i 127.0.0.1", appname);
}
int main(int argc, char *argv[]) {
  int mode = RUN_CLIENT;
  int opt;
  int port;
  char *ip;
  while ((opt = getopt(argc, argv, "csi:p:")) != -1) {
    switch (opt) {
    case 'c':
      mode = RUN_CLIENT;
      break;
    case 's':
      mode = RUN_SERVER;
      break;
    case 'i':
      ip = optarg;
      if (!optarg || strlen(optarg) > 15) {
        printf("Error: invalid ip\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
      }
      break;
    case 'p':;
      unsigned long len = strlen(optarg);
      if (len < 3 || len > 6) {
        printf("Error: invalid port\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
      }
      port = strtol(optarg, NULL, 10);
      break;
    }
  }
  if (!port) {
    port = PORT_DEFAULT;
  }
  if (mode & RUN_CLIENT) {
    if (!ip) {
      printf("Error: IP is a required argument.\n");
      print_usage(argv[0]);
      exit(EXIT_FAILURE);
    }
    run_client(ip, port);
  } else {
    run_server(port);
  }
  exit(EXIT_SUCCESS);
}
