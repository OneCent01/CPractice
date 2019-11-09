#include<stdio.h>
#include<stdlib.h>
#include<string.h>


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

void print_box_row(int max_len, char *text, int text_len)
{
	char print_text[max_len+1];
	int i;
	print_text[0] = '|';
	for(i = 1; i < max_len; i++) {
		if(i-1 < text_len) {
			print_text[i] = text[i-1];
		} else {
			print_text[i] = ' ';
		}
	}
	print_text[max_len] = '|';
	print_text[max_len+1] = '\0';
	printf("%s", print_text);
	printf("\n");
	print_bookend(max_len);
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

int tictactoe_text()
{
	char *input, *game_state="---------";
	int turn=0, x=0, y=0, board_size=6;

	while(1) {
		print_bookend(board_size);
		print_box_row(board_size, string_insert_at_every_other_index(substring(game_state, 0, 3), '|'), board_size);
		print_box_row(board_size, string_insert_at_every_other_index(substring(game_state, 3, 6), '|'), board_size);
		print_box_row(board_size, string_insert_at_every_other_index(substring(game_state, 6, 9), '|'), board_size);

		gets(input);
	}

	return 0;
}

int main()
{
	tictactoe_text();
	return 0;
}