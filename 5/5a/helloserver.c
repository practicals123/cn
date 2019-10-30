/*************************************************************
ASSIGNMENT NO.
Write a program using TCP socket for wired network for following 
a. Say Hello to Each other ( For all students) 
b. File transfer ( For all students) 
c. Calculator (Arithmetic) 
d. Calculator (Trigonometry) 
Demonstrate the packets captured traces using Wireshark Packet Analyzer Tool for peer to peer mode
Roll No.:
Batch:
*************************************************************/ 

#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h> 
#include<stdlib.h>

#define buffsize  150

int main(void)
{
  struct sockaddr_in servaddr,clientaddr; 
  char buff[buffsize];
  int listenfd,connfd;
  int sin_size;

  if((listenfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	  perror("Socket Creation Error.\n");
  else
	  printf("Socket Created Successfully\n"); 
  bzero((char *) &servaddr, sizeof(servaddr));
  servaddr.sin_family=AF_INET;
  servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
  servaddr.sin_port=htons(5000);
  if(bind(listenfd, (struct sockaddr *)&servaddr,sizeof(servaddr)) == -1)
	 perror("Bind Error\n"); 
  listen(listenfd,4);
  sin_size = sizeof(struct sockaddr_in);
  for(;;)
  {
  if((connfd=accept(listenfd,(struct sockaddr *)&clientaddr,
			  &sin_size))==-1)
         perror("Accept Error\n");
	 strcpy(buff,"Hello Client - Server\n");
	 write(connfd,buff,strlen(buff));
	 close(connfd);
}
}
