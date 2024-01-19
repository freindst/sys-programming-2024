#include <stdlib.h>
#include <stdio.h>

int main(){
  FILE * fp;
  fp = fopen("file.txt", "w+");
  fprintf(fp, "This is class system programming spring 2024.");
  fclose(fp);
  printf("Done with coding.");
  return(0);
}
