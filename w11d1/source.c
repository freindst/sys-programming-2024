#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>

int main(){
    int int_array[10];
    for(int k = 0; k < 10; k++){
        int_array[k] = k;
    }
    int sum = 0;
    int pid = fork();
    //process doesn't share memory, also not share variable
    //does it really speed up. be careful how to program
    //how can we put the value together?
    int f;
    if (pid == 0){
        f = 1;
    } else {
        f = 0;
    }
    for(int i = f; i < 10; i = i + 2){
        printf("%d\n", i);
        sum = sum + i;
    }
    /*
    for(int i = 0; i < 10; i++){
        printf("%d\n", i);
        if (pid == 0){ //child
            if (i > 10 / 2){
                sum = sum + int_array[i];
            }
        } else {
            if (i <= 10 / 2){
                sum = sum + int_array[i];
            }
        }
    }*/
    printf("The sum of array is %d.\n", sum);
    printf("The pointer of the sum is: %p.\n", &sum);
    return 0;
}