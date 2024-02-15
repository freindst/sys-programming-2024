#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(){
    //read genesis.txt
    FILE * genesis = fopen("genesis.txt", "r");
    //create genesis_copy.txt
    FILE * genesis_copy = fopen("genesis_copy.txt", "w+");
    FILE * genesis_copy_flip = fopen("genesis_copy_flip.txt", "w+");

    //use fgetc to get each characters one by one and
    //use fputc to write into genesis_copy one by one
    /*
    int c;
    while ((c = fgetc(genesis)) != EOF){
        fputc(c, genesis_copy);
    }
    */

    //use fgets and fputs

    //use fread and fwrite
    //ASCII, 1 char = 1 byte
    //UTF-16, 1 UNIT = 2 bytes
    //int32, 1 int = 4 bytes
    //bmp, rgb(red, green, blue) (0-255, 0-255, 0-255), (8bits, 8bits, 8bits) 24bit BMP
    //how many colors 24bit bmp support? 16million
    //resolution 1920 x 1080
    /*
    while( (fread(buf, 4, 20, genesis)) > 0){
        fwrite(buf, 4, 20, genesis_copy);
    } 
    */

    //flip the txt

    int length = fseek(genesis, 0, SEEK_END);
    length = ftell(genesis);
    printf("%d", length);
    int c;
    int i;
    for(i = length - 1; i >= 0; i--){
        fseek(genesis, i, SEEK_SET);
        c = fgetc(genesis);
        printf("%c", c);
        fputc(c, genesis_copy_flip);
    }

    fclose(genesis);
    fclose(genesis_copy);
    fclose(genesis_copy_flip);
}