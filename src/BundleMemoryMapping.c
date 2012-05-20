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

// BundleMemoryMapping.c
// author: Jarryd Hall
// mmap test for Bundle Project
// mmap usage: void * mmap(void* addr, size_t len, int prof, int flags, int fd, off_t offset)
// the mmap function returns the address to the map data, -1 is returned and errno set if it fails
//
// the arguments are as follows
// addr: Usually pass zero, if a non-zero number is passed, could be used as starting address
//       for mapped file. mmap might ignore this address.
// len: Number of bytes to map from the file. For large files, map smaller amount for speed.
// prot: Protection on the mapped memory, must match current file state.
//               (e.g. a file in read mode cant be mapped and set to PROT_WRITE(write mode)).
//               pass a bitwise OR of one or more of the following:
//       PROT_EXEC: Code can execute on mapped data, does not work on iOS.
//       PROT_READ: Memory is readable.
//               PROT_WRITE: Memory is writable.
// flags: The following flags are relevant for Bundle and can be used:
//               MAP_FILE: Map from a file. This is standard and doesnt need to be passed.
//               MAP_FIXED: Returned address must equal passed in address.
//               MAP_INHERIT: Allows mapped data to be passed amoung child processes.
//               MAP_PRIVATE: A copy is made of the stored bytes and the copy is referenced.
//               MAP_SHARED: Bytes stored to memory will modify mapped file.
// NOTE: The flags parameter must specify either MAP_PRIVATE or MAP_SHARED.
// fd: File descriptor for the open file.
// offset: Starting offset to start mapping from the file. Pass 0 to map from start of file.
// Pass in multiples of memory page size if not 0. NOTE: Use sysconf() to determine size
// The file needs to be unmapped using int munmap (caddr_t addr, size_t len);
// munmap returns zero on success, returns -1 and sets errno if error is encounterded.
// mapped memory is unmapped when program exits.
// When unmapping make sure to pass in multiples of the systems memory page size.
// NOTE: Use sysconf() to determine the memory page size on the target system
// NOTE: int msync(void *addr, size_t len, int flags) is ignored, no writing to memory is done

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "BundleMemoryMapping.h"

size_t size;

// this maps the pak file
int mapPakFile(const char *fileToOpen, long startOffset, struct mappedData *mData){
    
  	FILE *file = fopen(fileToOpen, "rb");
    if(file != NULL)
    {
        fseek(file, 0, SEEK_END);
	  	mData->fileSize = ftell(file);
	  	fseek(file, 0, SEEK_SET);
	  	printf("File size is %lu bytes.\n", (unsigned long) mData->fileSize);
        
	  	int fileDescriptor = fileno(file);
	  	off_t offset = startOffset;
        
	  	// map the data
	  	mData->mappedAddress = mmap(NULL, mData->fileSize, PROT_READ, MAP_PRIVATE, fileDescriptor, offset);
	  	if(mData->mappedAddress == (void *) -1)
        {
	      	printf("Failed to map file to virtual memory.\n");
	      	fclose(file);
	      	return 1;
        }
	  	printf("Successfully mapped file to virtual memory.\n");
	  	printf("Starting Mapped Address is: %X \n", (char *) mData->mappedAddress);
        
	  	// use this for optimizing the kernel for our intended use of the mapped data
	  	int ret;
	  	// tell kernel how we use data from addr -> addr + len
	  	// MADV_RANDOM  disables readahead, reads minimum data on each read
	  	ret = madvise (mData->mappedAddress, mData->fileSize, MADV_RANDOM);
	  	if (ret < 0)
	    	perror ("madvise");
        
	  	fclose(file);
	  	//printf("Reading %lu bytes.\n", (unsigned long)fileSize);
	  	return 0;
    }
    // file couldnt be opened
    return 1;
  	
}

int unMapPakFile (void *mappedAddress, size_t fileSize){
    if(munmap(mappedAddress, fileSize) != 0)
    {
        printf("Failed to unmap file.\n");
        return -1;
    }
    else
    {
        printf("Successfully unmapped file with size %lu.\n", (unsigned long)fileSize);
        return 0;
    }
}

unsigned char * getDataForOffsets(void *mappedAddress, size_t startOffset, size_t numberOfBtyes){
    unsigned char *temp;
    temp = (unsigned char*)  (mappedAddress+startOffset);
    //    temp = (unsigned char*) 200000 + startOffset;
    printf("Reading %lu bytes.\n", (unsigned long)numberOfBtyes);
    
    //	* EXAMPLE USAGE:
    FILE *sampleFile;
    sampleFile = fopen("Sample.png", "wb");
    printf("number of bytes: %ld, strt offset: %lu\nvmap address: %lu\n", numberOfBtyes, startOffset,mappedAddress);
    size_t i=0;
    for(i = 0; i <  numberOfBtyes; i++)
    {
        fwrite(temp, 1, 1, sampleFile);
	printf("Address [%02lu] : %X = %02X \n", (unsigned long)i, temp, *(temp));
        temp++;
    }
    if (i){
      printf("Wrote to Sample.png\n");
    }
    fclose(sampleFile);
    
    return temp;
}

/*
 int main (){
 
 FILE *f;
 int tempSize = 0;
 if (f = fopen("test.jpg", "rb"))
 {
 fseek(f, 0, SEEK_END);
 size = ftell(f);
 tempSize = size;
 printf("File size of file to map is %lu\n", (unsigned long) size);
 printf("Temp File size of file to map is %lu\n", (unsigned long) tempSize);
 fclose(f);
 }
 
 struct mappedData m;
 
 char *mappedFile = mapPakFile("test.jpg", 0, &m);
 
 //sleep(5);
 printf("Size to be passed to getDataForOffsets is %lu\n", (unsigned long) tempSize);
 printf("Test is %lu\n", (unsigned long) (size));
 unsigned char *fetchedData = getDataForOffsets(mappedFile, 0, size);
 unMapPakFile(mappedFile, size);
 
 return 0;
 }
 */
