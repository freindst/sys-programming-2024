#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char *path = "genesis.txt";
    char *path_1 = "genesis_part_1.txt";
    char *path_2 = "genesis_part_2.txt";

    int fd;

    fd = open(path, O_RDONLY);

    int length;

    char *buf = malloc(1);
    
    int fd_1, fd_2;
    fd_1 = creat(path_1, 0644);
    int ret;
    while(ret = read(fd, buf, 1) > 0){
        write(fd_1, buf, 1);
        length++;
    }
    ftruncate(fd_1, length / 2);
    close(fd_1);
    fd_2 = creat(path_2, 0644);
    lseek(fd, length / 2, SEEK_SET);
    while(ret = read(fd, buf, 1) > 0){
        write(fd_2, buf, 1);
    }
    close(fd_2);

    close(fd);

    free(buf);

    return 0;
}