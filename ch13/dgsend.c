#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define oops(m, x) { perror(m); exit(x); }

int make_dgram_client_socket();
int make_internet_address(char*, int , struct sockaddr_in*);

int main(int ac, char* av[])
{
    int sock;           //use this socket to send
    char* msg;          //send this message
    struct sockaddr_in saddr;      //put sender's address here

    if (ac != 4)
    {
        fprintf(stderr, "usage: dgsend host port 'message'\n");
        exit(1);
    }
    msg = av[3];

    //get a datagram socket
    if ((sock = make_dgram_client_sock()) == -1)
    {
        oops("cannot make socket", 2);
    }

    //combine hostname and portnumber of detination into an address
    make_internet_address(av[1], atoi(av[2]), &saddr);

    //send a string through the socket to that address
    if (sendto(sock, msg, strlen(msg), 0, &saddr, sizeof(saddr)) == -1)
    {
        oops("sendto failed", 3);
    }
    return 0;
}
