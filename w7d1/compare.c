#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(){
    char *path = "darthvader.bmp";
    char *newPath = "darthvader_new.bmp";

    int headerSize = 54;
    char * buf, * buf1;
    buf = malloc(headerSize);
    buf1 = malloc(headerSize);

    FILE * a, * b;
    a = fopen(path, "r+");
    b = fopen(newPath, "r+");

    fread(buf, headerSize, 1, a);
    fread(buf1, headerSize, 1, b);
    for(int i = 0; i < headerSize; i++){
        if (buf[i] != buf1[i]){
            printf("%d: %c - %c\n", i, buf[i], buf1[i]);
        }
    }
    free(buf);
    free(buf1);
    fclose(a);
    fclose(b);
}