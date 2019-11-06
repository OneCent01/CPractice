#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdio.h>

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

double calculator(char *expression, double *evaluated) 
{
	double result;
	char  *valid_operators, *operations, *next_int_string;
	int i, operation_count, is_valid_operator, *operation_indices=NULL, substring_start, substring_end, *consumed_indices, next_value_offset, result_index, evaluated_index;
	double *numbers;

	operation_count = 0;
	valid_operators = "^*/+-";

	operation_count = 0;
	for(i = 0; i < strlen(expression); i++) {
		is_valid_operator = char_arr_includes(valid_operators, expression[i]);
		if(is_valid_operator == 1) {
			operation_indices = realloc(operation_indices, sizeof(int) * i);
			operation_indices[operation_count] = i;
			operation_count++;
		} 
	}

	numbers = malloc(sizeof(double) * (operation_count+1));
	operations = malloc(sizeof(char) * operation_count);
	// create the operations array from the operation_indices array:
	for(i = 0; i <= operation_count; i++) {
		// for each iteration, I want to do two things: 
		// a) append the operation onto the operations array
		if(i < operation_count) {
			operations[i] = expression[operation_indices[i]];
			substring_end = operation_indices[i];
		} else {
			substring_end = strlen(expression);
		}

		// b) append the number to the left of the operation into the numbers array
		// slice out the string:

		if(i == 0) {
			substring_start = 0;
		} else {
			substring_start = operation_indices[i-1]+1;
		}

		next_int_string = substring(expression, substring_start, substring_end);
		
		// don't forget to convert the string to a float number with atof!
		if(char_arr_includes(next_int_string, '{') && char_arr_includes(next_int_string, '}')) {
			next_int_string = substring(expression, substring_start+1, substring_end-1);
			evaluated_index = atoi(next_int_string);
			numbers[i] = evaluated[evaluated_index];
		} else {
			numbers[i] = atof(next_int_string);
		}

		free(next_int_string);
	}
	// first go through and perform all the exponentials
	consumed_indices = malloc(sizeof(int) * operation_count);
	next_value_offset = 0;
	
	for(i = 0; i < operation_count; i++) {
		next_value_offset = 0;
		while(consumed_indices[i+1+next_value_offset] == 1) {
			next_value_offset++;
		}
		result_index = i+1+next_value_offset;
		if(operations[i] ==  '^') {
			numbers[result_index] = exponentiate(numbers[i], numbers[result_index]);
		} else if(operations[i] ==  '*') {
			numbers[result_index] = numbers[i] * numbers[result_index];
		} else if(operations[i] ==  '/') {
			numbers[result_index] = numbers[i] / numbers[result_index];
		} else if(operations[i] == '+') {
			numbers[result_index] = numbers[i] + numbers[result_index];
		} else if(operations[i] == '-') {
			numbers[result_index] = numbers[i] - numbers[result_index];
		}
		consumed_indices[i] = 1; 
	}

	result = numbers[result_index];

	// ALWAYS FREE DYNAMICALLY ALLOCATED VARIABLES TO AVOID MEMORY LEAKS
	free(operation_indices);
	free(numbers);
	free(operations);
	free(consumed_indices);

	return result;
}

char *whitespace_strip(char *string) 
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
	int is_valid=1, i, is_valid_special, is_valid_int;

	char *type, *valid_special_chars, *valid_integer_chars;
	valid_special_chars = "()^*/+-.";
	valid_integer_chars = "0123456789";

	for(i = 0; i < strlen(string); i++) {
		is_valid_int = char_arr_includes(valid_integer_chars, string[i]);
		is_valid_special = char_arr_includes(valid_special_chars, string[i]);
		
		if(is_valid_int != 1 && is_valid_special != 1) {
			is_valid = 0;
			break;
		}
	}

	return is_valid;
}


int *flat_expressions(char *expression)
{
	int *flat_expression_indices, i, opening_index, closing_index, count;
	opening_index = 0;

	flat_expression_indices = NULL;
	count = 0;

	for(i = 0; i < strlen(expression); i++) {
		if(expression[i] == '(') {
			opening_index = i;
		} else if(expression[i] == ')' && opening_index != -1) {
			closing_index = i;

			flat_expression_indices = realloc(flat_expression_indices, sizeof(int) * (count+1) * 2);

			flat_expression_indices[count * 2] = opening_index;
			flat_expression_indices[count * 2 + 1] = closing_index;


			closing_index = -1;
			opening_index = -1;
			count++;
		}
	}

	if(flat_expression_indices == NULL) {
		flat_expression_indices = realloc(flat_expression_indices, sizeof(int) * 2);

		flat_expression_indices[0] = 0;
		flat_expression_indices[1] = strlen(expression);
	}

	return flat_expression_indices;
}

