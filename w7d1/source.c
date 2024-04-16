#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


//this will change the bmp
//path: the original bmp
//newPath: new bmp
//mode:
//0: copy the bmp
//1: rotate 90 degree clockwise
//2: rotate 180 degree
//3: rotate 270 degree
//4: left-to-right flip
//5: shrink to 50%
void rotate(char *path, char *newPath, int mode);

unsigned int getIntFromBytes(char * buf){
    return ((unsigned char)buf[0]) | ((unsigned char)buf[1] << 8) | ((unsigned char)buf[2] << 16) | ((unsigned char)buf[3] << 24);
}

//convert an interger to binary form
//use 4 bytes to represent an integer
//original [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, ..., 15, .. 23, ..., 31]
//char[0] = [0, 1, 2, 3, .., 7]
// we get char[0] by shifting all original bits right by 24 bits
// [0, 0, 0, 0, 0.......o0, o1, o2, o3, o4, o5, o6, o7]
// when we only keep char, it will truncate all the header 24 bits.
// 32-bit unsigned int -> [byte0, byte1, byte2, byte3]
// byte0: 25th: 32th bit
// byte1: 17th: 24th bit
// byte2: 9th: 16th bit
// byte3: 1st: 8th bit
// reverse order. left most byte is the most signifant byte
char * int_to_int(unsigned int k) {
    static char ret[4];
    ret[3] = k >> 24; //convert a 32-bit int to 8-bit int, we only keep the right most 8 bits
    ret[2] = (k << 8) >> 24;
    ret[1] = (k << 16) >> 24;
    ret[0] = (k << 24) >> 24;
    return ret;
}

int main(){
    //may be I should be a real start-war fan
    char *path = "darthvader.bmp";
    char *newPath = "darthvader_new.bmp";

    //if our rotate works, that is it.
    rotate(path, newPath, 5);

    return 0;
}

