#include <stdio.h>

int main()
{
    FILE* fp;
    char buf[100];
    int i = 0;

    fp = popen("who | sort", "r");  //open the command
    while (fgets(buf, 100, fp) != NULL) //read from command
    {
        printf("%3d %s", i++, buf);     //print data
    }
    pclose(fp);             //IMPORTANT!
    return 0;
}
