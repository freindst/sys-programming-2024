#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main () {
	//open genesis.txt and read
	FILE * genesis = fopen("genesis.txt", "r");
	FILE * genesis_copy = fopen("genesis_copy.txt", "r");

	//create copy of genesis.txt and open file for reading and appending
	FILE * genesis_reversed = fopen("genesis_reversed.txt", "a+");

	//create copy of genesis.txt in reverse
	FILE * genesis_line_reversed = fopen ("genesis_line_reversed.txt", "a+");

	//fseek moves cursor to end of file and ftell returns position of cursor
	int length = fseek(genesis, 0, SEEK_END);
	length = ftell(genesis);
	
	//use fgetc to get each character one by one and use fputc to write into copy ony by one
	int c;
	int i;
	for(i = length - 1; i >= 0; i--){
		fseek(genesis, i, SEEK_SET);
		c = fgetc(genesis);
		fputc(c, genesis_reversed);
	}

	fclose(genesis);
	fclose(genesis_reversed);
	
	char *buf;
	buf = malloc(length +1);
	fseek(genesis_copy, 0, SEEK_END);
	length = ftell(genesis_copy);
	printf("%d", length);

    fseek(genesis_copy, 0, SEEK_SET);
	//copy genesis in reverse as a string
	while(fgets(buf, 100, genesis_copy) != NULL){
		printf("here");
		fputs(buf, genesis_line_reversed);
	}
	
	fclose(genesis_copy);
	fclose(genesis_line_reversed);

	return 0;
}