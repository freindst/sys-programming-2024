#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(){
    int o_fd;
    int n_fd;
    int count;
    count = 0;

    o_fd = open("genesis.txt", O_RDONLY);
    n_fd = creat("genesis_copy2.txt", 0644);

    char *buf = (char *)malloc(2);

    while(read(o_fd, buf, 1) > 0){
        count++;
    }

    free(buf);
    close(o_fd);

    open("genesis.txt", O_RDONLY);
    buf = malloc(count + 1);
    read(o_fd, buf, count);
    write(n_fd, buf, count);

    close(o_fd);
    free(buf);

    close(n_fd);

    return 0;
}