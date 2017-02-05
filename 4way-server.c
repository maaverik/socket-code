#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

struct info {
	char from;
	char to;
	char msg[25];
};


void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}


int ports[4];

void set(char c, int port){
	if (c == 's')
		ports[0] = port;
	else if (c == 'n')
		ports[1] = port;
		
	else if (c == 'w')
		ports[2] = port;
		
	else if (c == 'e')
		ports[3] = port;
}

int get(char c){
	if (c == 's')
		return ports[0];
	else if (c == 'n')
		return ports[1];
		
	else if (c == 'w')
		return ports[2];
		
	else if (c == 'e')
		return ports[3];
}

int main(int argc, char *argv[]){
	int sock;
	struct sockaddr_in ser, cli;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	
	bzero(&ser, sizeof(ser));
	ser.sin_family = AF_INET;
	ser.sin_port = htons(atoi(argv[1]));
	ser.sin_addr.s_addr = INADDR_ANY;

	bind(sock, (struct sockaddr *)&ser, sizeof(struct sockaddr_in));
	
	int slen = sizeof(ser);
	
	struct info a, b;
	int ports[4];
	
	printf("Starting\n");
	
	recvfrom(sock, &a, sizeof(a), 0, (struct sockaddr *)&cli, &slen);
	set(a.from, ntohs(cli.sin_port));
	recvfrom(sock, &a, sizeof(a), 0, (struct sockaddr *)&cli, &slen);
	set(a.from, ntohs(cli.sin_port));
	recvfrom(sock, &a, sizeof(a), 0, (struct sockaddr *)&cli, &slen);
	set(a.from, ntohs(cli.sin_port));
	recvfrom(sock, &a, sizeof(a), 0, (struct sockaddr *)&cli, &slen);
	set(a.from, ntohs(cli.sin_port));
	
	printf("Ready\n");
	
	char dir1 = 'e', dir2 = 'w';

	while(1){
		if (dir1 == 'e') {dir1 = 's'; dir2 = 'n';}
		else if (dir1 == 's') {dir1 = 'w'; dir2 = 'e';}
		else if (dir1 == 'w') {dir1 = 'n'; dir2 = 's';}
		else if (dir1 == 'n') {dir1 = 'e'; dir2 = 'w';}
		else{
			printf("Invalid direction\n");
			return 0;
		}
		int c = get(dir1);
		cli.sin_port = htons(c);
		
		a.from = 'a';
		sendto(sock, &a, sizeof(a), 0, (struct sockaddr *)&cli, slen);

		c = get(dir2);
		cli.sin_port = htons(c);
		a.from = 'b';
		sendto(sock, &a, sizeof(a), 0, (struct sockaddr *)&cli, slen);
		
		time_t x;
		x = time(NULL);
		while(x+5 > time(NULL)){
			recvfrom(sock, &b, sizeof(a), 0, (struct sockaddr *)&cli, &slen);
			printf("received\n");
			c = get(b.to);
			cli.sin_port = htons(c);
			sendto(sock, &b, sizeof(a), 0, (struct sockaddr *)&cli, slen);
			printf("Sent\n");
			delay(100000);
		}
		
		
	}
	close(sock);
	return 0;
}


