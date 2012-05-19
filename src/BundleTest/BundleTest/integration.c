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

    printf("%s\n", pakFile);
    
  // hash file
    if ((ret=hash_init(pakFile)) != 1){
    printf("Failed hashing %s, quitting...\n", pakFile);
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

offset_p bundle_getIndexDataFor(char *fileName, long int mmap_address)
{
  offset_p offs;

  //  get fileName offset and print it
  if ((offs = get_offset(fileName)) == NULL) return NULL;
  offs->offset_start+=mmap_address;
  return offs;
}

int bundle_stop(struct mappedData *mData){
  hash_destroy();
  return(unMapPakFile (mData->mappedAddress, mData->fileSize));
}

// This will be done within an objective-C file
//int main(int argc, char **argv){
//
//  /* Data needed by the dev to use Bundle
//   *     char * with the pak filename
//   *  struct mappedData which holds mappedAddress and fileSize
//   *  both are used for the bundle_stop function when munmapping
//   */
//
//  if (argc != 3){
//    printf("Usage %s [pak file] [file to locate]\n", argv[0]);
//    exit(1);
//  }
//
//  char *filename = argv[1];
//  char *file_to_locate= argv[2];
//  struct mappedData *mData;
//  mData = malloc(sizeof(struct mappedData)); /*free when done */
//  offset_p offs;
//  long int offset;
//
//  bundle_start(filename, mData);
//  
//  if ((offs=bundle_getIndexDataFor(file_to_locate, mData->mappedAddress)) == NULL){
//    printf("%s not found\n", file_to_locate);
//  }
//  
//  printf("%s Found:\n", file_to_locate);
//  print_offset(offs);
//
//  free(mData);
//  free(offs);
//
//  mData=offs=NULL;
//  /* Run Game Code From here */
//  
//  /* When done stop Bundle
//  //       bundle_stop(mData);
//  */
//
//  return 0;
//}



