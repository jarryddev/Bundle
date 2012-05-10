//
//	BundleHandler.h
//  Bundle
//
//  Created by Jarryd Hall on 2/7/12.
//

#include <stdio.h>

#ifndef Bundle_FileHandler_h
#define Bundle_FileHandler_h

/////////////////////////////////////////////////////////////
// Count the number of files for header length calculation
/////////////////////////////////////////////////////////////
static int countFiles(char **source);
/////////////////////////////////////////////////////////////
// Demonstration of zlib utility functions
/////////////////////////////////////////////////////////////
unsigned long file_size(char *filename);
int decompress_one_file(char *infilename, char *outfilename);
int compress_one_file(char *infilename, char *outfilename);
/////////////////////////////////////////////////////////////
// Functions for writing asset data to pak file
/////////////////////////////////////////////////////////////
void printData(FILE *someFile, char *text);
static int packageSourceFolder(char **source, char *desintation, char *extenstions[], int extCount);

#endif