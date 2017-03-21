#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
void *connection_handler1(void *);
void *connection_handler2(void *);
int main(int argc, char const *argv[])
{
	int sockfd,status;
	int *new_sock;
	char buf[1000],ret[1000];
	struct addrinfo hints,*res;
	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype= SOCK_STREAM;
 	status=getaddrinfo(argv[1],argv[2],&hints,&res);
 	if (status!=0)
	{
		fprintf(stderr, "getaddrinfo: %s\n",gai_strerror(status) );
		return 1;
	}
 	sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if (sockfd==-1)
	{
		fprintf(stderr, "client: failed to bind\n");
		exit(1);
	}
	if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) 
	{
		close(sockfd);
		perror("client: connect");
		exit(1);
	}
	freeaddrinfo(res);
	int l=recv(sockfd,ret,1000,0);
        ret[l]='\0';
        printf("%s",ret);
        fgets(buf,1024,stdin);
        send(sockfd,buf,strlen(buf),0);
	l=recv(sockfd,ret,1000,0);
	ret[l]='\0';
        printf("%s\n",ret);
        pthread_t sniffer_thread1,sniffer_thread2;
        new_sock = malloc(1);
        *new_sock = sockfd;
       if( pthread_create( &sniffer_thread1 , NULL ,  connection_handler1 , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 0;
        }
        
        if( pthread_create( &sniffer_thread2, NULL ,  connection_handler2 , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 0;
        }
        pthread_join( sniffer_thread1 , NULL);  
    return 0;
}
void *connection_handler1(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    char message[1000];  
    while(1)
    {
    	printf("\n");
    fgets(message,1024,stdin);
	send(sock,message,strlen(message),0);
    }
    free(socket_desc);
}
void *connection_handler2(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
     
   char message[1000]; 
    while(1)
    { 
       int l=recv(sock,message,1000,0);
       message[l]='\0';
       printf("%s",message);
    }
    free(socket_desc);
}