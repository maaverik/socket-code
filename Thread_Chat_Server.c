#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
void *connection_handler(void *);

char buf[1000],msg[1000];
char reg[100][100];
char cre[100][100];
int online[100];
int soc[100];
int count=0;
char user[1000],pass[1000];


int main(int argc, char const *argv[])
{
	char remoteIP[INET6_ADDRSTRLEN];
	int listener;
	for (int i = 0; i < 100; ++i)
	{
		online[i]=0;
	}
	int new_socket,status,newfd;
	int *new_sock;
	struct sockaddr_storage remoteaddr;
	socklen_t addrlen;
	struct addrinfo hints,*res;
	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype=SOCK_STREAM;
	hints.ai_flags=AI_PASSIVE;
	status=getaddrinfo(NULL,argv[1],&hints,&res);
	if (status!=0)
	{
		fprintf(stderr, "getaddrinfo: %s\n",gai_strerror(status) );
		return 1;
	}
	listener=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if(bind(listener,res->ai_addr,res->ai_addrlen)==-1)
	{
		close(listener);
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}
	freeaddrinfo(res);
	listen(listener,10);
	while(1)
	{
		addrlen=sizeof remoteaddr;
		newfd=accept(listener,(struct sockaddr *)&remoteaddr,&addrlen);
		pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = newfd;		
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
    }	
     if (new_socket<0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}		
	
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int newfd = *(int*)socket_desc;
    char msg1[]="Please sign up or login by sending credentials in the format USERNAME PASSWORD\n";
    send(newfd, msg1,strlen(msg1),0);
	int l=recv(newfd,buf,sizeof buf,0);
	buf[l]='\0';
	int flag=0,j1=0,j2=0;
	int k,j;
					for(k=0;k< strlen(buf);k++)
					{
						if (buf[k]==' ')
						{
						flag=1;
						}
						if(flag==0)
						{
						user[j1]=buf[k];
						j1++;
						}
						if(flag==1)
						{
						pass[j2]=buf[k];
						j2++;
						}
					}
					user[j1]='\0';
					pass[j2]='\0';
					flag=0;
					for(k=0;k<count;k++)
					{
					   if(strcmp(user,reg[k])==0)
					    {
						flag=1;
						if(strcmp(pass,cre[k])==0)
						{
						send(newfd,"login success\n",strlen("login success\n"),0);
						soc[k]=newfd;
						online[k]=1;
						break;
						}
						else
						{
						send(newfd,"Auth failed\n",strlen("Auth failed\n"),0);
						close(newfd);
						break;
						}
 					    }
					}
					if(flag==0)
					{
						send(newfd,"signup success\n",strlen("signup success\n"),0);
						strcpy(reg[count],user);
						strcpy(cre[count],pass);
						soc[count]=newfd;
						online[count]=1;
						count++;
					}	
					char on[1000];
						for (int i = 0; i < count-1; ++i)
						{
							if (online[i]==1)
							{
								send(newfd,"online user:",strlen("online user:"),0);
								strcpy(on,reg[i]);
								strcat(on,"\n");
								send(newfd,on,strlen(on),0);
							}
						}				
				while(1)
				{    
					 int size=recv(newfd,buf,sizeof buf,0);
					if (size<1)
					{
						printf("connection closed\n");
						for(int p=0;p<=count;p++)
						     {
							if(newfd==soc[p])
							{
							   online[p]=0;
							}
						     }
						close(newfd);
						break;
					}
					buf[size]='\0';
					for(k=0;k<5;k++)
					{
					user[k]=buf[k];
					}
					user[k]='\0';
					k++;
					j=0;
                    while(buf[k]!='\0')
                          {
                            msg[j]=buf[k];
                            k++;
                            j++;
                          }
                    msg[j]='\0';

                    for(k=0;k<=count;k++)
					{
						if(strcmp(reg[k],user)==0)
						{
						     int id=soc[k];
						     for(int p=0;p<=count;p++)
						     {
							if(newfd==soc[p])
							{
							   strcpy(buf,reg[p]);
							   strcat(buf,":");
							   strcat(buf,msg);
							   printf("%s\n",buf );
							   send(id,buf,strlen(buf),0);
							}
						     }
						}
					}
				}

    free(socket_desc);
     
    return 0;
}

					     



