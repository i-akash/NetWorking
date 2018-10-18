#include<stdio.h>
#include<netdb.h>
#include<arpa/inet.h>


int main(int argc,char *argv[])
{
  struct hostent *hostInfo;
  struct in_addr *address;

  if(argc<2)
   {
      puts("no DNS name");
      return 0;
   }
   
  hostInfo=gethostbyname(argv[1]);
  if(hostInfo==NULL)
   {
      puts("ERRORE");
      return 0;
   }
  address=(struct in_addr*)(hostInfo->h_addr);
  printf("%s has IP : %s\n",argv[1],inet_ntoa(*address));
  return 0;
}
