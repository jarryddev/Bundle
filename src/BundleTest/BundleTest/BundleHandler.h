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