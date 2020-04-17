#include <stdio.h>
#include <pthread.h>

#define NUM 5

void* print_msg(char* m);

int main()
{
    pthread_t t1, t2;   //two threads
    pthread_create(&t1, NULL, print_msg, "hello");
    pthread_create(&t2, NULL, print_msg, "world\n");
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}

void* print_msg(char* m)
{
    int i;
    for (i = 0; i < NUM; i++)
    {
        printf("%s", m);
        fflush(stdout);
        sleep(1);
    }
    return NULL;
}
