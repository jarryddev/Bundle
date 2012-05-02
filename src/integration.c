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

offset_p bundle_getIndexDataFor(char *fileName)
{
	printf("Returned offset is %lu\n", get_offset(fileName));
  return get_offset(fileName);
}

int bundle_stop(struct mappedData *mData){
  hash_destroy();
  unMapPakFile (mData->mappedAddress, mData->fileSize);
}

// This will be done within an objective-C file
int main(int argc, char **argv){

	/* Data needed by the dev to use Bundle
	*	 char * with the pak filename
	*  struct mappedData which holds mappedAddress and fileSize
	*  both are used for the bundle_stop function when munmapping
	*/
	
  char *filename = "test.pak";
  struct mappedData *mData;
	mData = malloc(sizeof(struct mappedData)); /*free when done */
	offset_p offs;

  bundle_start(filename, mData);

  //  hash_read();

  //  get test.jpg offset and print it
  if ((offs = get_offset("test.jpg")) == NULL){
    printf("Error: Cannot locate test.jpg\n");
    free(mData);
    return 1;
  }; 

	bundle_getIndexDataFor("test.jpg");

  printf("Key found\n");
  print_offset(offs);
  free(offs);
  
  /* Run Game Code From here */

  /* When done stop Bundle
//       bundle_stop(mData);
   */

  return 0;
}



