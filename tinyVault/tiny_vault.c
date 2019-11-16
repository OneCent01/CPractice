#include<ncurses.h>
#include<string.h>
#include<stdlib.h>

#include<termios.h>
#include<unistd.h>

struct app_state {
	int view, selected_db, db_count, select_db_step, db_creation_step, auth_step;
	char **db_names, **db_passwords, *new_db_name;
};

void free_app_state(struct app_state *state)
{
	int i = 0;
	while(i < (*state).db_count) {
		free((*state).db_names[i]);
		free((*state).db_passwords[i]);
		i++;
	}

	free((*state).db_names);
	free((*state).db_passwords);
}

void finish(struct app_state *state)
{
	// free memory
	free_app_state(state);

	// cleanup window, deallocate memory and clear screen:
	endwin();

	// terminate process: 
	exit(0);
}

void write_file(char *path, char *data)
{
	
}

void read_file(char *path)
{
	
}

char read_input_char(struct app_state *state)
{
	char c;
	c = getch();

	// if the user ever presses q, quit program:
	if(c == 'q') {
		// cleanup window, free memory, & terminate process:
		finish(state);
	}

	// if the first value is esc
	if (c == '\033') { 
		// skip the [
		c = getch();
		// the real value
		switch(c = getch()) { 
			// arrow up
			case 'A': return 'w';
			// arrow down
			case 'B': return 's';
			// arrow right
			case 'C': return 'd';
			// arrow left
			case 'D': return 'a';
		}
	}

	// default, return the typed char:
	return c;
};

char *read_input_str()
{
	char *input;

	getstr(input);

	return input;
}

void welcome_screen(struct app_state state)
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

void database_creation_screen(struct app_state state)
{
	mvwprintw(
		stdscr, 
		1, 
		1, 
		"%s", 
		"Create new database"
	);

	char *prompt;

	if(state.db_creation_step == 0) {
		prompt = "Database name: ";
	} else if(state.db_creation_step == 1) {
		prompt = "Master password: ";
	}

	mvwprintw(
		stdscr, 
		3, 
		1, 
		"%s", 
		prompt
	);
}

