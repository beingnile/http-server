#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define PORT "9997"
#define BUF_SIZE 1024
#define BACKLOG 10

void handle_client(int sock_fd);
void get_peer_addr(struct sockaddr_storage *client_addr);

#endif /* SERVER_H */
