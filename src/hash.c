#include "khash.h"
#include "header.h"

// initialize the hashmap type
KHASH_MAP_INIT_INT(32, void*);

khash_t(32) *header_hash;

int hash_init(char *filename){

  offset_p* offsets;
  unsigned int num_files, i;
  int ret;
  FILE *fh;
  khiter_t k;
  khint_t t;

  header_hash = kh_init(32);

  if ((fh=fopen(filename, "r+b")) == NULL){
    perror("fopen");
    return -1;
  }

  num_files = header_get_head(fh);

  if ((offsets = header_get_offsets(fh))==NULL){
    fprintf(stderr, "Cannot get offsets ...\n");
    return -1;
  }


  for (i=0; i< num_files;i++){
    t= offsets[i]->hash;
    //    printf("hash: %ld\n", t);
    k=kh_put(32, header_hash, t, &ret);
    //    kh_value(32, k) = offsets[i];
    ((header_hash)->vals[k]) = offsets[i];
    //print_offset(offsets[i]);
  }

  free(offsets);
  return 1;//header_hash;
}

hash_read(char *filename){
  
}

int main(void){
  
}
