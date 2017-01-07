#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char **argv){
	int sock = socket(AF_INET, SOCK_DGRAM, 0), recvlen;
	char data[1024];
	struct sockaddr_in server, client;
	int slen = sizeof(struct sockaddr_in);

	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr = inet_addr(argv[1]);

	strcpy(data, "1");
	sendto(sock, data, strlen(data), 0, (struct sockaddr *)&server, slen);

	while(1){

		printf("Waiting to receive\n");
		recvlen = recvfrom(sock, data, 1024, 0, (struct sockaddr *)&server, &slen);
		data[recvlen] = '\0';
		printf("Received data: %s\n", data);

		printf("\nEnter data\n");
		fgets(data, 1024, stdin);
		sendto(sock, data, strlen(data), 0, (struct sockaddr *)&server, slen);
		printf("Data sent\n");

	}
	close(sock);
	return 0;
}
