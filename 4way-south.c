#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

struct info {
	char from;
	char to;
	char msg[25];
};

int main(int argc, char *argv[]){
	int sock;
	struct sockaddr_in cli, ser;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	
	bzero(&cli, sizeof(cli));
	cli.sin_family = AF_INET;
	cli.sin_port = htons(atoi(argv[1]));
	cli.sin_addr.s_addr = inet_addr("127.0.0.1");
	bind(sock, (struct sockaddr *)&cli, sizeof(ser));
	
	bzero(&ser, sizeof(ser));
	ser.sin_family = AF_INET;
	ser.sin_port = htons(atoi(argv[2]));
	ser.sin_addr.s_addr = inet_addr("127.0.0.1");

	int slen = sizeof(cli);
	
	
	struct info a, b;
	a.from = 's';
	a.to='n';
	strcpy(a.msg, "From south to north\n");
	
	sendto(sock, &a, sizeof(a), 0, (struct sockaddr *)&ser, slen);
	
	
	while(1){
			recvfrom(sock, &b, sizeof(a), 0, (struct sockaddr *)&ser, &slen);
			
			if (b.from == 'a'){
				printf("Sending\n");					
				time_t x;
				x = time(NULL);
				while(x+5 > time(NULL)){
					sendto(sock, &a, sizeof(a), 0, (struct sockaddr *)&ser, slen);	
					delay(100000);		
				}
			}
			else if (b.from == 'b'){
				time_t x;				
				printf("receiving\n");
				x = time(NULL);
				while(x+5 > time(NULL)){
					recvfrom(sock, &b, sizeof(a), 0, (struct sockaddr *)&ser, &slen);
					printf("Received : %s\n", b.msg);
					delay(100000);
				}
			}
	}
	
	
	close(sock);
	return 0;
}

			

