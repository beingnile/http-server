#include "server.h"

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

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;

	status = getaddrinfo(NULL, PORT, &hints, &res);
	if (status != 0)
	{
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}

	for (rp = res; rp != NULL; rp = rp->ai_next)
	{
		socket_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (socket_fd == -1)
			continue;

		if (bind(socket_fd, rp->ai_addr, rp->ai_addrlen) == 0)
			break;

		close(socket_fd);
	}

	freeaddrinfo(res);

	if (rp == NULL)
	{
		fprintf(stderr, "[*] Could not bind to socket\n");
		exit(EXIT_FAILURE);
	}

	if (listen(socket_fd, BACKLOG) == -1)
	{
		fprintf(stderr, "[*] Could not listen for connection...\n");
		exit(EXIT_FAILURE);
	}

	printf("[*] Connecting to client...\n");
	while (1)
	{
		client_addr_len = sizeof(client_addr);
		sock_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_addr_len);
		if (sock_fd == -1)
		{
			fprintf(stderr, "[*] Could not connect to client\n");
			exit(EXIT_FAILURE);
		}

		get_peer_addr(&client_addr);
		handle_client(sock_fd);
	}

	close(sock_fd);
	close(socket_fd);

	return (0);
}

/**
 * get_peer_addr - Gets the address of the connected peer
 *
 * @client_addr: A pointer to struct sockaddr_storage with the client info
 */
void get_peer_addr(struct sockaddr_storage *client_addr)
{
	char peer_addr[INET6_ADDRSTRLEN];
	void *addr;

	if (client_addr->ss_family == AF_INET)
	{
		struct sockaddr_in *s = (struct sockaddr_in *)client_addr;
		addr = &(s->sin_addr);
	}
	else
	{
		struct sockaddr_in6 *s = (struct sockaddr_in6 *)client_addr;
		addr = &(s->sin6_addr);
	}

	inet_ntop(client_addr->ss_family, addr, peer_addr, sizeof(peer_addr));

	printf("[*] Connected to client at address: %s\n", peer_addr);
}

/**
 * handle_client - Connects to the client using TCP/UDP
 *
 * @sock_fd: The file descriptor returned by accept call
 */
void handle_client(int sock_fd)
{
	ssize_t nread, nsent;
	char buf[BUF_SIZE];
	char *msg;

	nread = recv(sock_fd, buf, BUF_SIZE, 0);

	printf("Received %lu bytes of data from client\n", nread);
	printf("Data:\n\n");
	printf("%s\n", buf);

	msg = "ACK!\n";
	nsent = send(sock_fd, msg, strlen(msg), MSG_CONFIRM);

	if (nsent == -1)
		fprintf(stderr, "[*] Could not send message to client\n");
}
