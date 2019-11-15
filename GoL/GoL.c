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
	// refresh screen to match in-memory model
	refresh();
}

int main()
{	
	// Start curses mode
	initscr();
	if(has_colors() == FALSE)
	{	
		printf("Your terminal does not support color\n");
	} else {
		// Start color mode
		start_color();
		init_pair(1, COLOR_RED, COLOR_BLACK);

		attron(COLOR_PAIR(1));
		print_in_middle(stdscr, LINES / 2, 0, 0, "Welcome to your tiny vault!\n welove you");
		attroff(COLOR_PAIR(1));
	    getch(); // blocker

	}
	// end curses mode
	endwin(); 
	return 1;
}
