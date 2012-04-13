// BundleMemoryMapping.h
// author: Jarryd Hall

#include <stdlib.h>
#include <string.h>

#ifndef Bundle_MemoryMapping_h
#define Bundle_MemoryMapping_h

struct mappedData {
	size_t fileSize;
	void *mappedAddress;
};

int mapPakFile (const char *fileToOpen, long startOffset, struct mappedData *mData);
void unMapPakFile (void *mappedAddress, size_t fileSize);

#endif
