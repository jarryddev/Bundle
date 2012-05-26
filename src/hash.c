#include "hash.h"
#include "header.h"
#include <string.h>
#include <stdio.h>

HASHTBL *bundle_hash_init(const char *filename){

  offset_p* offsets;
  unsigned int num_files, i;
  FILE *fh;
  header_offset toff;
  hash_size hash;
  HASHTBL *hashtbl;

  //  printf("hash size: %ld\n", sizeof(hash));

  if ((fh=fopen(filename, "r+b")) == NULL){
    perror("fopen");
    return -1;
  }

  num_files = header_get_head(fh);

  // get offsets
  if ((offsets = header_get_offsets(fh))==NULL){
    fprintf(stderr, "Cannot get offsets ...\n");
    fclose(fh);
    return NULL;
  }

  // ceating hash table
  if(!(hashtbl=hashtbl_create(num_files, NULL))) {
    fprintf(stderr, "ERROR: hashtbl_create() failed\n");
    return NULL;
  }

  for (i=0; i< num_files;i++){
    hash = offsets[i]->hash;//offsets[i]->hash;
    hashtbl_insert(hashtbl, hash, offsets[i]);
  }

  fclose(fh);
  return hashtbl;

}


/*int main(void){
  hash_size hash = 0x30162a8; //def_hashfunc("1.jpg");
  if (bundle_hash_init("test.pak") == -1){
    printf("error!");
    return 0;
  }
  
  offset_p offp= (offset_p)hashtbl_get(hashtbl, hash);
  
  printf("FOUND: !!\n");
  print_offset(offp);
  

  hashtbl_destroy(hashtbl);
  return 0;
  }*/


/*
  int main(void){

  HASHTBL *hashtbl;
  char *spain, *italy;

  if(!(hashtbl=hashtbl_create(6, NULL))) {
  fprintf(stderr, "ERROR: hashtbl_create() failed\n");
  exit(EXIT_FAILURE);
  }

  char *s= malloc(10);
  strcpy(s, "Paris");
  hashtbl_insert(hashtbl, def_hashfunc("France"), s);
  hashtbl_insert(hashtbl, def_hashfunc("England"), s);
  hashtbl_insert(hashtbl, def_hashfunc("Italy"), s);//"Rome");
  char *s2= malloc(10);
  strcpy(s2, "Stockholm");
  hashtbl_insert(hashtbl, def_hashfunc("Sweden"), s2);
  hashtbl_insert(hashtbl, def_hashfunc("Germany"), s);
  hashtbl_insert(hashtbl, def_hashfunc("Norway"), s);//"Oslo");

  printf("After insert:\n");
  italy=hashtbl_get(hashtbl, def_hashfunc("Italy"));
  printf("Italy: %s\n", italy?italy:"-");
  spain=hashtbl_get(hashtbl, def_hashfunc("Sweden"));
  printf("sweden: %s\n", spain?spain:"-");

  hashtbl_destroy(hashtbl);
  free(s); free(s2);
  return 0;
  }
*/
hash_size def_hashfunc(const char *key)
{
  hash_size hash=0;
  while(*key) hash+=(unsigned char)*key++;
  return hash;
}

static char *mystrdup(const char *s)
{
  char *b;
  if(!(b=malloc(strlen(s)+1))) return NULL;
  strcpy(b, s);
  return b;
}

HASHTBL *hashtbl_create(hash_size size, hash_size (*hashfunc)(const char *))
{
  HASHTBL *hashtbl;

  if(!(hashtbl=malloc(sizeof(HASHTBL)))) return NULL;

  if(!(hashtbl->nodes=calloc(size, sizeof(struct hashnode_s*)))) {
    free(hashtbl);
    return NULL;
  }

  hashtbl->size=size;

  if(hashfunc) hashtbl->hashfunc=hashfunc;
  else hashtbl->hashfunc=def_hashfunc;

  return hashtbl;
}


int hashtbl_insert(HASHTBL *hashtbl, hash_size key, void *data)
{
  struct hashnode_s *node;
  hash_size hash= key%hashtbl->size;

  /*fprintf(stderr, "hashtbl_insert() key=%s, hash=%d, data=%s\n", key, hash, (char*)data);*/

  node=hashtbl->nodes[hash];
  while(node) {
    if(node->key ==  key) {
      node->data=data;
      return 0;
    }
    node=node->next;
  }
  if(!(node=malloc(sizeof(struct hashnode_s)))) return -1;

  node->key=key;

  //  if (!(node->data= malloc(sizeof(header_offset)))) return -1;
      
  //  memcpy(&(*node).data, data, sizeof(header_offset));
  node->data= data;
  node->next=hashtbl->nodes[hash];
  hashtbl->nodes[hash]=node;

  return 0;
}

void *hashtbl_get(HASHTBL *hashtbl, hash_size key)
{
  struct hashnode_s *node;
  hash_size hash=key%hashtbl->size;

  /*fprintf(stderr, "hashtbl_get() key=%s, hash=%d\n", key, hash);*/

  node=hashtbl->nodes[hash];
  if (node) return node->data;
  //   while(node) {
  //     printf("key : %p request: %p\n", node->key, key);
  //     if(node->key == key) return node->data;
  //     node=node->next;
  //    }

  return NULL;
}

void hashtbl_destroy(HASHTBL *hashtbl)
{
  hash_size n;
  struct hashnode_s *node, *oldnode;

  for(n=0; n<hashtbl->size; ++n) {
    node=hashtbl->nodes[n];
    while(node) {
      oldnode=node;
      node=node->next;
      free(oldnode);
      //      free(node->data); // todo: might have to free data

    }
  }
  free(hashtbl->nodes);
  free(hashtbl);
}



