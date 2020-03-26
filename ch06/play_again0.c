#include <stdio.h>
#include <termios.h>

int get_response(char* question);
#define QUESTION "Do you want another transaction"

int main()
{
    int response = get_response(QUESTION);  //get some answer
    return response;
}

int get_response(char* question)
{
    printf("%s(y/n)?", question);
    while (1)
    {
        switch (getchar())
        {
            case 'Y':
            case 'y': return 0; break;
            case 'N':
            case 'n': 
            case EOF: return 1; break;
        }
    }
}

