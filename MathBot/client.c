#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netdb.h>
#define MAX 8192

int open_clientfd(char *hostname, int port, char *ipaddress) {
  // The client's socket file descriptor.
  int clientfd;

  struct hostent *hp;

  struct sockaddr_in serveraddr;
  struct in_addr address;

  printf("Echo Client is creating a socket.\n");

  if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) return -1;

  inet_aton(ipaddress, &address);

  hp = gethostbyaddr(&address, sizeof(address), AF_INET);

  bzero((char *)&serveraddr, sizeof(serveraddr));

  serveraddr.sin_family = AF_INET;

  bcopy((char *)hp->h_addr_list[0], (char *)&serveraddr.sin_addr.s_addr,
        hp->h_length);

  serveraddr.sin_port = htons(port);

  printf("Client is attempting to connect to (%s:%d).\n",
         inet_ntoa(serveraddr.sin_addr), port);

  if (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    return -1;

  printf("Echo Client connected.\n");

  return clientfd;
}

int main (int argc, char **argv) {
  int client_fd;

  char *username;

  char *IP_address;

  int p;

  char buffer[MAX];

  if (argc != 4) {
    fprintf(stderr, "use %s <username> <p> <IP_address>\n", argv[0]);
    exit(0);
  }

  username = argv[1];

  p = atoi(argv[2]);

  IP_address = argv[3];

  client_fd = open_clientfd(username, p, IP_address);

  char *message = "cs230 HELLO usahni@umass.edu\n";

  ssize_t int1 = send(client_fd, message, strlen(message), 0);

  ssize_t int2 = recv(client_fd, buffer, MAX, 0);

  if (int2 == -1) {
    printf("Not received");
  }

  if (int1 == -1) {
    printf("Not sent");
  }

  while (strstr(buffer, "BYE") == NULL) {
    printf("%ld bytes received: \n", int2);
    fputs(buffer, stdout);
    int first;
    int second;
    int result;
    char result_str[30];
    char course[20];
    char status[20];
    char operation;

    sscanf(buffer, "%s %s %d %c %d", course, status, &first, &operation, &second);

    memset(buffer, 0, sizeof(buffer));

    switch(operation) {
      case '+':
        result = first + second;
        break;
      case '-':
        result = first - second;
        break;
      case '*':
        result = first * second;
        break;
      case '/':
        result = first / second;
        break;
      case '%':
        result = first % second;
        break;
      default:
        break;
    }

    sprintf(buffer, "%s %d\n", course, result);

    int1 = send(client_fd, buffer, strlen(buffer), 0);
    int2 = recv(client_fd, buffer, MAX, 0);

    if (int2 == -1) {
      printf("Not received");
    }

    if (int1 == -1) {
      printf("Not sent");
    }

    printf("type: ");
    fflush(stdout);
  }

  printf("%s\n", buffer);

  close(client_fd);

  exit(0);
}