//mode:
//0: no need to explain
//1: 90 degree.
// in header, replace the width with original height, height with original width
// in body, row to column(implement)
//2: 180 degree
// row[x] -> row[height - x - 1]
// column[y] -> column[width - y - 1]
// completely reverse in pixels
//3: 270 degree
// in header, width to height, height to width
// in body, the first row become the first column, the first column become the last row
//4: only flip the pixel in the row
//5: shink 50%
// in header: width and height to half
// 75% pixel to remove == keep 25%
// [0, 1, 2, 3] keep [0]
// [p00, p01, p02, p03, ....]
// [p10, p11, p12, p13, ....]
// ->
// [p00, p02]
// every 2x2 grid, we should do a mean sampling
// every grid, we do the average between each color
// [00, 01]
// [10, 11]
// -> ((r[00] + r[01] + r[10] + r[11])/4)
// -> ((b..)/4)
// -> ((g..)/4)
// if we shrink in any percent
// we will need to sample a portion of the pixels, and generate a new one to represent the old ones
// if it is 90%, we should get 1.11111 pixel to generate 1 new pixel
void rotate(char *path, char *newPath, int mode){
    FILE * origin, * new;
    origin = fopen(path, "r");
    new = fopen(newPath, "w+");
    int widthPos = 18;
    int heightPos = 22;
    int sizePos = 34;   //this one I made a mistake in reading documents
    int headerSize = 54;
    char intBuf[4];
    fseek(origin, widthPos, SEEK_SET);
    fread(intBuf, 4, 1, origin);
    int width = getIntFromBytes(intBuf);
    fseek(origin, heightPos, SEEK_SET);
    fread(intBuf, 4, 1, origin);
    int height = getIntFromBytes(intBuf);
    if (mode == 0){
        fseek(origin, 0, SEEK_END);
        int total = ftell(origin);
        char * buf = malloc(total);
        rewind(origin);
        fread(buf, total, 1, origin);
        fwrite(buf, total, 1, new);
        free(buf);
    } else if (mode == 1){
        rewind(origin);
        //make width to be height, and height to width
        char * buf = malloc(widthPos);
        fread(buf, widthPos, 1, origin);
        fwrite(buf, widthPos, 1, new);
        unsigned int newHeight = width;
        unsigned int newWidth = height;
        free(buf);
        fwrite(int_to_int(newWidth), 4, 1, new);
        fwrite(int_to_int(newHeight), 4, 1, new);
        fseek(origin, 26, SEEK_SET);
        buf = malloc(54 - 26);
        fread(buf, 54 - 26, 1, origin);
        fwrite(buf, 54 - 26, 1, new);
        free(buf);
        //body
        buf = malloc(width * height * 3);
        char * buf1 = malloc(width * height * 3);
        fread(buf, 3, width * height, origin);
        int x, y;   //represent the old picture
        for(y = 0; y < height; y++){
            for(x = 0; x < width; x++){
                int index = y * width + x; //rotate 90degrees, x -> height - y -1; y -> x
                int new_index = (height - y - 1) + x * height;
                for(int k = 0; k < 3; k++){
                    buf1[new_index * 3 + k] = buf[index * 3 + k]; //map the old picture's pixel to the new picture, by each color
                }
            }
        }
        fwrite(buf1, 3, width * height, new);
        free(buf);
        free(buf1);
    } else if (mode == 2){
        rewind(origin);
        char * buf = malloc(headerSize);
        fread(buf, headerSize, 1, origin);
        fwrite(buf, headerSize, 1, new);
        free(buf);
        buf = malloc(width * height * 3);
        char * buf1 = malloc(width * height * 3);
        fread(buf, 3, width * height, origin);
        int x, y;
        for(y = 0; y < height; y++){
            for(x = 0; x < width; x++){
                int index = y * width + x;
                int new_index = (height - y - 1) * width + (width - x - 1); //count x and y from the end to reverse it
                for(int k = 0; k < 3; k++){
                    buf1[new_index * 3 + k] = buf[index * 3 + k];
                }
            }
        }
        fwrite(buf1, 3, width * height, new);
        free(buf);
        free(buf1);
    } else if (mode == 3){
        rewind(origin);
        //make width to be height, and height to width
        char * buf = malloc(widthPos);
        fread(buf, widthPos, 1, origin);
        fwrite(buf, widthPos, 1, new);
        unsigned int newHeight = width;
        unsigned int newWidth = height;
        free(buf);
        fwrite(int_to_int(newWidth), 4, 1, new);
        fwrite(int_to_int(newHeight), 4, 1, new);
        fseek(origin, 26, SEEK_SET);
        buf = malloc(54 - 26);
        fread(buf, 54 - 26, 1, origin);
        fwrite(buf, 54 - 26, 1, new);
        free(buf);
        //body
        buf = malloc(width * height * 3);
        char * buf1 = malloc(width * height * 3);
        fread(buf, 3, width * height, origin);
        int x, y;
        for(y = 0; y < height; y++){
            for(x = 0; x < width; x++){
                int index = y * width + x;
                int new_index = y + (width - x - 1) * height; //mapping between pixels
                for(int k = 0; k < 3; k++){
                    buf1[new_index * 3 + k] = buf[index * 3 + k];
                }
            }
        }
        fwrite(buf1, 3, width * height, new);
        free(buf);
        free(buf1);
    } else if (mode == 4){
        rewind(origin);
        char * buf = malloc(headerSize);
        fread(buf, headerSize, 1, origin);
        fwrite(buf, headerSize, 1, new);
        free(buf);
        buf = malloc(width * height * 3);
        char * buf1 = malloc(width * height * 3);
        fread(buf, 3, width * height, origin);
        int x, y;
        for(y = 0; y < height; y++){
            for(x = 0; x < width; x++){
                int index = y * width + x;
                int new_index = y * width + (width - x - 1);
                for(int k = 0; k < 3; k++){
                    buf1[new_index * 3 + k] = buf[index * 3 + k];
                }
            }
        }
        fwrite(buf1, 3, width * height, new);
        free(buf);
        free(buf1);
    } else if (mode == 5){
        //modify header first
        //move to the size
        rewind(origin);
        char * buf = (char *)malloc(widthPos);
        fread(buf, widthPos, 1, origin);
        fwrite(buf, widthPos, 1, new);
        free(buf);
        //write the new width to header
        unsigned int newWidth = (width / 2);
        char * widthBuf = int_to_int(newWidth);
        fwrite(widthBuf, 4, 1, new);
        unsigned int newHeight = (height / 2);
        char * heightBuf = int_to_int(newHeight);
        fwrite(heightBuf, 4, 1, new);
        //write the new size to header
        fseek(origin, heightPos + 4, SEEK_SET);
        char * buf1 = (char *)malloc(sizePos - heightPos - 4);
        fread(buf1, sizePos - heightPos - 4, 1, origin);
        fwrite(buf1, sizePos - heightPos - 4, 1, new);
        free(buf1);
        //write the new size to header
        unsigned int newSize = (height / 2) * (width / 2) * 3; //bytes per pixel
        char * sizeBuf = int_to_int(newSize);
        fwrite(sizeBuf, 4, 1, new);
        fseek(origin, sizePos + 4, SEEK_SET);
        char * buf2 = malloc(54 - sizePos - 4);
        fread(buf2, 54 - sizePos - 4, 1, origin);
        fwrite(buf2, 54 - sizePos - 4, 1, new);
        free(buf2);

        //work on the content
        //every 2 x 2 grid will be average to 1 pixel
        //read all pixel out into 2-d array
        char * * readBuffer = malloc(height * sizeof(char *));
        for(int i = 0; i < height; i++){
            //read the pixel or each row into row buffer
            char * row = malloc(width * 3);
            fread(row, width * 3, 1, origin);
            readBuffer[i] = row;
        }
        //create a new 2-d array half the width and height as the buffer for write
        char * writeBuffer = malloc(height / 2 * width / 2 * 3);
        int x, y;
        for(y = 0; y < newHeight; y++){
            for (x = 0; x < newWidth; x++){
                for (int channel = 0; channel < 3; channel++){
                    unsigned char p0 = readBuffer[2 * y][2 * x * 3 + channel]; //color is from 0 to 255
                    unsigned char p1 = readBuffer[2 * y][(2 * x + 1) * 3 + channel];
                    unsigned char p2 = readBuffer[2 * y + 1][2 * x * 3 + channel];
                    unsigned char p3 = readBuffer[2 * y + 1][(2 * x + 1) * 3 + channel];
                    writeBuffer[(y * newWidth + x) * 3 + channel] = (p0 + p1 + p2 + p3) / 4;
                }
            }
        }
        //write the new array into new file stream
        fwrite(writeBuffer, height / 2 * width / 2 * 3, 1, new);
        free(writeBuffer);
        for(int i = 0; i < height; i++){
            free(readBuffer[i]);
        }
        free(readBuffer);
    }
    fclose(new);
    fclose(origin);
}