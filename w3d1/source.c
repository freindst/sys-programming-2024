#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(){
    int fd;
    fd = open("source.c", O_RDONLY);

    char *buf;

    buf = malloc(3);

    int nr;

    nr = read(fd, buf, 2);

    //buf = "st";

    printf("%s", buf);


    free(buf);

    close(fd);
    return 0;

}