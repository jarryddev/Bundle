#include <stdio.h>
#include "header.h"
#include <stdlib.h>

int main(void){

  char *filename = "file.pak";
  printf("endianness big?: %d\n", is_big_endian_machine());
  FILE *fh = fopen(filename, "w+b");

  header_init(fh,3);

  //  header_update_head(fh, 12);
  offset_p offp1 = malloc(sizeof(header_offset));
  offp1->hash = 0xFFFFFFFC;
  offp1->size = 0xBBBBBBBBBBBBBBBC;
  //  offp1->offset_start= 0xCCCCCCCCCCCCCCCC;
  offset_p offp2 = malloc(sizeof(header_offset));
  offp2->hash = 0xAAAAAAAA;
  offp2->size = 0xDDDDDDDDDDDDDDDD;
  offp2->offset_start= 0xEEEEEEEEEEEEEEEE;

  offset_p offp3 = malloc(sizeof(header_offset));
  offp3->hash = 0x23232;
  offp3->size = 0xDDAADD;

  header_write_offset(fh, offp1,0);
  header_write_offset(fh, offp2,1);
  header_write_offset(fh, offp3,2);


  free(offp1); free(offp2); free(offp3);

  unsigned int n_files = 0;
  n_files= header_get_head(fh);
  fprintf(stderr, "Number of files: %d\n", n_files);


  //  offset_p offp = header_read_offset(fh, 0xAAAAAAAA);
  //  print_offset(offp);

  offset_p* offx;
  offx= header_get_offsets(fh);
  
  free(offx);
  
  header_close(fh);
  //  free(offp);
  }
