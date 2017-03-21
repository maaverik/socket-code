#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>

void do_task(int port){
	struct sockaddr_in server;
    int s,n;
    int errcode;
	char buffer[50];
    s=socket(AF_INET,SOCK_DGRAM,0);
    server.sin_family=AF_INET;
    server.sin_port=port;
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    n=sizeof(server);
    
    sprintf(buffer, "thankya i'm connected on %d now!", port);
    sendto(s,buffer,sizeof(buffer),0,(struct sockaddr *)&server,n);
    
    recvfrom(s,buffer,sizeof(buffer), 0,(struct sockaddr *)&server,&n);	
    printf("server : %s\n", buffer);
    
}

int main(){
	struct sockaddr_in server,client;
    int s,n;
    char b1[100],b2[100], buffer[50];
    s=socket(AF_INET,SOCK_DGRAM,0);
    server.sin_family=AF_INET;
    server.sin_port=3000;
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    n=sizeof(server);
    
    strcpy(buffer, "gimme a port");
    sendto(s,buffer,sizeof(buffer),0,(struct sockaddr *)&server,n);
    
    int port;
    recvfrom(s,&port,sizeof(port), 0,NULL,NULL);
    printf("port : %d\n", port);
    
	do_task(port);
    
}
