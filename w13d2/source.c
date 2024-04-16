#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

void * CallByThread(void * args){
    char * message = (char *)args;
    printf("%s", message);
    return NULL;
}

int main(){
    // every running program has a process
    // there process in natural has a thread, that is the master thread
    pthread_t p_master = pthread_self();
    
    pthread_t thread_1, thread_2;
    char * message_1 = "I am talking to child thread 1.\n";
    pthread_create(&thread_1, NULL, CallByThread, message_1);    //we ask the new child thread with p_id to run the function
    char * message_2 = "This time, speaking to child thread 2.\n";
    pthread_create(&thread_2, NULL, CallByThread, message_2);
    pthread_join(thread_1, NULL);   //we command the master thread to continue 
    pthread_join(thread_2, NULL);
    // to next command after the child thread p_id complete its work.
    // is p_id still live here?
    // child thread will be terminated when it finishes its work
    // join is asking the master thread to listen to the terminate signal from the child thread
    return 0; //when the master thread terminated, all threads will be terminated
}