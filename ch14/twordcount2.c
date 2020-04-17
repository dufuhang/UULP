#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include <stdlib.h>

int total_words;
pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER;

void* count_words(void* f);

int main(int ac, char* av[])
{
    pthread_t t1, t2;
    if (ac != 3)
    {
        printf("usage: %s file1 file2\n", av[0]);
        exit(1);
    }
    total_words = 0;
    pthread_create(&t1, NULL, count_words, (void*)av[1]);
    pthread_create(&t2, NULL, count_words, (void*)av[2]);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("%5d:total words\n", total_words);
    return 0;
}

void* count_words(void* f)
{
    char* filename = (char*)f;
    FILE* fp;
    int c, prevc = '\0';

    if ((fp = fopen(filename, "r")) != NULL)
    {
        while ((c = getc(fp)) != EOF)
        {
            //!的优先级高于&&
            if ((!isalnum(c)) && isalnum(prevc))
            {
                pthread_mutex_lock(&counter_lock);
                total_words++;
                pthread_mutex_unlock(&counter_lock);
            }
            prevc = c;
        }
        fclose(fp);
    }
    else
    {
        perror(filename);
    }
    return NULL;
}
