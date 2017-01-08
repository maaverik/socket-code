#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

struct details {
	int from_port;
	int to_port;
	char data[1000];
};

int main(int argc, char **argv){
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	struct details a;
	struct sockaddr_in server, cli;
	int slen = sizeof(struct sockaddr);

	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi("10000"));
	server.sin_addr.s_addr = INADDR_ANY;

	bind(sock, (struct sockaddr *)&server, sizeof(struct sockaddr_in));

	while(1){
		printf("Waiting\n");
		int recvlen = recvfrom(sock, &a, sizeof(a), 0, (struct sockaddr *)&cli, &slen);
		printf("Received from ip: %s, port: %d\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
		printf("From port %d, to port %d\n", a.from_port, a.to_port);
		//data[recvlen] = '\0';
		cli.sin_port = htons(a.to_port);
		sendto(sock, &a, sizeof(a), 0, (struct sockaddr *)&cli, slen);
		printf("Sent to ip: %s, port: %d\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
	}
	close(sock);
	return 0;
}
