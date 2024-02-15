#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(){
    FILE * file;
    file = fopen("genesis.txt", "r+");
    int t;
    char * buf = malloc(10);
    while ((fgets(buf, 10, file) )){
        printf("%s", buf);
    }
    fclose(file);
    return 0;
}