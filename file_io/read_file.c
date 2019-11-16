#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>

void assign_string(char *base, int index, char *value) 
{
	int i;
	for(i = 0; i < strlen(value); i++) {
		base[index+i] = value[i];
	}
}

int main()
{
	// CHECK whether the file exists
	char *path = "./test.txt";
	int file = open(path,O_RDONLY);
	struct stat sb;

	if(file == -1) {
		printf("(1) UNABLE TO OPEN FILE\n");
		return 0;
	} else if(fstat(file, &sb) == -1) {
		printf("(2) UNABLE TO LOCATE FILE\n");
		return 0;
	}

	// READ
	FILE *fp;
	char buffer[255], **results=NULL;
	int chunk_count = 0, char_count = 0;

	// open the file
	fp = fopen(path, "r");

	// iteratively get chunks from the file
	while(fgets(buffer, sizeof(buffer), (FILE*)fp)) {
		// add memory location for new result: 
		results = realloc(results, (chunk_count+1) * sizeof(char*));
		results[chunk_count] = malloc(strlen(buffer));

		// copy the string chunk into the temp storage array:
		strcpy(results[chunk_count], buffer);
		chunk_count++;

		// keep track of number of chars read, can be later
		// used to initialize the final string at the correct
		// length
		char_count += strlen(buffer);
	}

	// initialize the results string: 
	char final[char_count];

	// build the final string from the buffer chunks:
	int i, char_i=0;
	for(i = 0; i < chunk_count; i++) {
		// just like strcpy, but has a starting index: 
		assign_string(final, char_i, results[i]);

		// char_i tracks the number of characters alread
		// set in the final string to be used as an insertion
		// index
		char_i += strlen(results[i]);

		free(results[i]);
	}

	fclose(fp);

	free(results);

	printf("final: %s", final);
	printf("\n");

	return 0;
}