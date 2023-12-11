#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PORTNUMBER 443
#define HOSTNAME "www.google.com"

int main() {
  struct addrinfo hints, * res, * matchingIP;
  char addrstr[99];
  char port[20];
  snprintf(port, sizeof(port), "%d", PORTNUMBER); // PORTNUMBER as char*
  int sock;
  memset( & hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = 0;
  hints.ai_flags = AI_CANONNAME;

  int status = getaddrinfo(HOSTNAME, port, & hints, & matchingIP); // Memory Leak occurs here?
  if (status < 0) {
    perror("No IP address found.");
  }

  for (res = matchingIP; res != NULL; res = res -> ai_next) {
    sock = socket(res -> ai_family, res -> ai_socktype, res -> ai_protocol);
    if (sock < 0) {
      perror("No socket created.");
      continue;
    }

    if (connect(sock, res -> ai_addr, res -> ai_addrlen) < 0) {
      perror("Client not connected.");
      continue;
    }
    break;
  }

  if (res == NULL) {
    perror("Client could not connect.");
    return EXIT_FAILURE;
  }

  inet_ntop(res -> ai_family, & ((struct sockaddr_in * ) res -> ai_addr) -> sin_addr, addrstr, sizeof addrstr);
  printf("IPv4 address: %s (%s)\n", addrstr, res -> ai_canonname);
  freeaddrinfo(matchingIP);

  return 0;
}