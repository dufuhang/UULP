#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

#define PORTNUM 15000
#define HOSTLEN 256
#define oops(msg) { perror(msg); exit(1); }

void sanitize(char* str);

int main(int ac, char* av[])
{
    struct sockaddr_in saddr;
    struct hostent* hp;
    char hostname[HOSTLEN];
    int sock_id, sock_fd;
    FILE *sock_fpi, *sock_fpo;      //streams for in and out
    FILE* pipe_fp;
    char dirname[BUFSIZ];           //from client
    char command[BUFSIZ];           //for popen
    int dirlen, c;

    //Step1: ask kernel for a socket
    sock_id = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_id == -1)
    {
        oops("socket");
    }

    //Step2: bind address to socket. Address is host, port
    bzero((void*)&saddr, sizeof(saddr));
    gethostname(hostname, HOSTLEN);
    hp = gethostbyname(hostname);
    bcopy((void*)hp->h_addr, (void*)&saddr.sin_addr, hp->h_length);
    saddr.sin_port = htons(PORTNUM);
    saddr.sin_family = AF_INET;
    if (bind(sock_id, (struct sockaddr*)&saddr, sizeof(saddr)) != 0)
    {
        oops("bind");
    }
    
    //Step3: allow incoming calls with Qsize = 1 on socket
    if (listen(sock_id, 1) != 0)
    {
        oops("listen");
    }

    //main loop
    while (1)
    {
        sock_fd = accept(sock_id, NULL, NULL);
        if (sock_fd == -1)
        {
            oops("accept");
        }

        //open reading direction as buffered stream
        if ((sock_fpi = fdopen(sock_fd, "r")) == NULL)
        {
            oops("fdopen reading");
        }
        if (fgets(dirname, BUFSIZ - 5, sock_fpi) == NULL)
        {
            oops("reading dirname");
        }
        sanitize(dirname);

        //open writing direction as buffered stream
        if ((sock_fpo = fdopen(sock_fd, "w")) == NULL)
        {
            oops("fdopen writing");
        }
        sprintf(command, "ls %s", dirname);
        if ((pipe_fp = popen(command, "r")) == NULL)
        {
            oops("open");
        }


        //transfer data from ls to socket
        while ((c = getc(pipe_fp)) != EOF)
        {
            putc(c, sock_fpo);
        }
        pclose(pipe_fp);
        fclose(sock_fpo);
        fclose(sock_fpi);
    }
    return 0;
}

//不太理解{
void sanitize(char* str)
{
    char *src, *dest;
    for (src = dest = str; *src; src++)
    {
        if (*src == '/' || isalnum(*src))
        {
            *dest++ = *src;
        }
    }
    *dest = '\0';
}
