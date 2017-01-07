#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char **argv){
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in server;
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr = inet_addr(argv[1]);
	bind(sock, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
	while(1){

	}
}
