#include <stdlib.h>
#include "header.h"
#include "hash.h"
#include "BundleMemoryMapping.h"

/*
 *      compile using:
 *      gcc integration.c -o ../bin/integration -L../lib -lheader -lhash
 */

int bundle_start(char *pakFile, struct mappedData *mData){
  int ret;

  // hash file
  if ((ret=hash_init(pakFile)) != 1){
    printf("Filed hashing %s, quitting...\n", pakFile);
    return -1;
  }

  // map file...
  if(mapPakFile(pakFile, 0, mData) != 0)
    {
      printf("Failed to map file %s", pakFile);
      return -1;
    }
  return 1;

}

long int bundle_getIndexDataFor(char *fileName)
{
  offset_p offs;
  long int offset;
  
  //  get test.jpg offset and print it
  if ((offs = get_offset(fileName)) == NULL) return -1;

  //  bundle_getIndexDataFor(fileName);
  print_offset(offs);
  offset= offs->offset_start;
  free(offs);
  
  return offset;
}

int bundle_stop(struct mappedData *mData){
  hash_destroy();
  unMapPakFile (mData->mappedAddress, mData->fileSize);
}

// This will be done within an objective-C file
int main(int argc, char **argv){

  /* Data needed by the dev to use Bundle
   *     char * with the pak filename
   *  struct mappedData which holds mappedAddress and fileSize
   *  both are used for the bundle_stop function when munmapping
   */

  if (argc != 3){
    printf("Usage %s [pak file] [file to locate]\n", argv[0]);
    exit(1);
  }

  char *filename = argv[1];
  char *file_to_locate= argv[2];
  struct mappedData *mData;
  mData = malloc(sizeof(struct mappedData)); /*free when done */
  offset_p offs;
  long int offset;

  bundle_start(filename, mData);
  
  if ((offset=bundle_getIndexDataFor(file_to_locate)) == -1){
    printf("%s not found\n", file_to_locate);
  }
  
  printf("%s Found:\n\tOffset: %ld\n", file_to_locate, offset);

  free(mData);
  
  /* Run Game Code From here */
  
  /* When done stop Bundle
  //       bundle_stop(mData);
  */

  return 0;
}



