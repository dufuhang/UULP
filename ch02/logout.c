#include <stdio.h>
#include <unistd.h>

int logout_tty(char* line)
{
    int fd;
    struct utmp rec;
    int len = sizeof(struct utmp);
    int retval = -1;
    //open file
    if ((fd = open(UTMP_FILE, O_RDWR)) == -1)
    {
        return -1;
    }
    //search and replace
    while (read(fd, &rec, len) == len)
    {
        if (strncmp(rec.ut_line, line, sizeof(rec.ut_line)) == 0)
        {
            //set type
            rec.ut_type = DEAD_PROCESS;
            //and time
            if (time(&rec.ut_time != -1))
            {
                //back up
                if (lseek(fd, -len, SEEK_CUR) != -1)
                {
                    //update
                    if (write(fd, &rec, len) == len)
                    {
                        //success
                        retval = 0;
                    }
                }   
            }
            break;
        }
    }
    //close file
    if (close(fd) == -1)
    {
        retval = -1;
    }
    return retval;
}
