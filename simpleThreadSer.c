#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int unused_port = 2000;
int curport;

void *run_server(void *unused_port){
	printf("server running\n");
	
	struct sockaddr_in server,client;
    char buffer[50];
    int s,n;
    s=socket(AF_INET,SOCK_DGRAM,0);
    server.sin_family=AF_INET;
    server.sin_port=*((int *) unused_port);
    
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    bind(s,(struct sockaddr *)&server,sizeof(server));
    n=sizeof(client);

	recvfrom(s,buffer,sizeof(buffer), 0,(struct sockaddr *)&client,&n);	
	printf("msg from client : %s\n", buffer);
	
	strcpy(buffer, "you're welcome");
	sendto(s,buffer,sizeof(buffer),0,(struct sockaddr *)&client,n);
	
	
}

int start_new_server(){
	int *port = (int*) malloc(sizeof(int));
	*port = unused_port;
	curport = unused_port;
	
	pthread_t thread;
	
	pthread_create(&thread, NULL, run_server, (void*) port);
	
    
        
	return unused_port++;
}

int main(){
	struct sockaddr_in server,client;
    char b1[100],b2[100],a[100];
    int s,n;
    s=socket(AF_INET,SOCK_DGRAM,0);
    server.sin_family=AF_INET;
    server.sin_port=3000;
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    bind(s,(struct sockaddr *)&server,sizeof(server));
    n=sizeof(client);
	
	while(1)
    {
		recvfrom(s,&b2,sizeof(b2), 0,(struct sockaddr *)&client,&n);
		
		printf("%s\n", b2);
		
		int port = start_new_server();
		
		printf("server starting on %d\n", port);
		
		sendto(s,&port,sizeof(port),0,(struct sockaddr *)&client,n);
		
    }
}
