#include <stdio.h>
#include <stdlib.h>

int main(){
  FILE * fp;

  fp = fopen("file.txt", "w+");
  fprintf(fp, "We are in 2024");
  fclose(fp);
  printf("Done with creating \"file.txt\".");

  return(0);
}
