#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>


int main(){
    FILE * origin, * reverse;
    origin = fopen("genesis.txt", "r");
    reverse = fopen("genesis_reverse_line.txt", "a+");

    char * *buff;
    buff = malloc(sizeof(char *) * 1000);
    int count;
    count = 0;

    bool stop = false;
    //read lines into 2-d char array
    while(!stop){
        char * loop;    //why we do not need to free loop
        char * line_buff;   //why we should free line_buff
        line_buff = malloc(1000);
        loop = fgets(line_buff, 1000, origin);
        buff[count] = malloc(1000);
        strcpy(buff[count], line_buff);
        count++;
        stop = loop == NULL;
        free(line_buff);
        free(loop);
    }

    //write the lines in reversed order
    for(int i = 0; i < count; i++){
        fputs(buff[count - i - 1], reverse);
        free(buff[count - i - 1])
    }


    fclose(origin);
    fclose(reverse);
    free(buff);

    return 0;
}