char *assign_char_value(char *base, int index, char *value) 
{
	int i;
	for(i = 0; i < strlen(value); i++) {
		base[index+i] = value[i];
	}
	return base;
}

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

double recursive_calculator(char *expression, double *results)
{

	double result;
	int *flat_expression_indices, i, expression_start, expression_end, previous_expression_end, range, next_expression_length=0;
	char *next_expression, *next_recursion_expression, *next_recursion_expression_chunk, *next_recursion_expression_tail;

	flat_expression_indices = flat_expressions(expression);

	// allocate the max amount of memory the next expression
	// it'll never be larger than the original string, so allocate
	// the same amounf space the parent had
	next_expression = malloc(sizeof(char) * strlen(expression)); 
	next_recursion_expression = malloc(sizeof(char) * strlen(expression)); 
	next_recursion_expression_chunk = malloc(sizeof(char) * strlen(expression)); 

	for(i = 0; i < (sizeof(flat_expression_indices)/sizeof(int))/2; i++) {

		expression_start = flat_expression_indices[i*2];
		expression_end = flat_expression_indices[i*2+1];

		range = expression_end - expression_start;
		if(range == strlen(expression)) {
			result = calculator(expression, results);
			return result;
		} else {
			if(i > 0) {
				previous_expression_end = flat_expression_indices[i*2 - 1];
			} else {
				previous_expression_end = -1;
			}
			next_expression = substring(expression, expression_start+1, expression_end);

			result = calculator(next_expression, results);

			results = realloc(results, sizeof(double) * i);

			results[i] = result;

			free(next_expression);
			
			next_recursion_expression_chunk = substring(
				expression, 
				previous_expression_end+1, 
				expression_start
			);

			next_recursion_expression = assign_char_value(
				next_recursion_expression, 
				next_expression_length, 
				next_recursion_expression_chunk
			);

			next_expression_length += strlen(next_recursion_expression_chunk);

			next_recursion_expression_chunk = int_to_string_safe(i);

			next_recursion_expression_tail = substring(
				expression, 
				expression_start + range + strlen(next_recursion_expression_chunk), 
				strlen(expression)
			);

			// I shouldn't try and make the evaluated expressions 
			// actually back into strings. instead, I should register 
			// the float in an array and encode its index into the
			// next recurssion expression string. Then, when parsing, 
			// I can treat the encoded values as values, then fetch 
			// the actual referenced values for doing maths. some
			// ideas... wrap the evaluated index in a series of chars
			// that indicates the integer within is a reference to an 
			// evaluated value. Could use {}

			next_recursion_expression_chunk = char_array_insert(
				next_recursion_expression_chunk,
				"{",
				0
			);

			next_recursion_expression_chunk = char_array_insert(
				next_recursion_expression_chunk,
				"}",
				strlen(next_recursion_expression_chunk)
			);

			next_recursion_expression = char_array_insert(
				next_recursion_expression,
				next_recursion_expression_chunk,
				next_expression_length
			);
			next_expression_length += strlen(next_recursion_expression_chunk);

			next_recursion_expression = char_array_insert(
				next_recursion_expression,
				next_recursion_expression_tail,
				next_expression_length
			);
		}
	}

	free(next_recursion_expression_chunk);
	free(flat_expression_indices);

	return recursive_calculator(next_recursion_expression, results);
}


int main()
{
	char *input, *stripped_input;
	double result, *evaluated=NULL;

	printf("Enter operation: ");
	gets(input);

	// remove any spaces,
	stripped_input = whitespace_strip(input);
	// perform input validation,
	if(is_valid_expression(stripped_input) == 1) {
		result = recursive_calculator(stripped_input, evaluated);
		printf("Result: %f", result);
		printf("\n");
	} else {
		printf("INVALID EXPRESSION ENTERED\n");
		printf("Input must only consist of numbers, parentheses, and mathmatical operations: (^, *, /, +, -)\n");
	}

	free(stripped_input);
	free(evaluated);

	return 0;
}