#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <strings.h>

#define PORTNUM 13000       //our time service phone number
#define HOSTLEN 26         
#define oops(msg) { perror(msg); exit(1); }

int main(int ac, char* av[])
{
    struct sockaddr_in saddr;       //build our address here
    struct hostent* hp;             //this is part of our
    char hostname[HOSTLEN];         //address
    int sock_id, sock_fd;           //line id, file desc
    FILE* sock_fp;                  //use socket as stream
    time_t thetime;                 //the time we report

    //step1: ask kernel for a socket
    sock_id = socket(PF_INET, SOCK_STREAM, 0);  //get a socket
    if (sock_id == -1)
    {
        oops("socket");
    }

    //step2: bind address to socket. Address is host, port
    bzero((void*)&saddr, sizeof(saddr));    //clear out struct
    gethostname(hostname, HOSTLEN);         //where am I?
    hp = gethostbyname(hostname);           //get info about host
    bcopy((void*)hp->h_addr, (void*)&saddr.sin_addr, hp->h_length); //fill in host part
    saddr.sin_port = htons(PORTNUM);        //fill in socket port
    saddr.sin_family = AF_INET;             //fill in addr family

    if (bind(sock_id, (struct sockaddr*)&saddr, sizeof(saddr)) != 0)
    {
        oops("bind");
    }

    //step3: allow incoming calls with Qsize = 1 on socket
    if (listen(sock_id, 1) != 0)
    {
        oops("listen");
    }

    //main loop:accept(), write(), close()
    while (1)
    {
        sock_fd = accept(sock_id, NULL, NULL);      //wait for call
        printf("Wow!got a call\n");
        if (sock_fd == -1)
        {
            oops("accept");                         //error getting calls
        }
        sock_fp = fdopen(sock_fd, "w");             //we'll write to the socket as a stram
        if (sock_fp == NULL)
        {
            oops("fdopen");                         //error
        }
        thetime = time(NULL);                       //get time, and convert to string

        fprintf(sock_fp, "The time here is...");
        fprintf(sock_fp, "%s", ctime(&thetime));
        fclose(sock_fp);                            //release connection
    }
    return 0;
}
