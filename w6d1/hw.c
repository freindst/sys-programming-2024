#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

unsigned int getIntFromBytes(char * buf){
    return ((unsigned char)buf[0]) | ((unsigned char)buf[1] << 8) | ((unsigned char)buf[2] << 16) | ((unsigned char)buf[3] << 24);
}

int main(){
    FILE * origin = fopen("darthvador.bmp", "r");
    FILE * flipped = fopen("darthvador_flipped.bmp", "a+");

    //read the pixel of width and height from the header
    char intBuff[4];
    int width_pos = 18;
    unsigned width;
    fseek(origin, width_pos, SEEK_SET);
    fread(intBuff, 4, 1, origin);
    width = getIntFromBytes(intBuff);
    printf("image width is %d.\n", width);
    int height_pos = 22;
    unsigned height;
    fseek(origin, height_pos, SEEK_SET);
    fread(intBuff, 4, 1, origin);
    height = getIntFromBytes(intBuff);
    printf("image height is %d.\n", height);

    //copy the header to the new bmp
    int header_length = 54;
    char * header;
    header = malloc(header_length);
    //fseek(origin, 0, SEEK_SET);
    rewind(origin);
    fread(header, 54, 1, origin);
    fwrite(header, 54, 1, flipped);
    //long? long int = 64 bit int. short? short int = 16 bit int.
    //do we have 8 bit int? char

    //get the pixels line by line, flip, copy, repeatly
    for(int i = 0; i < height; i++){
        char line[3 * width];   //pixels in one row = width * 3
        char newLine[3 * width];
        fread(line, 3, width, origin);
        int from, to;
        for(int j = 0; j < width; j++){ //move from the first pixel to the last pixel in a row
            //the R in the original pixel will be still R in the new pixel
            for(int k = 0; k < 3; k++){
                from = j * 3 + k;
                to = 3 * (width - j - 1) + k;
                newLine[to] = line[from];
            }
        }
        fwrite(newLine, 3, width, flipped);
    }

    fclose(flipped);
    fclose(origin);
}