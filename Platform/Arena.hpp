#pragma once
#include <ncurses.h>

class Board
{
public: 
	Board(int height, int width)
	{
		int ymax, xmax;
		getmaxyx(stdscr,ymax,xmax);

		board_win = newwin(height, width, (ymax/2) - (height/2), (xmax/2) - (width/2));
		initialize();
	}

	void initialize()
	{
		clear();
		refresh();
	}

	void addBorder()
	{
		box(board_win,0,0);
	}

	void addat(int y, int x, chtype ch)
	{
		mvwaddch(board_win, y, x, ch);	
	}

	chtype getinput()
	{
		return wgetch(board_win);
	}

	void clear()
	{
		wclear(board_win);
		addBorder();
	}

	void refresh()
	{
		wrefresh(board_win);
	}



private:
	WINDOW * board_win;
};
