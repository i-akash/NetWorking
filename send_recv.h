#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void errore(const char *mess)
{
    puts(mess);
    exit(1);
}

int send_string(int toSockFd,const char *buffer)
{
   int bytesToSend=strlen(buffer);
   int bytesSent=0;
   
    while(bytesToSend>0)
    {
       bytesSent=send(toSockFd,buffer,bytesToSend,0);
       if(bytesSent=-1)return 0;
       
       bytesToSend-=bytesSent;
       buffer+=bytesSent;
    }
    return 1;
}


int recv_string(int fromSockFd,char* getBuffer)
{
    #define EOL "\r\n"
    #define EOL_size 2
    
    int eol=0;
     
    while(recv(fromSockFd,getBuffer,1,0)==1)
     {
        if(*getBuffer==EOL[eol])
        {
            eol++;
            if(eol==EOL_size)
             {
                *(getBuffer-EOL_size+1)='\0';
                 return (strlen(getBuffer));
             } 
        }
        else eol=0;
        
        getBuffer++; 
     }
     return 0;
}
