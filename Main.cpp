#include <ncurses.h>
#include "Platform/Arena.hpp"

#define brow 30
#define bcol 90
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
