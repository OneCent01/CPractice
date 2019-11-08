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
	printf("insertion: %s", insertion);
	printf("\n");
	*string_array[0] = malloc(sizeof(insertion) * sizeof(char));
	strcpy(*string_array[0], insertion);
	return;
}

void add_to_list(char ***list_names, char *new_list, int *lists_count)
{
	*list_names = realloc(*list_names, (*lists_count+1) * sizeof(char*));
	mutate_string_array(&(*list_names), new_list, *lists_count);
	mutate_int(&(*lists_count), 1);
	return;
}

int main()
{
	int lists_count=0, i;
	char **list_names=malloc(1 * sizeof(char*)), *new_list="BINGBINGWAHOO", *base_list="test";
	list_names[0] = base_list;

	printf("list_names[0]: %s", list_names[0]);
	printf("\n");

	add_to_list(&list_names, new_list, &lists_count);
	
	
	printf("list_names[0]: %s", list_names[0]);
	printf("\n");




	for(i = 0; i < lists_count; i++) {
		free(list_names[i]);
	}

	free(list_names);

	return 0;
}