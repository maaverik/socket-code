// Echo client

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define ERROR -1
#define BUFFER 1024

int main(int argc, char **argv){
	if (argc < 3){
		printf("Error: pass ip address and port no. also as command line argument\n");
		return 0;
	}
	int sock, new, len;
	struct sockaddr_in remote_server;
	char input[BUFFER], output[BUFFER];

	if ((sock = socket(AF_INET, SOCK_STREAM, 0))  == ERROR){
		perror("socket");
		exit(-1);
	}
	remote_server.sin_family = AF_INET;
	remote_server.sin_port = htons(atoi(argv[2]));
	remote_server.sin_addr.s_addr = inet_addr(argv[1]);
	bzero(&remote_server.sin_zero, 8);

	if ((connect(sock, (struct sockaddr *)&remote_server, sizeof(struct sockaddr_in))) == ERROR){
		perror("Connect");
		exit(-1);
	}

	while(1){
		fgets(input, BUFFER, stdin);
		send(sock, input, strlen(input), 0);
		len = recv(sock, output, BUFFER, 0);
		output[len] = '\0';
		printf("%s\n", output);
	}
	close(sock);
	return 0;
}
