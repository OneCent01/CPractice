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
	// printf("text: %s", text);
	// printf("\n");
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

void update_positions(char input, int *x, int *y, int *board_index)
{
	if(input == 'w') {
		// up
		if((*y) > 0) {
			(*y)--;
			(*board_index) -= 3;
		}
	} else if(input == 's') {
		// down
		if((*y) < 2) {
			(*y)++;
			(*board_index) += 3;
		}
	} else if(input == 'a') {
		// left
		if((*x) > 0) {
			(*x)--;
			(*board_index)--;
		}
	} else if(input == 'd') {
		// right
		if((*x) < 2) {
			(*x)++;
			(*board_index)++;
		}
	} else if(input == 'q') {
		exit(0);
	} 

	return;
}

int tictactoe_text()
{
	char input, *game_state="---------\0";
	int turn=0, x=0, y=0, board_size=6, index_to_change=0, spaces=9;


	while(1) {
		format_print_color("default", 0);
		system("clear");

		print_bookend(board_size);
		if(y == 0) {
			print_box_row(
				board_size, 
				string_insert_at_every_other_index(
					substring(
						game_state, 
						0, 
						3
					), 
					'|'
				), 
				board_size, 
				x
			);
		} else {
			print_box_row(
				board_size, 
				string_insert_at_every_other_index(
					substring(
						game_state, 
						0, 
						3
					), 
					'|'
				), 
				board_size, 
				-1
			);
		}
		if(y == 1){
			print_box_row(
				board_size, 
				string_insert_at_every_other_index(
					substring(
						game_state, 
						3, 
						6
					), 
					'|'
				), 
				board_size, 
				x
			);
		} else {
			print_box_row(
				board_size, 
				string_insert_at_every_other_index(
					substring(
						game_state, 
						3, 
						6
					), 
					'|'
				), 
				board_size, 
				-1
			);
		}

		if(y == 2){
			print_box_row(
				board_size, 
				string_insert_at_every_other_index(
					substring(
						game_state, 
						6, 
						9
					), 
					'|'
				), 
				board_size, 
				x
			);
		} else {
			print_box_row(
				board_size, 
				string_insert_at_every_other_index(
					substring(
						game_state, 
						6, 
						9
					), 
					'|'
				), 
				board_size, 
				-1
			);
		}

		input = get_raw();

		update_positions(input, &x, &y, &index_to_change);

		char *player_chars = "xo";

		if(input == 'e') {
			// change the board at the current spot if possible
			printf("\n");
			if(game_state[index_to_change] == '-') {
				// find position of currently selected space in the board string
				// game_state = assign_char_value(game_state, index_to_change, player_chars[turn]);
				game_state = assign_char_value(game_state, index_to_change, player_chars[turn], spaces);
				// // then change to the other players turn
				if(turn == 1) {
					turn = 0;
				} else {
					turn = 1;
				}

			}
		}
	}

	free(game_state);

	return 0;
}

int main()
{
	tictactoe_text();
	return 0;
}