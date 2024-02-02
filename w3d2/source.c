#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(){
    int fd;
    int ret;
    fd = open("genesis.txt", O_RDONLY);
    ret = lseek(fd, 0, SEEK_SET); //return 0
    printf("lseek(fd, 0, SEEK_SET) will return: ");
    printf("%d", ret); //output should be 0; cursor is also at the begining
    ret = lseek(fd, 100, SEEK_CUR); //return 100
    printf("lseek(fd, 100, SEEK_CUR) will return: ");
    printf("%d", ret); //output should be 100, cursor is at the offset from beginging of 100
    char *buf = malloc(10);
    read(fd, buf, 9);
    printf("%s", buf);
    free(buf);
    lseek(fd, 0, SEEK_END);

    int bytes;
    bytes = lseek(fd, -10, SEEK_END); //return the size of the file 1656
    printf("%d", bytes);
    close(fd);
    return 0;
}