#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<termios.h>
#include<unistd.h>

void format_print_color(char * color, int is_bold)
{
	char **color_strings;
	color_strings = malloc(sizeof(char*) * 7);
	color_strings[0] = "red";
	color_strings[1] = "green";
	color_strings[2] = "yellow";
	color_strings[3] = "blue";
	color_strings[4] = "magenta";
	color_strings[5] = "cyan";
	color_strings[6] = "default";
	char **color_code;
	color_code = malloc(sizeof(char*) * 7);
	if(is_bold) {
		color_code[0] = "\033[1;31m";
		color_code[1] = "\033[1;32m";
		color_code[2] = "\033[1;33m";
		color_code[3] = "\033[1;34m";
		color_code[4] = "\033[1;35m";
		color_code[5] = "\033[1;36m";
	} else {
		color_code[0] = "\033[0;31m";
		color_code[1] = "\033[0;32m";
		color_code[2] = "\033[0;33m";
		color_code[3] = "\033[0;34m";
		color_code[4] = "\033[0;35m";
		color_code[5] = "\033[0;36m";
	}
	color_code[6] = "\033[0;0m";
	char *selected_color_string;
	int i, selected_color_index=-1;

	for(i = 0; i < (sizeof(color_strings)); i++) {
		if(strcmp(color, color_strings[i]) == 0) {
			selected_color_index = i;
			break;
		}
	}

	printf("%s", color_code[selected_color_index]);

	free(color_strings);

	return;
}

void print_bookend(int len) 
{
	int i;
	char text[len+1];
	text[0] = '*';
	for(i = 1; i < len; i++) {
		text[i] = '-';
	}
	text[len] = '*';
	text[len+1] = '\0';
	printf("%s", text);
	printf("\n");
	return;
}

void print_int_array(int *array)
{
	printf("[");
	int size = sizeof(array) / sizeof(int), i=0;
	for(int i = 0; i < size; i++) {
	    printf("%i", array[i]);
	    if(i < size - 1) {
	    	printf(", ");
	    }
	} 
	printf("]");
	printf("\n");

}

void print_string_array(char **string_array, int count)
{
	int i;
	printf("[");
	for(i = 0; i < count; i++) {
		printf("%s", string_array[i]);
		if(i < count-1) {
			printf(", ");
		}
	}
	printf("]\n");
	return;
}

void print_colored_string(char *text, char **colors, int *color_change_indices, int color_changes)
{
	int i, current_color_index=0;

	format_print_color("default", 0);
	for(i = 0; i < strlen(text); i++) {
		if(
			color_change_indices != NULL
			&& current_color_index < color_changes
			&& color_change_indices[current_color_index] == i
		) {
			format_print_color(colors[current_color_index], 0);

			current_color_index++;
		}
		printf("%c", text[i]);
	}
	return;
}

void mutate_string_array(char ***string_array, char *insertion, int at) 
{
	(*string_array)[at] = malloc(strlen(insertion) * sizeof(char));
	strcpy((*string_array)[at], insertion);
	return;
}

char *assign_char_value(char *base, int index, char value, int size) 
{
	char *new_string;
	new_string = malloc(size);
	int i;
	for(i = 0; i < size; i++) {
		if(i == index) {
			new_string[i] = value;
		} else {
			new_string[i] = base[i];
		}
	}
	new_string[size] = '\0';
	return new_string;
}

void print_box_row(int max_len, char *text, int text_len, int selected)
{
	char print_text[max_len+1], **colors=NULL;
	int i, colors_count=0, selected_i=-1, color_change_indices_count=0, *color_change_indices=NULL;
	print_text[0] = '|';

	if(selected != -1) {
		selected_i = (2 * selected) + 1;
	}

	// need to figure out which index is at the selected row index..
	for(i = 1; i < max_len; i++) {
		if(selected_i != -1 && i == selected_i) {
			color_change_indices = realloc(color_change_indices, (color_change_indices_count+2) * sizeof(int));

			color_change_indices[color_change_indices_count] = i;
			color_change_indices_count++;

			color_change_indices[color_change_indices_count] = i+1;
			color_change_indices_count++;
			
			colors = realloc(colors, (colors_count+1) * sizeof(char*));
			mutate_string_array(&colors, "green", colors_count);
			colors_count++;

			colors = realloc(colors, (colors_count+1) * sizeof(char*));
			mutate_string_array(&colors, "default", colors_count);
			colors_count++;
		}
		print_text[i] = text[i-1];
	}
	print_text[max_len] = '|';
	print_text[max_len+1] = '\0';
	print_colored_string(
		print_text, 
		colors, 
		color_change_indices, 
		color_change_indices_count
	);
	printf("\n");
	print_bookend(max_len);

	for(i = 0; i < colors_count; i++) {
		free(colors[i]);
	}
	free(colors);
	free(color_change_indices);

	return;
}

char * substring(char *string, int start, int end)
{
	int i, range;
	range = end - start;
	char *result;
	result = malloc(sizeof(result) * range);
	for(i = start; i < end; i++) {
		result[i - start] = string[i];
	}
	result[range] = '\0';
	return result;
}

char *string_insert_at_every_other_index(char *base, char insertion)
{
	char *new_string;
	new_string = malloc((strlen(base) * 2) - 1);
	int i;

	for(i = 0; i < strlen(base); i++) {
		new_string[i*2] = base[i];
		if(i < strlen(base)) {
			new_string[(i*2)+1] = insertion;
		}
	}
	return new_string;
}

