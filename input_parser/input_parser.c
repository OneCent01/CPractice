#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

char * inputType(char *input)
{
	char *type;
	int digits, letters, decimals, str_len, i;
	digits = 0;
	letters = 0;
	decimals = 0;


	str_len = strlen(input);

	for(i = 0; i < str_len; i++) {
		if(input[i] == '.') {
			decimals++;
		} else if(isalpha(input[i]) != 0) {
			letters++;
		} else if(isdigit(input[i]) != 0) {
			digits++;
		};
	};

	printf("Digits: %i", digits);
	printf("\n");

	printf("Letters: %i", letters);
	printf("\n");

	if(letters == 0 && decimals == 0 && digits == 0) {
		type = "null";
	} else if(letters == 0 && decimals == 1 && digits > 0) {
		type = "decimal";
	} else if(letters == 0 && digits == 1 && decimals == 0) {
		type = "digit";
	} else if(letters == 0 && digits > 1 && decimals == 0) {
		type = "integer";
	} else {
		type = "string";
	}

	return type;
}

int main()
{
	char *input, *type;
	
	printf("Enter a number, fraction, or character\n");

	scanf("%s",input);

	type = inputType(input);

	printf("You've entered a(n): %s", type);
	printf("\n");
	
	return 0;
}