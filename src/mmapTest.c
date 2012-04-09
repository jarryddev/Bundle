// mmapTest.c
// author: Jarryd Hall
// mmap test for Bundle Project
// mmap usage: caddr_t mmap(caddr_t addr, size_t len, int prof, int flags, int fd, off_t offset)
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

// this returns the size of the pakFile which will be passed to mmap()
size_t getFileSize(FILE *pakFile)
{
	struct stat statBuffer;
	int result = fstat (pakFile, &statBuffer);
	if(result == -1)
	{
		printf("Couldn't open file"\n); 
		return -1; /* add the code to process this return*/
	}
	size_t fileSize = statBuffer.st_size;
	return fileSize;
}

// this maps the pak file and returns the address of the mapped data starting point
caddr_t mapPakFile (size_t fileSize, FILE pakFile)
{
  	// how about skipping to the compressed data ?
  // header_get_head(pakFile);

	// map the data
	caddr_t  mappedAddress = mmap (NULL, fileSize, PROT_READ, MAP_FILE, pakFile, 0);



	if(mappedAddress == (caddr_t) -1)
	{
		printf("Failed to map file to virtual memory"\n);
		return -1; /* add the code to process this return */
	}
	return mappedAddress;
}

