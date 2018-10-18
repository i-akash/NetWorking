
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <unistd.h>
#include <netdb.h>


#include "send_recv.h"

#define webRoot "/home/akash/Documents"
#define PORT 80

int get_file_size(int fd)
{
     struct stat stat_struct;
     if(fstat(fd,&stat_struct)==-1)
         return -1;

     return (int)stat_struct.st_size;   
}

void handlingConnection(int sockfd)
{
    puts("begin");
    int fd,length;
    char request[4024],resource[4024],*ptr;
    recv_string(sockfd,request);
    puts(request);
    ptr=strstr(request," HTTP/");
    if(ptr==NULL)
        errore("invalid http req");
    else
    {
        *ptr=0;
        ptr=NULL;

        if(strncmp(request,"GET ",4)==0)
           ptr=request+4;
        else if(strncmp(request,"HEAD ",5)==0)
           ptr=request+5;

        if(ptr==NULL)
        {
            errore("not acceptable get/head");
        }
        else
        {
            if(ptr[strlen(ptr)-1]=='/')
                strcat(ptr,"index.html");
            strcpy(resource,webRoot);
            strcat(resource,ptr);
            fd=open(resource,O_RDONLY,0);
            puts(resource);
            if(fd==-1)
            {
                puts("404 not found");
                send_string(sockfd,"HTTP/1.0 404 not found\r\n");
                send_string(sockfd, "Server: Tiny webserver\r\n\r\n");
                send_string(sockfd,"<html><head><title>404 not found</tile></head>");
                send_string(sockfd,"<body><h1>url no found</h1></body></html>\r\n");
            }
            else
            {
                puts("200 ok");
                send_string(sockfd,"HTTP/1.0 200 ok\r\n");
                send_string(sockfd, "Server: Tiny webserver\r\n\r\n");

                if(ptr==request+4)
                {
                       
                    if((length=get_file_size(fd))==-1)
                         errore("file size");
                    if((ptr=(char*)malloc(length))==NULL)
                         errore("malloc");
                    puts("get REq");     
                    read(fd,ptr,length);
                    *(ptr+length)='\r';
                    *(ptr+length+1)='\n';
                    puts(ptr);
                    send_string(sockfd,ptr);            
                }
                free(ptr);   

            }
            close(fd);
        }      

    }
    shutdown(sockfd,SHUT_RDWR);
}

int main()
{
    int sockfd,newsockfd,opt=1;
    struct sockaddr_in hosta,client;
    socklen_t sin_size;

    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
           errore("in socket");

           
   if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))==-1) 
            errore("in socket option");

    hosta.sin_family=AF_INET;
    hosta.sin_port=htons(PORT);
    hosta.sin_addr.s_addr=0;
    memset(&(hosta.sin_zero),'\0',8);

    puts(inet_ntoa(hosta.sin_addr));
    
    if (bind(sockfd, (struct sockaddr *)&hosta, sizeof(hosta))==-1) 
        errore("in bind erore");
    
    puts("binded");
    
    if(listen(sockfd,20)==-1)
            errore("in listen");
    puts("listended");
    
    while(1)
    {
        sin_size=sizeof(struct sockaddr_in);
        puts("b4 accept");
        newsockfd=accept(sockfd,(struct sockaddr*)&client,&sin_size);
        puts("after accept");
        if(newsockfd==-1)
            errore("in new socket");
        puts("connected");
        puts(inet_ntoa(client.sin_addr));   
        handlingConnection(newsockfd);    

    }                                  
}