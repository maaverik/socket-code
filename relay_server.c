#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char **argv){
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	char data[1024];
	struct sockaddr_in server, cli;
	int slen = sizeof(struct sockaddr);

	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = INADDR_ANY;

	bind(sock, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
	int cli1port, cli2port;
	recvfrom(sock, data, 1024, 0, (struct sockaddr *)&cli, &slen);
	cli1port = ntohs(cli.sin_port);
	recvfrom(sock, data, 1024, 0, (struct sockaddr *)&cli, &slen);
	cli2port = ntohs(cli.sin_port);

	while(1){
		printf("Waiting\n");
		int recvlen = recvfrom(sock, data, 1024, 0, (struct sockaddr *)&cli, &slen);
		printf("Received from ip: %s, port: %d\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
		data[recvlen] = '\0';
		cli.sin_port = cli.sin_port == htons(cli1port) ? htons(cli2port) : htons(cli1port);
		sendto(sock, data, recvlen, 0, (struct sockaddr *)&cli, slen);
		printf("Sent to ip: %s, port: %d\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
	}
	close(sock);
	return 0;
}
