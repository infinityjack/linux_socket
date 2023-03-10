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
#include<netdb.h>


void error(const char* msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}


int main(int argc, char** argv)
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;//detail in socket.docx

    char buffer[255];
    if(argc < 3)
    {
        fprintf(stderr, " usage %s hostname port\n ", argv[0]);
        exit(EXIT_FAILURE);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
    {
        error(" error opening socket. ");
    }

    server = gethostbyname(argv[1]);

    /*assert if server not found*/
    if(server == NULL)
    {
        fprintf(stderr, " error, no such host. ");
    }

    /*reet mem*/
    memset((char*) &serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;

    /*bcopy() can copy mem from one location to another*/
    /*void bcopy(const void* src, void* dest, size_t n)*/
    /*does not perform any toye check or alignment operations*/
    /*memcpy() similar to bcopy(), but more efficient*/
    /*strcpy() can only copy str, but memcpy() can work with 
    * arbitrary block of mem*/
    bcopy((char*)server->h_addr, (char*) &serv_addr.sin_addr.s_addr,
        server->h_length);

    serv_addr.sin_port = htons(portno);

    /*assert connect status*/
    if(connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
    {
        error(" connect failed. ");
    }

    /*start conmunicate*/
    while(1)
    {
        memset(buffer, 0, 255);
        fgets(buffer, 255, stdin);
        n = write(sockfd, buffer, strlen(buffer));

        if(n < 0)
        {
            error(" error on writing. ");
        }

        memset(buffer, 0, 255);
        n = read(sockfd, buffer, 255);

        if(n < 0)
        {
            error(" error on reading. ");
        }

        printf(" server : %s ", buffer);

        int i = strncmp("bye", buffer, 3);
        int j = strncmp("Bye", buffer, 3);

        if(i == 0 || j == 0)
        {
            break;
        }
    }

    close(sockfd);
    return 0;
}
