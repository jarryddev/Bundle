/*

Copyright (c) <2012>, <Jarryd Hall, Taher Odeh>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met: 

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies, 
either expressed or implied, of the FreeBSD Project.

*/

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
