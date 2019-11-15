#include<ncurses.h>
#include<string.h>
#include<stdlib.h>

#include<termios.h>
#include<unistd.h>

// void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string)
// {	
// 	int length, x, y;
// 	float middle;

// 	if(win == NULL)
// 		win = stdscr;
// 	getyx(win, y, x);
// 	if(startx != 0)
// 		x = startx;
// 	if(starty != 0)
// 		y = starty;

// 	length = strlen(string);
// 	middle = (width - length)/ 2;
// 	x = startx + (int)middle;

// 	mvwprintw(win, y, x, "%s", string);

// 	refresh();
// }

void welcome_screen()
{	
	attron(COLOR_PAIR(2));
	char *print_string = "Welcome to your little vaulty\0";

	mvwprintw(
		stdscr, 
		1, 
		1, 
		"%s", 
		print_string
	);

	mvwprintw(
		stdscr, 
		3,
		1, 
		"%s",
		"[Press any key to continue]"
	);

	attroff(COLOR_PAIR(2));
}

void database_screen(int selected_db, int select_db_step,  char **db_names, int dbs_count)
{
	attron(COLOR_PAIR(2));

	mvwprintw(
		stdscr, 
		1, 
		1, 
		"%s", 
		"Select database"
	);

	char *db_name;
	int i = dbs_count;
	while(i > 0) {
		db_name = db_names[i-1];

		if(i-1 == selected_db) {
			attron(COLOR_PAIR(3));
			mvwprintw(
				stdscr, 
				i * 2 + 1,
				1, 
				"%s",
				"->"
			);
		}

		mvwprintw(
			stdscr, 
			i * 2 + 1,
			3, 
			"%s",
			db_name
		);

		if(i-1 == selected_db) {
			attroff(COLOR_PAIR(3));
			attron(COLOR_PAIR(2));
		}


		i--;
	}

	if(selected_db == dbs_count) {
		attron(COLOR_PAIR(3));
		mvwprintw(
			stdscr, 
			dbs_count * 2 + 3,
			1, 
			"%s",
			"->"
		);
	}

	mvwprintw(
		stdscr, 
		dbs_count * 2 + 3,
		3, 
		"%s",
		"ADD NEW DATABASE"
	);

	if(selected_db == dbs_count) { 
		attroff(COLOR_PAIR(3));
		attron(COLOR_PAIR(2));
	}

	attroff(COLOR_PAIR(2));
}

void auth_screen()
{

}

void records_screen()
{

}

void record_screen()
{

}

void update_screen(int view, int selected_db, int select_db_step, char **db_names, int dbs_count)
{
	/*
		0: welcome,
		1: select,
		2: form
    */
    clear();
	if(view == 0) {
		welcome_screen();
	} else if(view == 1) {
		database_screen(selected_db, select_db_step, db_names, dbs_count);
	} else if(view == 2) {
		auth_screen();
	} else if(view == 3) {
		records_screen();
	} else if(view == 4) {
		record_screen();
	} 
	refresh();
}

void handle_input(char input, int *view, int *selected_db, int *select_db_step, int db_count)
{
	if(*view == 0) {
		*view = 1;
	} else if(*view == 1) {
		if(input == 'w') { // up
			if(*selected_db > 0) {
				(*selected_db)--;
			}
		} else if(input == 's') { // down
			if(*selected_db < db_count) {
				(*selected_db)++;
			}
		} else if(input == '\r' || input == '\n') {
			//option selected
			if(*selected_db == db_count) {
				// they selected "create new db", 
				// switch to that view
				
			}
		}
	}
}

char read_input_char()
{
	char c;
	c = getch();
	if (c == '\033') { // if the first value is esc
		c = getch(); // skip the [
		switch(c = getch()) { // the real value
			case 'A':
				// code for arrow up
				return 'w';
			case 'B':
				// code for arrow down
				return 's';
			case 'C':
				// code for arrow right
				return 'd';
			case 'D':
				// code for arrow left
				return 'a';
		}
	} 
	return c;
};

int main(int argc, char *argv[])
{
    initscr();

    int view = 0, selected_db=0, select_db_step=1, db_count=1;

    char **db_names;
    db_names = malloc(sizeof(char*));
    db_names[0] = "test_dummy";

    /*
    APP VIEWS
		0: welcome,
		1: database,
			1.0: creation,
			1.1: selection,
			1.2: authentication,
			1.3: confirm_action
		2: authentication,
			1.0: enter password (embedded onfail error message)
			1.1: enter pin || 2fa method code (embedded onfail error message)
		3: records,
		4: record
    */


    if(has_colors() == FALSE)
	{	
		printf("Your terminal does not support color\n");
	} else {
		start_color();
		init_pair(1, COLOR_RED, COLOR_BLACK);
		init_pair(2, COLOR_GREEN, COLOR_BLACK);
		init_pair(3, COLOR_WHITE, COLOR_BLACK);

		char c;
		while(1) {
			update_screen(view, selected_db, select_db_step, db_names, db_count);
			// listen for input...
	    	c = read_input_char();
	    	if(c == 'q') {
	    		break;
	    	} else {
	    		handle_input(
	    			c, // input
	    			&view, // view pointer
	    			&selected_db, // selected db pointer
	    			&select_db_step, // db selection view step pointer
	    			db_count // how many dbs there are
	    		);
	    	}
		}
	}

    endwin();

    return 0;
}