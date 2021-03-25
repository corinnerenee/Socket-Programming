/*****************************************************************************
 * server-c.c
 * Name: Corinne Bond
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

#define SA struct sockaddr
#define QUEUE_LENGTH 10
#define RECV_BUFFER_SIZE 2048

server(char *server_port) {
	int s, new_s;
	socklen_t len;
	struct addrinfo *addr;
	struct addrinfo idk;
	struct sockaddr_storage client;

	//open socket using info from getaddrinfo and send message from stdin
	memset(&idk, 0, sizeof idk);
	idk.ai_family = AF_INET;
	idk.ai_socktype = SOCK_STREAM;
	idk.ai_flags = AI_PASSIVE;

	//filling up data structs w ip, port, etc
	if (getaddrinfo(NULL, server_port, &idk, &addr) != 0) {
		perror("address info error");
	}

	//create socket
	s = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if (s == -1) {
		perror("Socket failed");
	}
	
	//bind to socket
	if (bind(s, addr->ai_addr, addr->ai_addrlen) == -1){
		perror("socket bind failed\n");
		close(s);
		exit(0);
	}

	//prepare server for incoming requests and set s to listening socket
	if (listen(s, 10) != 0) {
			printf("an error: %s\n", strerror(errno));//prints an error: + message frm errno
			exit(0);
	}
	len = sizeof(client);

	//infinite loop for clients to connect
	while (1) {
		new_s = accept(s, (SA*)&client, &len);
		if (new_s  < 0) {
			printf("server accept failed\n");
			exit(0);
		}	
	char buf [RECV_BUFFER_SIZE];
	//receive message from server
	recv(new_s, buf, sizeof(buf), 0);

	//print msg to output
	printf("%s", buf);
	fflush(stdout);

	//clear buffer
	bzero(buf, RECV_BUFFER_SIZE);

	close(new_s);
	}

	
	return 0;
	//send message to client using the new_s descrip
		//* Print received message to stdout
		//* Return 0 on success, non - zero on failure, close new_s 
	
}



/*
 * main():
 * Parse command-line arguments and call server function
*/
int main(int argc, char **argv) {
	char *server_port;

	if (argc != 2) {
		fprintf(stderr, "Usage: ./server-c [server port]\n");
		exit(EXIT_FAILURE);
	}

	server_port = argv[1];
	return server(server_port);
}