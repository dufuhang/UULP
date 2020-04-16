#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#define HOSTLEN 256

int make_internet_adress();

int make_dgram_server_socket(int portnum)
{
    struct sockaddr_in saddr;       //build our address here
    char hostname[HOSTLEN];         //address
    int sock_id;                    //socket

    sock_id = socket(PF_INET, SOCK_DGRAM, 0);       //get a socket
    if (sock_id == -1) return -1;

    //build address and bind it to socket
    gethostname(hostname, HOSTLEN);         //where am I?
    make_internet_adress(hostname, portnum, &saddr);

    if (bind(sock_id, (struct sockaddr*)&saddr, sizeof(saddr)) != 0)
    {
        return -1;
    }
    return sock_id;
}

int make_dgram_client_socket()
{
    return socket(PF_INET, SOCK_DGRAM, 0);
}

//construct for an Internet socket address, uses hostname and port
//(host, post) -> *addrp
int make_internet_adress(char* hostname, int port, struct sockaddr_in* addrp)
{
    struct hostent* hp;
    bzero((void*)addrp, sizeof(struct sockaddr_in));
    hp = gethostbyname(hostname);
    if (hp == NULL) return -1;
    bcopy((void*)hp->h_addr, (void*)&addrp->sin_addr, hp->h_length);
    addrp->sin_port = htons(port);
    addrp->sin_family = AF_INET;
    return 0;
}

//extracts host and port from an internet socket address
//addrp->(host,port)
int get_internet_address(char* host, int len, int* portp, struct sockaddr_in* addrp)
{
    strncpy(host, inet_ntoa(addrp->sin_addr), len);
    *portp = ntohs(addrp->sin_port);
    return 0;
}