char get_raw()
{
	int c = 0;
	static struct termios oldTermios, newTermios;
	
	tcgetattr(STDIN_FILENO, &oldTermios);
	newTermios = oldTermios;
	
	cfmakeraw(&newTermios);
	
	tcsetattr(STDIN_FILENO, TCSANOW, &newTermios);
	c = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldTermios);

	return c;
}

void process_input(char input, int *turn, char *player_chars, int *x, int *y, char **game_state, int *board_index, int spaces, int *show_controls)
{

	if(input == 'w' && (*y) > 0) {
		// up
		(*y)--;
		(*board_index) -= 3;
	} else if(input == 's' && (*y) < 2) {
		// down
		(*y)++;
		(*board_index) += 3;
	} else if(input == 'a' && (*x) > 0) {
		// left
		(*x)--;
		(*board_index)--;
	} else if(input == 'd' && (*x) < 2) {
		// right
		(*x)++;
		(*board_index)++;
	} else if(input == 'q') {
		exit(0);
	} else if(input == 'e' && (*game_state)[*board_index] == '-') {
		// find position of currently selected space in the board string
		// game_state = assign_char_value(game_state, (*board_index), player_chars[turn]);
		*game_state = assign_char_value(*game_state, *board_index, player_chars[(*turn)], spaces);
		// // then change to the other players turn
		(*x) = 0;
		(*y) = 0;
		(*board_index) = 0;
		if(*turn == 1) {
			(*turn) = 0;
		} else {
			(*turn) = 1;
		}
	} else if(input == 'h') {
		(*show_controls) = 1;
	}

	return;
}

void print_tictactoe_row(int x, int y, int board_size, char *game_state, int row)
{
	int row_x;
	if(y == row) {
		row_x = x;
	} else {
		row_x = -1;
	}
	
	print_box_row(
		board_size, 
		string_insert_at_every_other_index(
			substring(
				game_state, 
				row*3, 
				(row*3)+3
			), 
			'|'
		), 
		board_size, 
		row_x
	);

	return;
}

void print_tictactoe_board(int x, int y, int board_size, char *game_state)
{
	format_print_color("default", 0);
	system("clear");
	
	print_bookend(board_size);
	print_tictactoe_row(x, y, board_size, game_state, 0);
	print_tictactoe_row(x, y, board_size, game_state, 1);
	print_tictactoe_row(x, y, board_size, game_state, 2);
	return;
}

int board_has_n_sequential_chars(char *game_state, char target_char, int sequence)
{
	int won=-1, x=0, y=0, sequential=0, search_index;
	while(x < 3) {
		while(y < 3) {
			search_index = x + (3 * y);
			if(game_state[search_index] == target_char) {
				sequential++;
			}
			y++;
		}
		if(sequential == 3) {
			won = 1;
			return won;
		}
		sequential = 0;
		y = 0;
		x++;
	}

	x = 0;
	while(y < 3) {
		while(x < 3) {
			search_index = x + (3 * y);
			if(game_state[search_index] == target_char) {
				sequential++;
			}
			x++;
		}
		if(sequential == 3) {
			won = 1;
			return won;
		}
		sequential = 0;
		x = 0;
		y++;
	}

 	return won;
}

int board_has_win(char *game_state, char *player_chars)
{
	int won=-1, check_player_win=0, winner=-1;

	winner = board_has_n_sequential_chars(
		game_state, 
		player_chars[check_player_win], 
		3
	);
	if(winner == 1) {
		won = check_player_win;
		return won;
	}

	check_player_win++;
	winner = board_has_n_sequential_chars(
		game_state, 
		player_chars[check_player_win], 
		3
	);
	if(winner == 1) {
		won = check_player_win;
	}

	return won;
}

int tictactoe_text()
{
	char input, *game_state="---------\0", *player_chars="xo";
	int first_iter=1, show_controls=0, turn=0, winner=-1, x=0, y=0, board_size=6, index_to_change=0, board_spaces=9, row_x=-1;

	while(1) {

		// print the board
		print_tictactoe_board(x, y, board_size, game_state);

		if(first_iter == 1) {
			printf("(type h for controls)\n");
			first_iter = 0;
		}

		if(show_controls == 1) {
			printf("Move to a new space with WASD\n");
			printf("Select a space with e\n");
			printf("Quit program with q\n");
			show_controls = 0;
		}

		winner = board_has_win(game_state, player_chars);

		if(winner != -1) {
			printf("PLAYER ");
			printf("%c", player_chars[winner]);
			printf(" WINS!!!\n");
			printf("Play again? \n(y or n): ");

			input = get_raw();

			if(input == 'y') {
				game_state = "---------\0";
				turn = 0;
				winner = -1; 
				x = 0; 
				y = 0;
				index_to_change = 0;
				row_x = -1;
				continue;
			} else {
				break;
			}
		}
		// ask for the next move
		input = get_raw();

		// interpret the input, update the state
		process_input(
			input, 
			&turn, 
			player_chars,
			&x, 
			&y, 
			&game_state, 
			&index_to_change, 
			board_spaces,
			&show_controls
		);
		// repeat!
	}

	free(game_state);
	return 0;
}

int main()
{
	tictactoe_text();
	return 0;
}