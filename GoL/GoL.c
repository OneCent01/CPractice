#include<ncurses.h>
#include<string.h>
#include<stdlib.h>

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string)
{	
	int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	mvwprintw(win, y, x, "%s", string);
	refresh();
}

int main()
{	
	initscr();// Start curses mode
	if(has_colors() == FALSE)
	{	endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
	start_color();// Start color mode
	init_pair(1, COLOR_RED, COLOR_BLACK);

	attron(COLOR_PAIR(1));
	print_in_middle(stdscr, LINES / 2, 0, 0, "Texty color screen!");
	attroff(COLOR_PAIR(1));
    getch(); // blocker
	endwin(); // end color
}


/*
	compile command: gcc -lncurses ./GoL.c
	-lncurses required to link required resources

	Resources on the topic: 
		- Ncurses programming guide: 
		  > http://www.cs.ukzn.ac.za/~hughm/os/notes/ncurses.html#using
		- NCURSES Programming HOWTO
			> http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/
		- Stack Overflow: ncurses multi colors on screen
			> https://stackoverflow.com/a/10487615/7890967
*/