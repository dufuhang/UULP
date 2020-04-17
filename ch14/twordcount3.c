#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include <stdlib.h>

struct arg_set
{
    char* fname;
    int count;
};

void* count_words(void* f);

int main(int ac, char* av[])
{
    pthread_t t1, t2;
    struct arg_set arg1, arg2;
    if (ac != 3)
    {
        printf("usage: %s file1 file2\n", av[0]);
        exit(1);
    }
    arg1.fname = av[1];
    arg1.count = 0;
    pthread_create(&t1, NULL, count_words, (void*)&arg1);
    arg2.fname = av[2];
    arg2.count = 0;
    pthread_create(&t2, NULL, count_words, (void*)&arg2);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("%5d: %s\n", arg1.count, av[1]);
    printf("%5d: %s\n", arg2.count, av[2]);
    printf("%5d: total words\n", arg1.count + arg2.count);
    return 0;
}

void* count_words(void* a)
{
    struct arg_set* args = a;
    FILE* fp;
    int c, prevc = '\0';

    if ((fp = fopen(args->fname, "r")) != NULL)
    {
        while ((c = getc(fp)) != EOF)
        {
            //!的优先级高于&&
            if ((!isalnum(c)) && isalnum(prevc))
            {
                args->count++;
            }
            prevc = c;
        }
        fclose(fp);
    }
    else
    {
        perror(args->fname);
    }
    return NULL;
}
