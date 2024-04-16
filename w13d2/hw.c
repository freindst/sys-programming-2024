#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

//divide and conquer
// what to divide? what to join?
// we are counting from 1 to 100;
void * count(void * args);

int lower[3], upper[3];

int sum = 0;

int main(){
    // if I would like to split it into three equal amount of work, all in master thread, how would you do?
    // how to not hard code the upper bound?
    int total = 100;
    sum = 0;
    for(int i = 0; i < 3; i++){
        lower[i] = i * 100 / 3; //lower[0] = 0, lower[1] = 33, lower[2] = 66
        upper[i] = lower[i] + 100 / 3 - 1; //upper[0] = 32, upper[1] = 65, upper[2] = 98
    }
    upper[2] = total; //make the last upper bound to be total
    pthread_t t_1, t_2, t_3;
    // part one: 1 to 33
    //count(1, 33);
    int index[3];
    for(int i = 0; i < 3; i++){
        index[i] = i;
    }
    pthread_create(&t_1, NULL, count, (void *)&index);
    // part two: 34 to 67
    //count(34, 67);
    pthread_create(&t_2, NULL, count, &index[1]);
    // part three: 67 to 100
    //count(67, 100);
    pthread_create(&t_3, NULL, count, &index[2]);
    pthread_join(t_1, NULL);
    pthread_join(t_2, NULL);
    pthread_join(t_3, NULL);

    printf("The sum of total is %d.\n", sum);

    return 0;
}

void * count(void * args){
    int i = * ((int *)args);
    for(int k = lower[i]; k <= upper[i]; k++){
        printf("%d\n", k);
        sum = sum + k;
    }
    return NULL;
}