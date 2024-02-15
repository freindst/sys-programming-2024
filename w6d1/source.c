#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

unsigned int getIntFromBytes(char * buf) {
        return ((unsigned char)buf[0]) | ((unsigned char)buf[1] << 8) | ((unsigned char)buf[2] << 16) | ((unsigned char)buf[3] << 24);
    }

int main() {
    FILE * origin = fopen("darthvador.bmp", "r");
    FILE * flipped = fopen("darthvador_flipped.bmp", "a+");

    //read pixel width and height from header 
    char intBuff[4];
    
    int width_pos = 18;
    unsigned width;
    fseek(origin, width_pos, SEEK_SET);
    fread(intBuff, 4, 1, origin);
    width = getIntFromBytes(intBuff);

    int height_pos = 22;
    unsigned height;
    fseek(origin, height_pos, SEEK_SET);
    height = getIntFromBytes(intBuff);

    //copy header to new bmp file
    int header_length = 54;
    char * header;
    header = malloc(header_length);
    fseek(origin, 0, SEEK_SET);
    fread(header, 54, 1, origin);
    fwrite(header, 54, 1, flipped);
    
    //get pixels line by line, flip, copy, and repeat
    for(int i = 0; i < height; i++)
    {
        char * line = malloc(3 * width);
        char * newLine = malloc(3 * width);
        fread(line, 3, width, origin);
        int from, to;
        for(int j = 0; j < width; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                from = j * 3 + k;
                to = 3 * (width - j) + k;
                newLine[to] = line[from];
            }
        }

        fwrite(newLine, 3, width, flipped);
        free(line);
        free(newLine);
    }

    fclose(flipped);
    fclose(origin);
}