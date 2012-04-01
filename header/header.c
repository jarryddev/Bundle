#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

/*void bin_to_int(char *str){
  int d=0;
  for (d=0; d< sizeof(int); d++){
  char *b= str << d;
  if ((*b & 1) == 1) fprintf(stderr, "1");
  else fprintf(stderr, "1");

  }
  }
*/

const long OFFSET_SIZE = sizeof(guint)+sizeof(off_t)+sizeof(size_t);

int is_big_endian_machine(void)
{
  unsigned tst = 1;

  int stat = (*(unsigned char*)&tst) != 1;

  return stat;
}


FILE *header_file_open(char *pkg_file){
  FILE *fh;
  // open file with binary/read mode
  if ( (fh = fopen(pkg_file, "wb+")) == NULL){
    perror("fopen");
    return NULL; // todo: ...
  }

  return fh;
}

/*
 * returns a pointer to a new struct of the offset
 * existing according to the hash key passed.
 */
offset_p header_read_offset(FILE *fh, guint hash){
  unsigned int i,num_files = header_get_head(fh);
  //  fseek(fh, 0, SEEK_SET);
  fseek(fh, sizeof(unsigned int), SEEK_SET);
  char *buff = NULL;
  offset_p offp = malloc( sizeof(offset) );
  for (i=0;i<num_files;i++){
    buff = malloc( OFFSET_SIZE );

    fread(buff, OFFSET_SIZE, 1, fh);
    memcpy(&offp->hash        , buff                            , sizeof(guint));
    memcpy(&offp->offset_start, buff+sizeof(guint)              , sizeof(off_t));
    memcpy(&offp->size        , buff+sizeof(guint)+sizeof(off_t), sizeof(size_t));
    free(buff);


    if (offp->hash == hash) return offp;
  }

  return NULL;
}

offset to_offset(guint hash, size_t filesize){
  offset of;
  of.hash= hash;
  of.size= filesize;
  return of;
}

//void header_write_file(FILE *fh, guint hash, size_t filesize){
//  offset p = to_offset(hash, filesize);
//  header_write_offset(fh, off);
//}

void header_write_offset(FILE *fh,offset_p offp, unsigned int index){
  char *buff = malloc(OFFSET_SIZE);
  bzero(buff, OFFSET_SIZE);

  // jump to end of header

int a=  fseek(fh, sizeof(unsigned int)+index*sizeof(offset), SEEK_SET); /* todo: err check */

 printf("index: %d, offset: %d fseek ret: %d\n", index, sizeof(unsigned int)+index*sizeof(offset), a);
  //  offp->offset_start= ftell(fh); // set offset index

  // fill up buffer
  memcpy(buff, (void *)&offp->hash, sizeof(guint));
  memcpy(buff+sizeof(guint), (void *)&offp->offset_start, sizeof(off_t));
  memcpy(buff+sizeof(guint)+sizeof(off_t), (void*)&offp->size, sizeof(size_t));

  fwrite(buff, sizeof(buff), 1, fh);
  //write(fh, buff, OFFSET_SIZE);

  //  header_update_head(fh, files_num+1);

  free(buff);
}

void small_to_big_endian_int(char **bin, unsigned int size){
  int i;
  char *temp = malloc( sizeof(char) * size);
  for (i=size-1; i >= 0; i--){
    printf("i: %d val: %x\n", i, (*bin)[i]);
    temp[size-i] = (*bin)[i];
  }
  memcpy(bin, &temp, sizeof(char) * size);
  printf(" bla: %p\n", temp);
  free(temp);
}

unsigned int header_init(FILE *fh, unsigned int n_files){
  fseek(fh, 0, SEEK_SET);
  int header_size;
  printf("ftell reutnred %ld\n", ftell(fh));
  printf("updaing %d\n", n_files);
  int len=fwrite(&n_files, sizeof(unsigned int), 1, fh);
  printf("fwrite reutnred %ld\n", len);

  //write rubbish until end of header.
  header_size=n_files * sizeof(offset) + sizeof(unsigned int);
  char buff[header_size];
  bzero(buff, header_size);
  int a=0;
  fwrite(&buff, header_size, 1, fh);
  printf("HEADER SIZE: %d offset size %d\n", header_size, sizeof(offset)); 
  return header_size;

}


unsigned int header_get_head(FILE *fh){
  unsigned int *c = malloc(sizeof(int)), r;

  fseek(fh, 0L, SEEK_SET);

  if (c == NULL){
    perror("malloc");
    return;
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
