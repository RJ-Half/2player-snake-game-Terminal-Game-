#include <ncurses.h>
#include "Platform/Arena.hpp"

#define bdim 30
#define brow bdim
#define bcol bdim * 2.5
using namespace std;

int main()
{
initscr();
refresh();
noecho();
curs_set(0);

Board board(brow,bcol);
board.initialize();
board.refresh();

getch();
endwin();
return 0;
}
