#include <stdio.h>
#include "header.h"
#include <stdlib.h>

int main(void){

  char *filename = "file.pak";
  printf("endianness big?: %d\n", is_big_endian_machine());
  FILE *fh = fopen(filename, "w+b");

  header_init(fh,3);

  //  header_update_head(fh, 12);
  offset_p offp1 = malloc(HEADER_OFFSET_SIZE);
  offp1->hash = 0xFFFFFFFC;
  offp1->size = 0xBBBBBBBBBBBBBBBC;
  offp1->offset_start= 0xCCCCCCCCCCCCCCCC;
  offset_p offp2 = malloc(HEADER_OFFSET_SIZE);
  offp2->hash = 0xAAAAAAAC;
  offp2->size = 0xDDDDDDDDDDDDDDDC;
  offp2->offset_start= 0xEEEEEEEEEEEEEEEE;

  offset_p offp3 = malloc(HEADER_OFFSET_SIZE);
  offp2->hash = 0xAAAAAAfC;
  offp2->size = 0xDDDDDDDDDDDDDDfC;
  offp2->offset_start= 0xEEEEEEEEEfEEEEEE;


  header_write_offset(fh, offp1,0);
  header_write_offset(fh, offp2,1);
  header_write_offset(fh, offp3,2);


  free(offp1); free(offp2); free(offp3);


  unsigned int n_files = 0;
  n_files= header_get_head(fh);
  fprintf(stderr, "Number of files: %d\n", n_files);


  offset_p* offx;
  offx= header_get_offsets(fh);
  int i=0;
  for (i=0; i< n_files; i++){
    print_offset(offx[i]);
  }
  
  free(offx);
  
  header_close(fh);
  //  free(offp);
  }
