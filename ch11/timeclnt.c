#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>

#define oops(msg) { perror(msg); exit(1); }

int main(int ac, char* av[])
{
    struct sockaddr_in servadd;     //the number to call
    struct hostent* hp;             //used to get number
    int sock_id, sock_fd;           //the socket and fd
    char message[BUFSIZ];           //to receive message
    int messlen;                    //for message length

    //step1: Get a socket
    sock_id = socket(AF_INET, SOCK_STREAM, 0);  //get a line
    if (sock_id == -1)
    {
        oops("socket");
    }

    //step2: connect to server  need to build address (host, port) of server first
    bzero(&servadd, sizeof(servadd));
    hp = gethostbyname(av[1]);          //lookup hosts ip
    if (hp == NULL)
    {
        oops(av[1]);
    }
    bcopy(hp->h_addr, (struct sockaddr*)&servadd.sin_addr, hp->h_length);
    servadd.sin_port = htons(atoi(av[2]));      //fill in port number
    servadd.sin_family = AF_INET;

    if (connect(sock_id, (struct sockaddr*)&servadd, sizeof(servadd)) != 0)
    {
        oops("connect");
    }

    //step3: transfer data from server, then hangup
    messlen = read(sock_id, message, BUFSIZ);   //read stuff
    if (messlen == -1)
    {
        oops("read");
    }
    if (write(1, message, messlen) != messlen)
    {
        oops("write");      
    }
    close(sock_id);
    return 0;
}
