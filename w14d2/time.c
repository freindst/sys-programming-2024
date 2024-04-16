#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

int main() {
   struct timeval start_time, end_time;
   long milli_time, seconds, useconds;
   gettimeofday(&start_time, NULL);
   int a;
   for(int i = 0; i < 100000; i++){
      a = i;
      printf("%d\n", a);
   }
   gettimeofday(&end_time, NULL);
   seconds = end_time.tv_sec - start_time.tv_sec; //seconds
   useconds = end_time.tv_usec - start_time.tv_usec; //microseconds
   milli_time = ((seconds) * 1000 + useconds/1000.0);
   printf("Elapsed time: %ld milliseconds\n", milli_time);
   return 0;
}