#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

int main()
{
	char *input;
	int digits, letters, decimals, str_len, i;
	digits = 0;
	letters = 0;
	decimals = 0;

	printf("Enter a number, fraction, or character\n");

	scanf("%s",input);

	printf("You entered: %s", input);
	printf("\n");

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

	if(decimals > 1) {
		printf("You've entered a complex string some decimals...");
	} else if(letters == 0 && decimals == 1 && digits > 0) {
		printf("You've entered a decimal!");
	} else if(letters == 0 && digits == 1) {
		printf("You've entered a digit");
	} else if(letters == 0 && digits > 1) {
		printf("You've entered an integer!");
	} else {
		printf("You've entered an complex string!");
	}
	printf("\n");
	
	return 0;
}