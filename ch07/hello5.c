// 使字符串在屏幕四壁弹来弹去
#include <stdio.h>
#include <curses.h>
#include <string.h>

#define LEFTEDGE 10
#define RIGHTEDGE 30
#define ROW 10

int main()
{
    //char message = "Hello";
    //char blank = "         ";
    int dir  = 1;
    int pos = LEFTEDGE;

    initscr();                          //turn on curses
    clear();                            //draw some stuff
    while (1)
    {
        move(ROW, pos);
        addstr("Hello");
        move(LINES - 1, COLS - 1);
        refresh();
        sleep(1);
        move(ROW, pos);
        addstr("       ");                  //erase line

        pos += dir;                     //advance posting
        if (pos >= RIGHTEDGE)
        {
            dir = -1;
        }
        if (pos <= LEFTEDGE)
        {
            dir = 1;
        }
    }
    return 0;
}
