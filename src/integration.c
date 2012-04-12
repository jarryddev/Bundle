#include "header.h"
#include "hash.h"
#include <stdlib.h>

int bundle_start(char *pakFile){
  int ret;

  if ((ret=hash_init(pakFile)) == 1){
    printf("%s Hashed....\n", pakFile);
  } else {
    printf("Filed hashing %s, quitting...\n", pakFile); 
    return -1;
  }
  
  // map file...

  return 1;
}

int bundle_stop(){
  hash_destroy();
}

int main(int argc, char **argv){

  
  char *filename = "test.pak";

  bundle_start(filename);
  
  return 0;
}



