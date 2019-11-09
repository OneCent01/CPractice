#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void mutate_int(int *integer, int by) 
{
	(*integer) += by;
	return;
}

void mutate_string_array(char ***string_array, char *insertion, int at) 
{
	(*string_array)[at] = malloc(strlen(insertion) * sizeof(char));
	strcpy((*string_array)[at], insertion);
	return;
}

void add_to_list(char ***list_names, char *new_list, int *lists_count)
{
	*list_names = realloc(*list_names, ((*lists_count+1) * sizeof(char*)));
	mutate_string_array(list_names, new_list, *lists_count);
	mutate_int(lists_count, 1);
	return;
}

// colors: red, green, yellow, blue, cyan, magenta, reset
// modifiers: 
//   is_bold: integer, regular, bold
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

void todo_print_view(int view, int mode, char **lists, int list_count)
{
	int max_len=30, i;
	char *top_row_text = "TODO TEXT APP";
	char *mode_select_string = " 0 | 1 | 2 ";

	print_bookend(max_len);

	print_box_row(
		max_len, 
		top_row_text, 
		(strlen(top_row_text) * sizeof(char))
	);

	if(view == 0) {
		for(i = 0; i < list_count; i++) {
			print_box_row(
				max_len, 
				lists[i], 
				(strlen(lists[i]) * sizeof(char))
			);
		}
	}


	// if(view == 0) {
	// 	print_box_row(
	// 		max_len, 
	// 		mode_select_string, 
	// 		(strlen(mode_select_string) * sizeof(char))
	// 	);
	// } else if(view == 1) {
	// 	if(mode == 0) {
	// 		if(view == 0) {
	// 			for(i = 0; i < list_count; i++) {
	// 				print_box_row(
	// 					max_len, 
	// 					lists[i], 
	// 					(strlen(lists[i]) * sizeof(char))
	// 				);
	// 			}
	// 		}

	// 	} else if(mode == 1) {

	// 	} else if(mode == 2) {

	// 	}

	// }

	return;
}

int process_user_input(int view, char *input, char ***list_names, int *lists_count)
{
	add_to_list(
		list_names, 
		input, 
		lists_count
	);
	return 0;
}

void todo_app()
{
	int lists_count=0, view=1, mode=0, i;
	char **list_names=malloc(sizeof(char*)), input[30], char_input;

	// views: mode select and mode view
	// views: 
	// 	0. select
	// 	1. mode
	//  2. exit
	// modes:
	// 	0. add
	//  1. delete
	//  2. update
	while(view < 2) {
		todo_print_view(view, mode, list_names, lists_count);

		printf("Enter new list item: ");

		// control flow skeleton for view/mode: 

		// if(view == 0) {

		// } else if(view == 1) {
		// 	if(mode == 0) {

		// 	} else if(mode == 1) {

		// 	} else if(mode == 2) {

		// 	}
		// }
		gets(input);

		view = process_user_input(
			view, 
			input,
			&list_names, 
			&lists_count
		);
	}

	for(i = 0; i < lists_count; i++) {
		free(list_names[i]);
	}
	free(list_names);
	return;
}

int main()
{
	// char *input;
	// while(1) {
	// 	printf("Choose your color: ");
	// 	gets(input);

	// 	format_print_color(input, 0);
	// }
	todo_app();
	return 0;
}