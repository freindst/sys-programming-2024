#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

void * Yell(){
    printf("Yellinnnnnnnng\n");
    pthread_exit(NULL);
    return NULL;
}

int main(){
    //any program must have a process. which makes it a single-process program
    //any process must have a thread created, if there is only one thread in the proces, it is a single-threaded process
    int pid;
    pid = getpid();
    pthread_t tid1;
    tid1 = pthread_self();
    pthread_t tid2; //we declared a pthread_t type variable for the new thread id
    pthread_create(&tid2, NULL, Yell, NULL);  //we run a new function in a different thread
    //Q1. Who created the new thread, and make the thread id in tid2?
    //A1. tid1. The default thread of the process is the parent thread of all threads
    //Q2. Why we do not see yelling without pthread_join?
    //A2. because the process or the parent thread ends before the new thread complete the task
    pthread_cancel(tid2);
    pthread_join(tid2, NULL);  //join is the command to ask the current thread to wait for the thread
    //which thread_id in the parameter to complete, then keep moving
    //sleep(1);   //sleep is actually ask the main thread to wait
    return 0;
}