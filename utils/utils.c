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

void print_int_arr(int * array, int length)
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