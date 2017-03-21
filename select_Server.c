#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
int main(int argc, char const *argv[])
{
	fd_set master,read_fds;
	int fdmax;
	char remoteIP[INET6_ADDRSTRLEN];
	int listener;
	int newfd,status;
	struct sockaddr_storage remoteaddr;
	socklen_t addrlen;
	char buf[1000],msg[1000];
	char reg[100][100];
	char cre[100][100];
	int soc[100];
	int yes=1;
	char msg1[]="Sign up or login by sending credentials in the format USERNAME PASSWORD\n";
	struct addrinfo hints,*res;
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
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
	FD_SET(listener,&master);
	fdmax=listener;
	int count=0;
	char user[1000],pass[1000],to[1000];
	while(1)
	{
		read_fds=master;
		select(fdmax+1,&read_fds,NULL,NULL,NULL);
		for (int i = 0; i <= fdmax; ++i)
		{
			if (FD_ISSET(i,&read_fds))
			{
				if (i==listener)
				{
					addrlen=sizeof remoteaddr;
					newfd=accept(listener,(struct sockaddr *)&remoteaddr,&addrlen);
			                send(newfd, msg1,strlen(msg1),0);
					int l=recv(newfd,buf,sizeof buf,0);
					buf[l]='\0';
					int flag=0,j1=0,j2=0;
					for(int k=0;k< strlen(buf);k++)
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
					for(int k=0;k<count;k++)
					{
					   if(strcmp(user,reg[k])==0)
					    {
						flag=1;
						if(strcmp(pass,cre[k])==0)
						{
						send(newfd,"login success\n",strlen("login success\n"),0);
						FD_SET(newfd,&master);
					        if (newfd > fdmax)
					         {
						 fdmax=newfd;
					         }
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
						count++;
						FD_SET(newfd,&master);
						soc[count]=newfd;
						 if (newfd > fdmax)
					         {
						 fdmax=newfd;
					         }
					}

		                }
				else
				{
					int flag=0,j=0,j2=0,k=0;
				        int size=recv(i,buf,sizeof buf,0);
					if (size<1)
					{
						printf("connection closed\n");
						close(i);
						FD_CLR(i,&master);
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
					for(int k=0;k<=count;k++)
					{
						if(strcmp(reg[k],user)==0)
						{
						     int id=soc[k+1];
						     for(int p=0;p<=count;p++)
						     {
							if(i==soc[p])
							{
							   strcpy(buf,reg[p-1]);
							   strcat(buf,":");
							   strcat(buf,msg);
							   printf("%s\n",buf );
							   send(id,buf,strlen(buf),0);
							}
						     }
						}
					}
			      }
            }
		}
	}
	close(listener);
	return 0;
}




