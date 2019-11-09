void print_char_arr(char * array, int length)
{
	printf("[");
	for(int i = 0; i < strlen(array) || i < length; i++) {
	    printf("%d", array[i]);
	    if(i < length - 1) {
	    	printf(", ");
	    }
	} 
	printf("]");
	printf("\n");
}

void print_int(int * array, int length)
{
	printf("[");
	for(int i = 0; i < length; i++) {
	    printf("%d", array[i]);
	    if(i < length - 1) {
	    	printf(", ");
	    }
	} 
	printf("]");
	printf("\n");
}

void print_int_array(int **array)
{
	printf("[");
	int size = sizeof(array) / sizeof(int*), i=0;
	for(int i = 0; i < size; i++) {
	    printf("%i", array[i]);
	    if(i < length - 1) {
	    	printf(", ");
	    }
	} 
	printf("]");
	printf("\n");

}

void print_double_arr(double * array, int length)
{
	printf("[");
	for(int i = 0; i < length; i++) {
	    printf("%f", array[i]);
	    if(i < length - 1) {
	    	printf(", ");
	    }
	} 
	printf("]");
	printf("\n");
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
	color_strings[4] = "cyan";
	color_strings[5] = "magenta";
	color_strings[6] = "reset";
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

// The most interoperable and safe way to convert an
// integer into a string is by using the formatting
// utilities utilized by the C's native print module.
char * int_to_string_safe(integer)
{
	char *formatted;

	formatted = malloc(sizeof(int) * 10);

	int format = snprintf(
		formatted,
		10,
		"%d",
		integer
	);

	return formatted;
}

// inserts the insert string into the base, returing a new string
// with a length of the base and insertion length combined
char *char_array_insert(char *base, char *insertion, int at)
{
	char *result, next_result;
	int i, final_length;

	final_length = strlen(base) + strlen(insertion);
	result = malloc(sizeof(char) * final_length+1);

	for(i = 0; i < final_length; i++) {
		if(i < at) {
			next_result = base[i];
		} else if(i < (at + strlen(insertion))) {

			next_result = insertion[i - at];
		} else {
			next_result = base[i - strlen(insertion)];
		}
		result[i] = next_result;
	}

	return result;
}

// overwrites the values in a character array with a given string
// starts at the indicated location. Returns a the modified base
// string without changing it's size. 
char *assign_char_value(char *base, int index, char *value) 
{
	int i;
	for(i = 0; i < strlen(value); i++) {
		base[index+i] = value[i];
	}
	return base;
}

// takes two args: a pointer to an integer to mutate, 
// and an integer to mutate it by
void mutate_int(int *integer, int by) 
{
	(*integer) += by;
	return;
}

// accepts three args: pointer to target string array, the string to inject,
// and the index at wich it should be injected
void mutate_string_array(char ***string_array, char *insertion, int at) 
{
	printf("insertion: %s", insertion);
	printf("\n");
	*string_array[at] = malloc(sizeof(insertion) * sizeof(char));
	strcpy(*string_array[0], insertion);
	return;
}