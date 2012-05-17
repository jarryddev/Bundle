#include <stdio.h>
#include <sys/types.h>
#include "header.h"

int main(void){
  int *buff= malloc(100);
  int i=0;
  for (i=0;i < 100; i++){
    printf("%p - ", &buff[i]); 
    buff[i]= i;
  }
  printf("&buff = %p\n", (int*)&buff[0]);

  free(buff);
}
