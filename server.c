/*************
*argv[0] filename
*argv[1] server_ip_addr
*argv[2] portno
**************/



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
    /*void bzero(void* buf, size_t size)*/
    /*set the given size buf mem block all 0*/
    /*bzero is discarded, and replaced by memset() in string.h*/
    /*void* memset(void* buf, int value, size_t n)*/
    /*buf is pointer to the memory block to be cleared, value is the value to set
     * every byte to, size is the size of block(bytes)*/
     
     /*easy to say, bzero() can only set buf value to 0, while memset() can set
      * with any value.*/
    bzero((char*) &serv_addr, sizeof(serv_addr));

    /*convert port number*/
    portno = atoi(argv[1]);

    /*set the addr family of socket to AF_INET, aka ipv4*/
    serv_addr.sin_family = AF_INET;

    /*sets the ip addr of server to the ip addr of machine running the server,
     * INADDR_ANY is another const defined in the socket.h, which specifies that
     * the server should binf to all available network interfaces on machine*/
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    /*sets the port number that the server will listen on.
    * htons() is a function that converts a port number from host byte order to
    * the network byte order(big-endian).*/
    serv_addr.sin_port = htons(portno);

    /*assert bind status*/
    if(bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
    {
        error(" binding failed. ");
    }

    /*listen the port*/
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    /*accept new connection, we need a socketfd*/
    newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);

    /*assert accept status*/
    if(newsockfd < 0)
    {
        error(" error on accept. ");
    }

    /*we use loop to start chat, until input something to terminate*/
    while(1)
    {
        /*use memset() instead bzero()*/
        /*reset buf*/
        memset(buffer, 0, 255);
        
        /*read client input*/
        n = read(newsockfd, buffer, 255);

        /*assert input status*/
        if(n < 0)
        {
            error(" error on reading. ");
        }

        /*print client input*/
        printf("client : %s", buffer);
        
        /*reset buf*/
        memset(buffer, 0, 255);

        /*read bytes*/
        fgets(buffer, 255, stdin);

        /*write data*/
        n = write(newsockfd, buffer, strlen(buffer));

        if(n < 0)
        {
            error(" error on writing. ");
        }

        /*stop connect prompt*/
        int i = strncmp("Bye", buffer, 3);
        int j = strncmp("bye", buffer, 3);
        if(i == 0 || j == 0)
        {
            break;
        } 
    }

    /*disconnect*/
    close(newsockfd);
    close(sockfd);
}
