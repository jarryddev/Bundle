#include "profiler.h"

long long doSum(long long  a){
  if (a<=1) return 1L;
  return a+doSum(a-1);
}

/**
 * Usage example 
 *
 */
/*int main(){
  
  if (profiler_start()){
    int sum=doSum(100000L);
    printf("Call: doSum(), time: %lu\n", profiler_getTime());
  }
  }*/


int profiler_start(){
  return (gettimeofday(&tv,NULL)==0);
}

void profiler_printTime(char *function_name){
  printf("Call of %s took : %lu milliseconds\n", function_name, profiler_getTime());
}

suseconds_t profiler_getTime(){
  suseconds_t usec= tv.tv_usec;
  gettimeofday(&tv,NULL);
  return tv.tv_usec-usec;
}

