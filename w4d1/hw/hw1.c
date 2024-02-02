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
    char *buf;

    fd = open(path, O_RDONLY); //cursor is currently at the pos = 0

    int length;
    length = lseek(fd, 0, SEEK_END); //return the total bytes of the file, cursor is at the end

    int fd_1, fd_2;
    fd_1 = creat(path_1, 0644);
    fd_2 = creat(path_2, 0644);

    buf = malloc(length / 2);
    lseek(fd, 0, SEEK_SET);  //move the cursor to the head of the file;
    read(fd, buf, length / 2);  //read first half of the bytes from genesis.txt into buf
    write(fd_1, buf, length / 2);
    close(fd_1); //done with genesis_part_1.txt file create

    read(fd, buf, length / 2);
    write(fd_2, buf, length / 2);
    close(fd_2);

    close(fd);

    free(buf);

    return 0;
}