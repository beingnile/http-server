#include <errno.h>
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

/**
 * main - Program's entry point
 *
 * Return: 0 on success
 */
int main(void)
{
	int status, socket_fd, sock_fd;
	struct addrinfo hints;
	struct addrinfo *res, *rp;
	struct sockaddr_storage client_addr;
	socklen_t client_addr_len;
	ssize_t nread, nsent;
	char buf[BUF_SIZE];
	char *msg;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;

	status = getaddrinfo(NULL, PORT, &hints, &res);
	if (status != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}

	for (rp = res; rp != NULL; rp = rp->ai_next) {
		socket_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (socket_fd == -1)
			continue;

		if (bind(socket_fd, rp->ai_addr, rp->ai_addrlen) == 0)
			break;

		close(socket_fd);
	}

	freeaddrinfo(res);

	if (rp == NULL) {
		fprintf(stderr, "[*] Could not bind to socket\n");
		exit(EXIT_FAILURE);
	}

	if (listen(socket_fd, BACKLOG) == -1) {
		fprintf(stderr, "[*] Could not listen for connection...\n");
		exit(EXIT_FAILURE);
	}

	printf("[*] Connecting to client...\n");
	while (1) {
		client_addr_len = sizeof(client_addr);
		sock_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_addr_len);
		if (sock_fd == -1) {
			fprintf(stderr, "[*] Could not connect to client\n");
			exit(EXIT_FAILURE);
		}

		printf("[*] Connected to client!!!\n");

		msg = "Hello, World!\nInput data to send> ";
		nsent = send(sock_fd, msg, strlen(msg), MSG_CONFIRM);

		if (nsent == -1) {
			fprintf(stderr, "[*] Could not send message to client\n");
			continue;
		}

		nread = recv(sock_fd, buf, BUF_SIZE, 0);

		printf("Received %zd bytes of data from client\n", nread);
		printf("Data:\n\n");
		printf("%s\n", buf);
	}

	close(sock_fd);
	close(socket_fd);

	return (0);
}