void database_selection_screen(struct app_state state) 
{
	mvwprintw(
		stdscr, 
		1, 
		1, 
		"%s", 
		"Select database"
	);
	
	char *db_name;
	int i = state.db_count;
	while(i > 0) {
		db_name = state.db_names[i-1];
	
		if(i-1 == state.selected_db) {
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
	
		if(i-1 == state.selected_db) {
			attroff(COLOR_PAIR(3));
			attron(COLOR_PAIR(2));
		}
	
	
		i--;
	}
	
	if(state.selected_db == state.db_count) {
		attron(COLOR_PAIR(3));
		mvwprintw(
			stdscr, 
			state.db_count * 2 + 3,
			1, 
			"%s",
			"->"
		);
	}
	
	mvwprintw(
		stdscr, 
		state.db_count * 2 + 3,
		3, 
		"%s",
		"ADD NEW DATABASE"
	);
	
	if(state.selected_db == state.db_count) { 
		attroff(COLOR_PAIR(3));
		attron(COLOR_PAIR(2));
	}
}

void database_confirm_screen(struct app_state state) 
{
	mvwprintw(
		stdscr, 
		1, 
		1, 
		"%s", 
		"Are you sure?"
	);

	mvwprintw(
		stdscr, 
		3, 
		1, 
		"%s", 
		"(y/n): "
	);
}

void database_screen(struct app_state state)
{
	attron(COLOR_PAIR(2));

	if(state.select_db_step == 0) {
		database_creation_screen(state);

	} else if(state.select_db_step == 1) {
		database_selection_screen(state);

	} else if(state.select_db_step == 2) {
		database_confirm_screen(state);

	}

	attroff(COLOR_PAIR(2));
}

void auth_screen(struct app_state state)
{
	mvwprintw(
		stdscr, 
		1, 
		1, 
		"%s", 
		"Prove ownership of this database"
	);

	mvwprintw(
		stdscr, 
		3, 
		1, 
		"%s", 
		"Password: "
	);
}

void records_screen(struct app_state state)
{

}

void record_screen(struct app_state state)
{

}

void update_screen(struct app_state state)
{
    // wipe the screen:
    clear();

    // update the screen state based on app state:
	if(state.view == 0) {
		welcome_screen(state);

	} else if(state.view == 1) {
		database_screen(state);

	} else if(state.view == 2) {
		auth_screen(state);

	} else if(state.view == 3) {
		records_screen(state);

	} else if(state.view == 4) {
		record_screen(state);

	} 

	// paint the screen to the console 
	// reflecting its internal state:
	refresh();
}

void handle_database_char_input(char input, struct app_state *state)
{
	if((*state).select_db_step == 1) {
		if(input == 'w') { // up
			if((*state).selected_db > 0) {
				(*state).selected_db--;
			}

		} else if(input == 's') { // down
			if((*state).selected_db < (*state).db_count) {
				(*state).selected_db++;
			}

		} else if(input == '\r' || input == '\n') {
			//option selected
			if((*state).selected_db == (*state).db_count) {
				// they selected "create new db", 
				// switch to that view
				(*state).select_db_step = 0;
				
			} else {
				// set it back to the default view screen
				(*state).select_db_step = 1; 
				(*state).view = 2;
			}
		}
	}
}

void handle_auth_char_input(char input, struct app_state *state)
{

}

void handle_records_char_input(char input, struct app_state *state)
{
	
}

void handle_record_char_input(char input, struct app_state *state)
{
	
}

void handle_char_input(char input, struct app_state *state)
{
	if((*state).view == 0) {
		// processing welcome view input
		// always just move on to next view...
		(*state).view = 1;

	} else if((*state).view == 1) {
		// processing database view input
		handle_database_char_input(input, state);

	} else if((*state).view == 2) {
		// processing authentication view input
		handle_auth_char_input(input, state);

	} else if((*state).view == 3) {
		// processing records view input
		handle_records_char_input(input, state);

	} else if((*state).view == 4) {
		// processing record view input
		handle_record_char_input(input, state);

	}
}

void handle_database_str_input(char *input, struct app_state *state)
{
	if((*state).db_creation_step == 0) {
		if(strlen(input) > 0) {
			(*state).new_db_name = input;
			(*state).db_creation_step++;
		}
	} else if((*state).db_creation_step == 1) {
		if(strlen(input) > 0) {			
			(*state).db_count++;

			// add the db name to the names array
			(*state).db_names = realloc((*state).db_names, (*state).db_count * sizeof(char*));
			(*state).db_names[(*state).db_count-1] = malloc(strlen((*state).new_db_name) * sizeof(char));
			strcpy((*state).db_names[(*state).db_count-1], (*state).new_db_name);

			// add it's password to the password array
			// TODO: STOP STORING PAINTEXT! HASH N' SHIT!
			(*state).db_passwords = realloc((*state).db_passwords, (*state).db_count * sizeof(char*));
			(*state).db_passwords[(*state).db_count-1] = malloc(strlen(input) * sizeof(char));
			strcpy((*state).db_passwords[(*state).db_count-1], input);

			// go back, reset state:
			(*state).select_db_step = 1;
			(*state).db_creation_step = 0;
		}
	} 
}

void handle_auth_str_input(char *input, struct app_state *state)
{
	
}

void handle_records_str_input(char *input, struct app_state *state)
{
	
}

void handle_record_str_input(char *input, struct app_state *state)
{
	
}

#define esc 27
void handle_str_input(char *input, struct app_state *state)
{
	if(input[0] == esc) {
		finish(state);
	}

	if((*state).view == 1) {
		// processing database view input
		handle_database_str_input(input, state);

	} else if((*state).view == 2) {
		// processing authentication view input
		handle_auth_str_input(input, state);

	} else if((*state).view == 3) {
		// processing records view input
		handle_records_str_input(input, state);

	} else if((*state).view == 4) {
		// processing record view input
		handle_record_str_input(input, state);

	}
}

void create_colorpairs()
{
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_BLACK);
}

int main()
{
    struct app_state state = {0, 0, 0, 1, 0, 0};
    char **db_names=NULL;

    /*
    APP VIEWS
		0: welcome,
		1: database,
			1.0: creation,
				1.0.0: name
				1.0.1: password
			1.1: selection,
			1.2: confirm_action
		2: authentication,
			1.0: enter password (embedded onfail error message)
			1.1: enter pin || 2fa method code (embedded onfail error message)
		3: records,
		4: record
    */

    initscr();

    if(has_colors() == FALSE)
	{	
		printf("Your terminal does not support color\n");
	} else {
		// init color mode:
		start_color();

		// create the colorpairs for.. coloring:
		create_colorpairs();

		while(1) {
			// clear and replaing the screen based on the state:
			update_screen(state);

			// listen for input:
			if(state.select_db_step == 0 || (state.view == 2 && state.auth_step == 0)) {
				// process user input string and modify the state accordingly:
				handle_str_input(
					read_input_str(),//<--listen for string input
					&state
				);

			} else {
				// process the user input and modify the state accordingly: 
				handle_char_input(
					read_input_char(&state), //<--listen for character input:
					&state
				);

			}
		}
	}

	finish(&state);
}