#include <stdio.h>

int main()
{
	FILE *fp;

	fp = fopen("./test.txt", "w+");
	fputs("First line...\n\nantoher line", fp);
	fclose(fp);

	return 0;
}