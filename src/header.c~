#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <sys/types.h>
#include <stdio.h>
#include "types.h"


/*void bin_to_int(char *str){
  int d=0;
  for (d=0; d< sizeof(int); d++){
  char *b= str << d;
  if ((*b & 1) == 1) fprintf(stderr, "1");
  else fprintf(stderr, "1");

  }
  }
*/

const long OFFSET_SIZE = sizeof(ub4)+sizeof(off_t)+sizeof(size_t);

int is_big_endian_machine(void)
{
  unsigned tst = 1;

  int stat = (*(unsigned char*)&tst) != 1;

  return stat;
}


FILE *header_file_open(char *pkg_file){
  FILE *fh;
  // open file with binary/read mode
  if ( (fh = fopen(pkg_file, "rb+")) == NULL){
    perror("fopen");
    return NULL; // todo: ...
  }

  return fh;
}

/*
 * returns a pointer to a new struct of the offset
 * existing according to the hash key passed.
 */
offset_p header_read_offset(FILE *fh, khint_t hash){
  unsigned int i,num_files = header_get_head(fh);

  fseek(fh, sizeof(unsigned int), SEEK_SET);
  char *buff = NULL;
  offset_p offp = malloc( sizeof(header_offset) );

  for (i=0;i<num_files;i++){
    buff = malloc( sizeof(header_offset) );


    fread(buff, sizeof(header_offset), 1, fh);
    memcpy(&offp->hash        , buff                            , sizeof(ub4));
    memcpy(&offp->offset_start, buff+sizeof(ub4)              , sizeof(off_t));
    memcpy(&offp->size        , buff+sizeof(ub4)+sizeof(off_t), sizeof(size_t));
    free(buff);

    if (offp->hash == hash) return offp;
  }

  return NULL;
}

header_offset to_offset(ub4 hash, size_t filesize){
  header_offset of;
  of.hash= hash;
  of.size= filesize;
  return of;
}

void header_write_offset(FILE *fh,offset_p offp, unsigned int index){
  char *buff = malloc(sizeof(header_offset));
  bzero(buff, sizeof(header_offset));

  // jump to end of header
  fseek(fh, sizeof(unsigned int)+index*sizeof(header_offset), SEEK_SET); /* todo: err check */

  //  offp->offset_start= ftell(fh); // set offset index

  // fill up buffer
  memcpy(buff, (void *)&offp->hash, sizeof(ub4));
  memcpy(buff+sizeof(ub4), (void *)&offp->offset_start, sizeof(off_t));
  memcpy(buff+sizeof(ub4)+sizeof(off_t), (void*)&offp->size, sizeof(size_t));

  fwrite(buff, sizeof(header_offset), 1, fh); // write offset

  // free buffer
  free(buff);
}

void small_to_big_endian_int(char **bin, unsigned int size){
  int i;
  char *temp = malloc( sizeof(char) * size);
  for (i=size-1; i >= 0; i--){
    temp[size-i] = (*bin)[i];
  }
  memcpy(bin, &temp, sizeof(char) * size);
  printf(" bla: %p\n", temp);
  free(temp);
}

unsigned int header_init(FILE *fh, unsigned int n_files){
  int header_size;
  int len=fwrite(&n_files, sizeof(unsigned int), 1, fh);
  char buff[header_size];

  fseek(fh, 0, SEEK_SET);

  //write rubbish until end of header.
  header_size=n_files * sizeof(header_offset) + sizeof(unsigned int);

  bzero(buff, header_size);

  fwrite(&buff, header_size, 1, fh);
  return header_size;
}

void print_offset(offset_p off){
  fprintf(stdout, "OFFSET: ---- \n size: %u, offset: %u, hash: %u\n", off->size, off->offset_start, off->hash);
}

unsigned int header_get_head(FILE *fh){
  unsigned int *c = malloc(sizeof(int)), r;

  fseek(fh, 0L, SEEK_SET);

  if (c == NULL){
    perror("malloc");
    return -1;
  }

  int len= fread(c, sizeof(int), 1, fh);

  if (len<1) {
    printf("head not initialized!\n");
    return -1;
  }
  r= *c;
  free(c);
  return r;
}

void header_close(FILE* fh){
  close(fh);
}

offset_p* header_get_offsets(FILE *fh){
  unsigned int n_files=  header_get_head(fh),i,len;
  offset_p *offsets;
  char *buff;
  
  fseek(fh, sizeof(unsigned int), SEEK_SET);

  if ((buff=malloc( sizeof(header_offset)*n_files)) == NULL){
    perror("malloc");
    return NULL;
  }

  if ((len = fread(buff, sizeof(header_offset), n_files, fh)) == -1){
    perror("fread");
    return NULL;
  }

  if ((offsets=malloc(sizeof(header_offset) * n_files)) == NULL){
      perror("malloc");
      return NULL;
    }

  for (i=0; i< n_files; i++){
    memcpy(&offsets[i], &buff, sizeof(header_offset));
    buff+= sizeof(header_offset);
  }
  
  buff-= sizeof(header_offset)*n_files;
  free(buff);

  return offsets;
}




