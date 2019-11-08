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
	return;
}

void print_code_block()
{
	int max_len=10;
	print_bookend(max_len);
	print_box_row(max_len, "lordy", 5);
	print_bookend(max_len);
	return;
}

void todo_print_view(int view, char **lists, int list_count)
{
	int max_len=30, i;
	print_bookend(max_len);
	if(view == 0) {
		for(i = 0; i < list_count; i++) {
			print_box_row(
				max_len, 
				lists[i], 
				(strlen(lists[i]) * sizeof(char))
			);
			print_bookend(max_len);
		}
	}
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

void format_print_color(char * color, int is_bold)
{

	return;
}

void todo_app()
{
	int lists_count=0, view=0, i;
	char **list_names=malloc(sizeof(char*)), 
	*new_list="BINGBINGWAHOO", 
	*new_list_1="plingpling",
	*input;

	add_to_list(&list_names, new_list, &lists_count);
	add_to_list(&list_names, new_list_1, &lists_count);

	while(view < 3) {
		todo_print_view(view, list_names, lists_count);

		printf("Enter new list item: ");
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
	todo_app();
	return 0;
}