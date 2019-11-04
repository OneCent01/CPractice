#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdio.h>

char *input_type(char input)
{
	char *type;
	int digits, letters, decimals;
	digits = 0;
	letters = 0;
	decimals = 0;

	if(input == '.') {
		decimals++;
	} else if(isalpha(input) != 0) {
		letters++;
	} else if(isdigit(input) != 0) {
		digits++;
	};

	if(letters == 0 && decimals == 0 && digits == 0) {
		type = "null";
	} else if(letters == 0 && decimals == 1 && digits > 0) {
		type = "float";
	} else if(letters == 0 && digits == 1 && decimals == 0) {
		type = "digit";
	} else if(letters == 0 && digits > 1 && decimals == 0) {
		type = "int";
	} else if(letters == 1 && digits == 0 && decimals == 0) {
		type = "char";
	} else {
		type = "str";
	}

	return type;
}

int char_arr_includes(char *array, char value)
{
	int included, i;
	included = 0;

	for(i = 0; i < strlen(array); i++) {
		if(array[i] == value) {
			included = 1;
			break;
		}
	}

	return included;
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

double exponentiate(double num1, double exponent)
{
	double result, i;

	result = num1;
	for(i = 1; i < exponent; i++) {
		result = result * num1;
	}

	return result;
}

double calculator(char *expression) 
{
	double result;
	char  *valid_operators, *operations, *next_int_string;
	int i, operation_count, is_valid_operator, *operation_indices, substring_start, substring_end, *consumed_indices, next_value_offset, result_index;
	double *numbers;

	operation_count = 0;
	valid_operators = "^*/+-";

	// initial iteraration to determine how many operands it contains.
	// this is necessary to intialize the operation_indices at the correct size
	// if I didn't do this, I would have to allocate a set amount of space
	// for the array, which would result in the algorithm failing if there
	// are more operands than the allocated memory.
	for(i = 0; i < strlen(expression); i++) {
		is_valid_operator = char_arr_includes(valid_operators, expression[i]);
		if(is_valid_operator == 1) {
			operation_count++;
		} 
	}

	operation_indices = malloc(sizeof(int) * operation_count);

	operation_count = 0;
	for(i = 0; i < strlen(expression); i++) {
		is_valid_operator = char_arr_includes(valid_operators, expression[i]);
		if(is_valid_operator == 1) {
			operation_indices[operation_count] = i;
			operation_count++;
		} 
	}

	numbers = malloc(sizeof(double) * (operation_count+1));
	operations = malloc(sizeof(char) * operation_count);
	// // create the operations array from the operation_indices array:
	for(i = 0; i <= operation_count; i++) {
		// for each iteration, I want to do two things: 
		// a) append the operation onto the operations array
		if(i < operation_count) {
			operations[i] = expression[operation_indices[i]];
			substring_end = operation_indices[i];
		} else {
			substring_end = strlen(expression);
		}

		// // b) append the number to the left of the operation into the numbers array
		// // slice out the string:

		if(i == 0) {
			substring_start = 0;
		} else {
			substring_start = operation_indices[i-1]+1;
		}

		// next_int_string = malloc(sizeof(int) * (substring_end - substring_start));
		next_int_string = substring(expression, substring_start, substring_end);
		

		// don't forget to convert the string to a float number with atof!
		numbers[i] = atof(next_int_string);
		free(next_int_string);
	}


	// first go through and perform all the exponentials
	consumed_indices = malloc(sizeof(int) * operation_count);
	next_value_offset = 0;
	
	for(i = 0; i < operation_count; i++) {
		next_value_offset = 0;
		if(operations[i] ==  '^') {
			while(consumed_indices[i+1+next_value_offset] == 1) {
				next_value_offset++;
			}

			numbers[i+1+next_value_offset] = exponentiate(numbers[i], numbers[i+1+next_value_offset]);
			result_index = i+1+next_value_offset;
			consumed_indices[i] = 1;
		}
	}

	for(i = 0; i < operation_count; i++) {
		next_value_offset = 0;
		if(operations[i] ==  '*') {
			while(consumed_indices[i+1+next_value_offset] == 1) {
				next_value_offset++;
			}
			numbers[i+1+next_value_offset] = numbers[i] * numbers[i+1+next_value_offset];
			result_index = i+1+next_value_offset;
			consumed_indices[i] = 1;
		}
	}

	for(i = 0; i < operation_count; i++) {
		next_value_offset = 0;
		if(operations[i] ==  '/') {
			while(consumed_indices[i+1+next_value_offset] == 1) {
				next_value_offset++;
			}
			numbers[i+1+next_value_offset] = numbers[i] / numbers[i+1+next_value_offset];
			result_index = i+1+next_value_offset;
			consumed_indices[i] = 1;
		}
	}

	for(i = 0; i < operation_count; i++) {
		next_value_offset = 0;
		if(operations[i] == '+') {
			while(consumed_indices[i+1+next_value_offset] == 1) {
				next_value_offset++;
			}
			numbers[i+1+next_value_offset] = numbers[i] + numbers[i+1+next_value_offset];
			consumed_indices[i] = 1;
			result_index = i+1+next_value_offset;
		}
	}

	for(i = 0; i < operation_count; i++) {
		next_value_offset = 0;
		if(operations[i] == '-') {
			while(consumed_indices[i+1+next_value_offset] == 1) {
				next_value_offset++;
			}
			numbers[i+1+next_value_offset] = numbers[i] - numbers[i+1+next_value_offset];
			consumed_indices[i] = 1; 
			result_index = i+1+next_value_offset;
		}
	}

	result = numbers[result_index];

	// ALWAYS FREE DYNAMICALLY ALLOCATED VARIABLES TO AVOID MEMORY LEAKS
	free(operation_indices);
	free(numbers);
	free(operations);
	free(consumed_indices);

	return result;
}

char *whitespace_strip(char * string) 
{
	char *stripped;
	int i, stripped_count, whitespace_count, input_len;
	input_len = strlen(string);
	stripped_count = 0;
	whitespace_count = 0;
	for(i = 0; i < input_len; i++) {
		if(string[i] == ' ') {
			whitespace_count++;
		}
	}

	stripped = malloc(sizeof(char) * (input_len - whitespace_count));

	for(i = 0; i < input_len; i++) {
		if(string[i] != ' ') {
			stripped[stripped_count] = string[i];
			stripped_count++;
		}
	}
	stripped[(input_len - whitespace_count)] = '\0';

	return stripped;
}

int is_valid_expression(char *string)
{
	int is_valid, i, are_equal, is_valid_special, is_digit;
	is_valid = 1;
	is_valid_special = 0;

	char *type, *valid_special_chars, *digitKey;
	valid_special_chars = "^*/+-.";
	digitKey = "digit";

	for(i = 0; i < strlen(string); i++) {
		type = input_type(string[i]);

		is_digit = strcmp(type, digitKey);
		is_valid_special = char_arr_includes(valid_special_chars, string[i]);

		if(is_digit == 0 && is_valid_special != 0) {
			is_valid = 0;
			break;
		}
	}

	return is_valid;
}

// CURRENTLY LACKS SUPPORT OF RECURSIVELY NESTED OPERATIONS

int main()
{
	char *input, *stripped_input;
	double result;

	printf("Enter operation: ");
	gets(input);

	// remove any spaces,
	stripped_input = whitespace_strip(input);
	// perform input validation,
	if(is_valid_expression(stripped_input) == 1) {
		result = calculator(stripped_input);
	} else {
		printf("INVALID EXPRESSION ENTERED");
		printf("\n");
		printf("Input must only consist of digits and mathmatical operations [^, *, /, +, -]");
		printf("\n");
	}

	free(stripped_input);

	printf("Result: %f", result);
	printf("\n");

	return 0;
}