#include "khash.h"
#include "header.h"
#include "hash.h"

// initialize the hashmap type
KHASH_MAP_INIT_INT(32, void*);

// GLOBAL HASH TABLE
khash_t(32) *header_hash;


/*int main(int argc, char **argv){
  if (argc != 3){
    printf("Usage: %s pakfile filename\n", argv[0]);
    exit(1);
  }

  hash_init(argv[1]);
  //  hash_read();
  offset_p p= get_offset(argv[2]);
  print_offset(p);
  kh_destroy(32, header_hash);
  }*/


/**
 * Initialize a hashtable, passing a pak filename to extract the header offsets from
 * and create a hashmap of [hash of filename]=>[offset].
 *
 */
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
    fclose(fh);
    return -1;
  }

  for (i=0; i< num_files;i++){
    t= offsets[i]->hash;
    //    printf("Pushing offset: \n");

    k=kh_put(32, header_hash, t, &ret); // creating key
    ((header_hash)->vals[k]) = offsets[i];  // setting value  todo: offsets[i]->hash is saved incorrectly.
    //    ((header_hash)->vals[k]) = malloc(sizeof(header_offset)); // offsets[i];  // setting value
    //    memcpy((&(header_hash)->vals[k]), &offsets[i], sizeof(header_offset));
    //    print_offset((header_hash)->vals[k]);
  }

  free(offsets);
  fclose(fh);

  printf(" --- Hash created --- \n");
  printf(" buckets: %d, items: %d \n\n", kh_n_buckets(header_hash), kh_size(header_hash));

  return 1;
}

/**
 * Returns the offset pointer corresponding to filename as
 * the key, or NULL of key does not exist
 *
 */
offset_p get_offset(char *filename){

  khiter_t k; // key

  if (header_hash == NULL){
    printf("hash map not intiialized ...\n");
    exit(1);
  }

  if (!kh_size(header_hash)) return NULL;

  k=kh_get(32, header_hash, __ac_X31_hash_string(filename)); // get key

  if (k == kh_end(header_hash)) return NULL; // key does not exist

  return kh_value(header_hash, k);
}

/**
 * Output the elements in the hash table.
 */
void hash_read(){

  khiter_t iter;

  if (header_hash == NULL){
    printf("hash map not intiialized ...\n");
    exit(1);
  }

  for (iter = kh_begin(header_hash); iter != kh_end(header_hash); iter++){
    if (kh_exist(header_hash, iter)) {

      printf("Hashmap key: %p\n", kh_key(header_hash, iter));
      printf("Hashmap value: \n");
      //      kh_get(32, header_hash,
      print_offset(kh_value(header_hash, iter));
      printf("---------------\n");
    }
  }
}


