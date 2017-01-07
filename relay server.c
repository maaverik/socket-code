#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char **argv){
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in server, cli1, cli2;
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = argv[1];
	server.sin_addr.s_addr = INADDR_ANY;
	bind(sock, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
	while(1){

	}
}
