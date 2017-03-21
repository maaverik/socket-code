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

void * send_handle(void *socket){
	int sockfd = *(int *)socket;
	char buf[100];
	while(1){
		fgets(buf,1024,stdin);
		send(sockfd,buf,strlen(buf),0);
	}
}

void * recv_handle(void *socket){
	int sockfd = *(int *)socket;
	int l;
	char ret[100];
	 while(1){
		l=recv(sockfd,ret,1000,0);
		ret[l]='\0';
		printf("%s",ret);
    }
}

int main(int argc, char const *argv[])
{
	int sockfd,status;
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
    pthread_t t1, t2;
    int *socket = malloc(1);
    *socket = sockfd;
    pthread_create(&t1, NULL, send_handle, (void *)socket);
    pthread_create(&t2, NULL, recv_handle, (void *)socket);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}


