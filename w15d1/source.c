#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include <stdlib.h>

#define MAX 10000

int SUM = 0;

pthread_mutex_t mutex;

void * Add_t(void * arg){
    pthread_mutex_lock(&mutex);
    int n = * (int *)arg;
    SUM = SUM + n;
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void * partition(void * arg){
    int i = * (int *)arg;
    for(int j = 0; j < MAX / 100; j++){
        pthread_t t;
        int a = i * 100 + j;
        pthread_create(&t, NULL, Add_t, &a);
        pthread_join(t, NULL);
    }
    return NULL;
}

int main(){
    pthread_mutex_init(&mutex, NULL);
    for(int i = 0; i < 100; i++){
        pthread_t t;
        pthread_create(&t, NULL, partition, &i);
        pthread_join(t, NULL);
    }
    pthread_mutex_destroy(&mutex);
    printf("%d\n", SUM);
}