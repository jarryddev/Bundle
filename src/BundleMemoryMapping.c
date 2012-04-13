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
//		 (e.g. a file in read mode cant be mapped and set to PROT_WRITE(write mode)).
//		 pass a bitwise OR of one or more of the following:
//       PROT_EXEC: Code can execute on mapped data, does not work on iOS.
//       PROT_READ: Memory is readable.
//		 PROT_WRITE: Memory is writable.
// flags: The following flags are relevant for Bundle and can be used:
//		 MAP_FILE: Map from a file. This is standard and doesnt need to be passed.
//		 MAP_FIXED: Returned address must equal passed in address.
//		 MAP_INHERIT: Allows mapped data to be passed amoung child processes.
//		 MAP_PRIVATE: A copy is made of the stored bytes and the copy is referenced.
// 		 MAP_SHARED: Bytes stored to memory will modify mapped file.
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
int mapPakFile (const char *fileToOpen, long startOffset, struct mappedData *mData){
	unsigned char *temp;
	unsigned int store;
	size_t i;
	
	FILE *file = fopen(fileToOpen, "rb");
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
		return NULL;
	}
	printf("Successfully mapped file to virtual memory.\n");
	printf("Starting Mapped Address is: %X \n", (char *) mData->mappedAddress);
		
	// use this for optimizing the kernel for our intended use of the mapped data
	int ret;
	// tell kernel how we use data from addr -> addr + len
	// MADV_RANDOM  disables readahead, reads minimum data on each read
	ret = madvise (mappedAddress, m-Data->fileSize, MADV_RANDOM);
	if (ret < 0)
		perror ("madvise");
	
	fclose(file);
	//printf("Reading %lu bytes.\n", (unsigned long)fileSize);
	return 0;
}

void unMapPakFile (void *mappedAddress, size_t fileSize){
	if(munmap(mappedAddress, fileSize) != 0)
	{
		printf("Failed to unmap file.\n");
	}
	else
	{
		printf("Successfully unmapped file with size %lu.\n", (unsigned long)fileSize);
	}
}

unsigned char * getDataForOffsets(void *mappedAddress, size_t startOffset, size_t numberOfBtyes){
	unsigned char *temp;
	temp = (unsigned char*) mappedAddress + startOffset;
	
	printf("Reading %lu bytes.\n", (unsigned long)numberOfBtyes);

	FILE *sampleFile;
	sampleFile = fopen("Sample.png", "wb");
	
	size_t i;
	for(i = startOffset; i < (startOffset + numberOfBtyes)/2; i++)
	{
		fwrite(temp, 1, 1, sampleFile);
		//printf("Address [%02lu] : %X = %02X \n", (unsigned long)i, temp, *(temp));
		temp++;
	}
	fclose(sampleFile);
	return temp;
}

int main (){
	
	FILE *f;
	int tempSize = 0;
	if (f = fopen("test.jpg", "rb"));
	{
	   	fseek(f, 0, SEEK_END);
	   	size = ftell(f);
		tempSize = size;
		printf("File size of file to map is %lu\n", (unsigned long) size);
		printf("Temp File size of file to map is %lu\n", (unsigned long) tempSize);
	   	fclose(f);
	}
	
	char *mappedFile = mapPakFile("test.jpg", 0);
	
	
		//sleep(5);
		printf("Size to be passed to getDataForOffsets is %lu\n", (unsigned long) tempSize);
		printf("Test is %lu\n", (unsigned long) (size));
		unsigned char *fetchedData = getDataForOffsets(mappedFile, 0, size);
		unMapPakFile(mappedFile, fileSize);
	}
	
	return 0;
}
