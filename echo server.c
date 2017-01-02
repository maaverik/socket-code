// Echo server

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
#define MAX_CLIENTS 2
#define MAX_DATA 1024

int main(int argc, char **argv){
	if (argc == 1){
		printf("Error: pass port no. also as command line argument\n");
		return 0;
	}
	int sock, new, data_len, sockaddr_len = sizeof(struct sockaddr_in);
	struct sockaddr_in server, client;
	char data[MAX_DATA];

	if ((sock = socket(AF_INET, SOCK_STREAM, 0))  == ERROR){
		perror("server socket: ");
		exit(-1);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(&server.sin_zero, 8);

	if ((bind(sock, (struct sockaddr *)&server, sockaddr_len)) == ERROR){
		perror("Bind");
		exit(-1);
	}

	if ((listen(sock, MAX_CLIENTS)) == ERROR){
		perror("listen");
		exit(-1);
	}

	while(1){
		if ((new = accept(sock, (struct sockaddr *)&client, &sockaddr_len)) == -1){
			perror("accept");
			exit(-1);
		}
		printf("New client connected from port no %d and IP %s\n", ntohs(client.sin_port), inet_ntoa(client.sin_addr));
		data_len = 1;

		while(data_len){
			data_len = recv(new, data, MAX_DATA, 0);
			if (data_len){
				send(new, data, data_len, 0);
				data[data_len] = '\0';
				printf("Sent mesg: %s\n", data);
			}
		}
		printf("Client disconnected\n");

		close(new);
	}
	return 0;
}
