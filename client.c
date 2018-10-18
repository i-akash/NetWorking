#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>

#include "send_recv.h"

int main(int argc,char *argv[])
{
   int sockfd;
   struct hostent *hostinfo;
   struct sockaddr_in target;
   char buffer[100005];

   if(argc<2)
       errore("less argc");
   hostinfo=gethostbyname(argv[1]);   

   if((sockfd=socket(PF_INET,SOCK_STREAM,0))==-1)
        errore("in socket errore");
   
    
    target.sin_family=AF_INET;
    target.sin_port=htons(80);
    target.sin_addr=*((struct in_addr*)hostinfo->h_addr);
    memset(&(target.sin_zero),'\0',8);
    
     if(connect(sockfd,(struct sockaddr *)&target,sizeof(struct sockaddr))==-1)
        errore("couldnt connect");
    puts("connected ");
    
    if(send_string(sockfd,"GET / HTTP/1.1\r\n\r\n")==-1)
         errore("in send errore");
     
     while(recv_string(sockfd,buffer))
     {
         printf("%s\n",buffer);
     }    
    return 0;
}
