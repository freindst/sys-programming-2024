#include<stdio.h>
#include<stdlib.h>

void func(){
    int a = 1;
}

int main(){ //this is single-thread program
    //there is a process
    //there is one thread
    int a = 0;
    func();
    printf("hello world\n");
    return 0;
}