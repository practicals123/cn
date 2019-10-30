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
//client program
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<string.h>
#include<netdb.h>
#include<stdlib.h>

main()
{
    struct sockaddr_in serveraddr;
    int clientsock,n,rdret,length;
    char filename[20],filedata[300];
    bzero((char*)&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=2000;
    serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    clientsock=socket(AF_INET,SOCK_STREAM,0);
    if(connect(clientsock,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0)
    {
        printf("\nError:Cannot connect...");
        exit(0);
    }
    printf("Enter the name of the file : ");
    scanf("%s",filename);
    length=strlen(filename);
    write(clientsock,filename,length);
    rdret=read(clientsock,filedata,300);
    printf("\nThe contents of the file: \n\n");
    printf("%s",filedata);
    close(clientsock);
}

/*----------------------Output---------------------------

gescoe@ubuntu:~/Desktop$ gcc tcpfileClient.c 
gescoe@ubuntu:~/Desktop$ ./a.out
Enter the name of the file : tcp.txt

The contents of the file: 

R.H.Sapat college of Engineering
Welcome to GESCOE CNlab
This is TCP FileTransfer Program

gescoe@ubuntu:~/Desktop$
------------------------------------------------------*/
