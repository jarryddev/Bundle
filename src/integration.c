#include <stdlib.h>
#include "header.h"
#include "hash.h"
#include "BundleMemoryMapping.h"

/* 	
*	compile using:
* 	gcc integration.c -o ../bin/integration -L../lib -lheader -lhash
*/

int bundle_start(char *pakFile, struct mappedData *mData){
  	int ret;

	// hash file
 	if ((ret=hash_init(pakFile)) == 1){
    printf("%s Hashed....\n", pakFile);
  	} else {
    	printf("Filed hashing %s, quitting...\n", pakFile); 
    	return -1;
  	}
  
  	// map file...
	char *mappedFile = mapPakFile(pakFile, 0, mData);
	if(mappedFile == NULL)
	{
		printf("Failed to map file %s", pakFile);
		return -1;
	}
  	return 1;
}

void bundle_getIndexDataFor(char *fileName)
{
	// TO DO:
	// return the index data
	// offset and size
	
}

int bundle_stop(struct mappedData *mData){
  	hash_destroy();
	unMapPakFile (mData->mappedAdress, mData->fileSize);
}

// This will be done within an objective-C file
int main(int argc, char **argv){

  	char *filename = "test.pak";
	struct mappedData *mData;
	
  	bundle_start(filename, mData);

	/* Run Game Code From here */
	
	/* When done stop Bundle 
	*	bundle_stop(mData);
  	*/

  	return 0;
}



