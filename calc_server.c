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
#define MAX_CLIENTS 5
#define MAX_DATA 1024

void calculate(char data[], char answer[]){
	char op, temp[MAX_DATA];
	int i, j, n1, n2, ans;
	i = 0;
	j = 0;
	while(data[i] != ' '){
		temp[j++] = data[i++];
	}
	i++;
	temp[j] = '\0';
	op = data[i++];
	n1 = atoi(temp);
	j = 0;
	while(data[i] != '\0'){
		temp[j++] = data[i++];
	}
	temp[j] = '\0';
	n2 = atoi(temp);
	switch (op){
		case '+': ans = n1+n2; break;
		case '-': ans = n1-n2; break;
		case '*': ans = n1*n2; break;
		case '/': if (n2 != 0)ans = n1/n2;
				  else printf("Division by zero\n");
				  break;
		default: printf("Error: invalid operation\n");
	}
	sprintf(answer, "%d", ans);
}

int main(int argc, char **argv){
	if (argc == 1){
		printf("Error: pass port no. also as command line argument\n");
		return 0;
	}
	int sock, new, data_len, ans_len, sockaddr_len = sizeof(struct sockaddr_in);
	struct sockaddr_in server, client;
	char data[MAX_DATA], answer[MAX_DATA];

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

	printf("Waiting for connections\n");

	while(1){
		if ((new = accept(sock, (struct sockaddr *)&client, &sockaddr_len)) == -1){
			perror("accept");
			exit(-1);
		}
		printf("New client connected from port no %d and IP %s\n", ntohs(client.sin_port), inet_ntoa(client.sin_addr));
		close(sock);

		data_len = 1;

		while(data_len){
			data_len = recv(new, data, MAX_DATA, 0);

			calculate(data, answer);
			ans_len = strlen(answer);

			if (data_len){
				send(new, answer, ans_len, 0);
				answer[ans_len] = '\0';
				printf("Sent mesg: %s\n", answer);
			}
		}
		printf("Client disconnected\n");

		close(new);

		sock = socket(AF_INET, SOCK_STREAM, 0);
		bind(sock, (struct sockaddr *)&server, sockaddr_len);
		listen(sock, MAX_CLIENTS);
	}
	return 0;
}
