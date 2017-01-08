#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

struct details {
	int from_port;
	int to_port;
	char data[1000];
};

void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

int main(int argc, char **argv){
	int sock = socket(AF_INET, SOCK_DGRAM, 0), recvlen;
	char data[1024];
	struct sockaddr_in server, client;
	int slen = sizeof(struct sockaddr_in);

	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi("10000"));
	server.sin_addr.s_addr = inet_addr("127.0.0.1");

	bzero(&client, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port = htons(atoi(argv[1]));
	client.sin_addr.s_addr = inet_addr("127.0.0.1");
	bind(sock, (struct sockaddr *)&client, sizeof(struct sockaddr_in));

	struct details a, b;
	a.from_port = atoi(argv[1]);
	a.to_port = atoi(argv[2]);
	strcpy(a.data, argv[3]);

	while(1){
		printf("While\n");
		if(time(NULL) % 2 == atoi(argv[1])%2){
			printf("Sending\n");
			sendto(sock, &a, sizeof(a), 0, (struct sockaddr *)&server, slen);
		}
		else{
			printf("Receiving\n");
			recvfrom(sock, &b, sizeof(a), 0, (struct sockaddr *)&server, &slen);
			printf("Data received : %s\n", b.data);
		}
		delay(10000);
	}
	close(sock);
	return 0;
}
