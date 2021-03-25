   /*****************************************************************************
 * client-c.c                                                                 
 * Name:
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
#define SEND_BUFFER_SIZE 2048
#define SA struct sockaddr

client(char *server_ip, char *server_port) {
	//declare variable
	int s;
	struct addrinfo* addr;
	struct addrinfo idk;
	struct sockaddr_in servaddr, cli;

	//load data struct
	memset(&idk, 0, sizeof idk);
	idk.ai_family = AF_INET;
	idk.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(NULL, server_port, &idk, &addr) != 0) {
		perror("address info error");
		return -1;
	}

	//open socket using info from getaddrinfo and send message from stdin
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == -1) {
		//handle error
		perror("Socket failed");
		exit(0);
	}

	//connect to server using socket s
	if (connect(s,addr->ai_addr, addr->ai_addrlen) != 0) {
		//handle error
		perror("Connection failed");
		exit(0);
	}
	
	char buffer[SEND_BUFFER_SIZE]; //buffer for message
	//read input
	fread(buffer, 1, SEND_BUFFER_SIZE, stdin);
	
	send(s, buffer, sizeof(buffer), 0);
	//clear buffer
	bzero(buffer, sizeof(buffer));

	return 0;
	//return 0 on success, non-zero on failure
}



/*
 * main()
 * Parse command-line arguments and call client function
*/
int main(int argc, char **argv) {
  char *server_ip;
  char *server_port;

  if (argc != 3) {
    fprintf(stderr, "Usage: ./client-c [server IP] [server port] < [message]\n");
    exit(EXIT_FAILURE);
  }

  server_ip = argv[1];
  server_port = argv[2];
  return client(server_ip, server_port);
}
