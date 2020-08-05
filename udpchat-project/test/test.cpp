#include<stdio.h>
#include<ncurses.h>
#include<unistd.h>

int main()
{
    // newwin(int lines,int  cols,int start_y,int start_x);
    initscr();
    curs_set(0);// 0:光标不可见   1:光标可见
    WINDOW* header= newwin(5,5,0,0);
    //'|'--->左右边框  '+'-->上下边框  0默认编框
    box(header,0,0);
    wrefresh(header);
    sleep(4);
    endwin();
    return 0;
}
