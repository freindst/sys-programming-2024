#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>

FILE * copy;

void signalClose(){
    fclose(copy);
    exit(0);
}

int main(){
    FILE * original = fopen("genesis.txt", "r+");
    copy = fopen("genesis_copy.txt", "w+");
    int pid = fork();
    fseek(original, 0, SEEK_END);
    int total = ftell(original);
    char * buf = malloc(total / 2);
    size_t n;
    if (pid == 0) {
        //move the file pointer to the beginning
        rewind(original);
        //copy half of the txt file into the buffer
        if ((n = fread(buf, total / 2, 1, original)) == total / 2){
            printf("good\n");
        } else {
            printf("not well\n");
        }
        //write the buf into copy file pointer
        if ((n = fwrite(buf, total / 2, 1, copy)) == total / 2){
            printf("write good\n");
        } else {
            printf("write not well\n");
        }

        free(buf);
        fclose(original);
        //fclose(copy);
        signal(SIGINT, signalClose);
        for(;;);
    } else {
        //move the file pointer to the half of the file
        fseek(original, total / 2, SEEK_SET);
        //copy half of the txt file into the buffer
        if ((n = fread(buf, total / 2, 1, original)) == total / 2){
            printf("good\n");
        } else {
            printf("not well\n");
        }
        fseek(copy, total / 2, SEEK_SET);
        //write the buf into copy file pointer
        sleep(0);
        if ((n = fwrite(buf, total / 2, 1, copy)) == total / 2){
            printf("write good\n");
        } else {
            printf("write not well\n");
        }
        free(buf);
        fflush(copy);

        fclose(original);
        fclose(copy);
        kill(pid, SIGINT);
    }


    return 0;
}