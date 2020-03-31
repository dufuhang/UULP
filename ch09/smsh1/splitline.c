#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smsh.h"

#define is_delim(x) ((x) == ' ' || (x) == '\t')

char* next_cmd(char* prompt, FILE* fp)
{
    char* buf;      //the buffer
    int bufspace = 0;   //total size
    int pos = 0;
    int c;          //input char*

    printf("%s", prompt);
    while ((c = getch(fp)) != EOF)
    {
        if (pos + 1 >= bufspace)
        {
            if (bufspace == 0)
            {
                buf = (char*)emalloc(BUFSIZ);
            }
            else
            {
                buf = (char*)erealloc(buf, bufspace+BUFSIZ);
            }
            bufspace += BUFSIZ;
        }
        if (c == '\n')
        {
            break;
        }
        buf[pos++] = c;
    }
    if (c == EOF && pos == 0)
    {
        return NULL;
    }
    buf[pos] = '\0';
    return buf;
}

char** splitline(char* line)
{
    char** args;
    int spots = 0;      //spots in table
    int bufspace = 0;   //bytes in table
    int argnum = 0;     //slots used
    char* cp  = line;   //pos in string
    char* start;
    int len;

    if (line == NULL)       //handle special case
    {
        return NULL;
    }
    args = (char**)emalloc(BUFSIZ); //initialize array
    bufspace = BUFSIZ;
    spots = BUFSIZ / sizeof(char*);

    while (*cp != '\0')
    {
        while (is_delim(*cp))   //skip leading spaces
        {
            cp++;
        }
        if (*cp == "\0")
        {
            break;
        }
        if (argnum+1 >= spots)
        {
            args = (char**)erealloc(args, bufspace + BUFSIZ);
            bufspace += BUFSIZ;
            spots += (BUFSIZ/sizeof(char*));
        }
        //make start, then find end of word
        start = cp;
        len = 1;
        while (*++cp != '\0' && !(is_delim(*cp)))
        {
            len++;
        }
        args[argnum++] = newstr(start, len);
    }
    args[argnum] = NULL;
    return args;
}

char* newstr(char* s, int l)
{
    char* rv = (char*)emalloc(l+1);
    rv[l] = '\0';
    strncpy(rv, s, l);
    return rv;
}

void freelist(char** list)
{
    char** cp = list;
    while (*cp)
    {
        free(*cp++);
    }
    free(list);
}

void* emalloc(size_t n)
{
    void* rv;
    if ((rv = malloc(n)) == NULL)
    {
        fatal("out of memory", " ", 1);
    }
    return rv;
}

void* erealloc(void* p, size_t n)
{
    void* rv;
    if ((rv = realloc(p,n)) == NULL)
    {
        fatal("realloc() failed", " ", 1);
    }
    return rv;
}

void fatal(char* s1, char* s2, int n)
{
    fprintf(stderr, "Error: %s, %s\n", s1, s2);
    exit(n);
}
