// 通过一个loop来观察curses函数如何运转
#include <stdio.h>
#include <curses.h>

int main()
{
    int i;
    initscr();          //turn on curses
    clear();            //draw some stuff
    for (i = 0; i < LINES; i++)
    {
        move(i, i + 1);
        if (i % 2 == 1)
        {
            standout();
        }
        addstr("Hello,world");
        if (i % 2 == 1)
        {
            standend();
        }
    }
    refresh();          //update the screen
    getch();            //wait for user inout
    endwin();           //reset the tty etc
    return 0;
}
