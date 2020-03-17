#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
void show_info(struct utmp * utbufp);
#define SHOWHOST

int main()
{
    //deine a struct of utmp
    struct utmp current_record;
    //define a descriptor
    int utmpfd;
    //size of utmp
    int reclean = sizeof(current_record);

    if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1) 
    {
        perror(UTMP_FILE);
        exit(1);
    }
    while (read(utmpfd, &current_record, reclean) == reclean)
    {
        show_info(&current_record);
    }
    close(utmpfd);
    return 0;
}
void show_info(struct utmp * utbufp)
{
    //user only
    if (utbufp->ut_type != USER_PROCESS)
    {
        return;
    }
    printf("% -8.8s", utbufp->ut_name);
    printf(" ");
    printf("% -8.8s", utbufp->ut_line);
    printf(" ");
    printf("% 10ld", utbufp->ut_host);
    printf(" ");
# ifdef SHOWHOST
    printf("(%s)", utbufp->ut_host);
# endif 
    printf("\n");
}
