#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>


void error(const char* msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}
int main(int argc, char** argv)
{
    /*if ont supply port number*/
    if(argc < 2)
    {
        fprintf(stderr, " port nnot provided.\n ");
        exit(EXIT_FAILURE);
    }

    /*define parameters*/
    int sockfd, newsockfd, portno, n;
    char buffer[255];

    /*define server and client addr*/
    struct sockaddr_in serv_addr, cli_addr;

    /*define socket len*/
    socklen_t clilen;
    
    /*define sockfd, and assert status*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        error(" error openning socket. ");
    }
    /*use bzero() to set mem block to 0, usually use to init buffer or data
     * structure brfore it use*/
    /*void bzero(void* s, size_t n)*/
    /*bzero is discarded, and replaced by memset() in string.h*/
    /*void* memset(void* s, int v, size_t n)*/
    /*s is pointer to the memory block to be cleared, v is the value to set
     * every byte to, n is the size of block(bytes)*/
    bzero((char*) &serv_addr, sizeof(serv_addr));

    /*convert port number*/
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /*assert bind status*/
    if(bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
    {
        error(" binding failed. ");
    }
}
