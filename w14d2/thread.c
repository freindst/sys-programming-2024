#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define Size 100000000
#define MaxLevel 7


// we divide the merge sort into two part
// first part is the single threaded divide, divide the array into 2^6 for 128 pieces
// for each part, create a thread to run the merge sort on the small part
// use a single thread to merge the sorted pieces together

struct MergeSortStruct{
    int begin;
    int end;
    int level;
};

int Array[Size];

// for merge
// given the index of left, middle, and right
void Merge(int left, int middle, int right);

// for divide or merge sort
void MergeSort(int begin, int end);

void * MergeSort_T(void * arg);

int main(){
    //initiate a random seed, and randomly assign an integer into array between 0 to 99
    time_t t;
    srand((unsigned) time(&t));
    for(int i = 0; i < Size; i++){
        Array[i] = rand() % Size + 1;
        //printf("%d\n", Array[i]);
    }
    // start measure the time
    struct timeval start_time, end_time;
    long milli_time, seconds, useconds;
    gettimeofday(&start_time, NULL);

    struct MergeSortStruct arg;
    arg.begin = 0;
    arg.end = Size - 1;
    arg.level = 0;
    // master thread create a child thread, so there is only one part
    // then the child thread will create two thread, and each one work on half of the array
    pthread_t p;
    pthread_create(&p, NULL, MergeSort_T, &arg);
    pthread_join(p, NULL);

    gettimeofday(&end_time, NULL);
    seconds = end_time.tv_sec - start_time.tv_sec; //seconds
    useconds = end_time.tv_usec - start_time.tv_usec; //microseconds
    milli_time = ((seconds) * 1000 + useconds/1000.0);
    printf("Elapsed time: %ld milliseconds\n", milli_time);

    for(int i = 0; i < Size; i++){
        //printf("%d\n", Array[i]);
    }
    return 0;
}

// merge means we have a sorted left array index between [left, middle - 1]
// and a sorted right array index between [middle - 1, right]
// we will join this two array togethr on the original array rangiing from [left, right]
// have two cursor on both array, and compare the numbers on the cursor, move down the smaller one
// and move the cursor to the next index
// after one of the cursor reach the end, copy the rest of the other array on to the original array
void Merge(int left, int middle, int right){
    // left array, right arrary
    int * leftArray, * rightArray;
    leftArray = (int *)malloc((middle - left) * sizeof(int));
    rightArray = (int *)malloc((right - middle + 1) * sizeof(int));
    // copy the value from Array to left and right
    for(int i = left; i <= middle - 1; i++){
        leftArray[i - left] = Array[i];
    }
    for(int i = middle; i <= right; i++){
        rightArray[i - middle] = Array[i];
    }
    // destination array: Array[left, right]
    // left cursor, right curosr, and destination cursor
    int l = 0, r = 0, d = left;
    int leftSize = middle - left;
    int rightSize = right - middle + 1;
    // compare the number on the left cursor and right cursor.
    // We move the cursor to next number when it has been copied
    // stop when either array hits end
    while((l < leftSize) && (r < rightSize)){
        if (leftArray[l] <= rightArray[r]){
            Array[d] = leftArray[l];
            l++;
        } else {
            Array[d] = rightArray[r];
            r++;
        }
        d++;
    }
    // if there is any number left without copied to the Array on the left array
    while(l < leftSize){
        Array[d] = leftArray[l];
        d++;
        l++;
    }
    // if there is any number left on the right array
    while(r < rightSize){
        Array[d] = rightArray[r];
        d++;
        r++;
    }
    free(leftArray);
    free(rightArray);
}

void MergeSort(int begin, int end){
    if (begin == end){
        return;
    }
    // begin = 0, end = 9, middle = 4
    int middle = (begin + end) / 2;
    //MergeSort(0, 4)
    MergeSort(begin, middle);
    //MergeSort(5, 9)
    MergeSort(middle + 1, end);
    //0 - 4 is the left sorted array, 5 to 9 is the right sorted array
    //begin, middle is 0 to 4, middle + 1 to end is 5 to 9
    Merge(begin, middle + 1, end);
}

void * MergeSort_T(void * arg){
    struct MergeSortStruct str = * (struct MergeSortStruct *) arg;
    int begin = str.begin;
    int end = str.end;
    int currentLevel = str.level;
    if (begin == end){
        return NULL;
    }
    int middle = (begin + end) / 2;
    if (currentLevel <= MaxLevel){
        pthread_t t1, t2;
        struct MergeSortStruct arg1, arg2;
        arg1.begin = begin;
        arg1.end = middle;
        arg1.level = currentLevel + 1;
        pthread_create(&t1, NULL, MergeSort_T, &arg1);
        arg2.begin = middle + 1;
        arg2.end = end;
        arg2.level = currentLevel + 1;
        pthread_create(&t2, NULL, MergeSort_T, &arg2);
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
    } else {
        //MergeSort(0, 4)
        MergeSort(begin, middle);
        //MergeSort(5, 9)
        MergeSort(middle + 1, end);
    }
    Merge(begin, middle + 1, end);
    return NULL;
}