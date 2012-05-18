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

#include "BundleCheckFileType.h"

/////////////////////////////////////////////////////////////
// Function for checking file Extension
// should compress file? return 1 for yes, 0 for no
/////////////////////////////////////////////////////////////
int shouldCompressFileType(char *filePath, char *extensions[], int extCount)
{
	int extLength = 0;	
	while(*filePath != '\0')
	{
		filePath++;
	}
	while(*filePath != '.')
	{
		filePath--;
		extLength++;
	}
	extLength -= 1;
	char *tmpExt = malloc(extLength * sizeof(char));
	char *extension = tmpExt;
	extension = filePath++;
	while(*filePath != '\0')
	{
		extension = filePath++;
		extension++;
	}
	int i;
	for(i = 0; i < extLength; i++)
	{
		extension--;
	}
	
	int shouldCompress = 0;
	
	// print passed in extensions
	int index;
	for(index = 0; index < extCount; index++)
	{
		if(strcmp(extensions[index], extension) == 0)
		{
			// matched the extension, compress
			shouldCompress = 1;
		}
		else
			shouldCompress = 0;
	}
	
	/* NOTE: Use this for hard coding the file extensions to compress
     //Add extension checks here:
     // possibly only files that need to be compressed
     
     // Example ->
     // if(strcmp("txt", extension) == 0)
     // 	{
     // 		printf("txt file\n");	
     // 		shouldCompress = 1;
     // 	}
     
     // add more file types here
     
     else // if file not here, dont compress
     {
     shouldCompress = 0;
     }	
     */
	
	free(tmpExt);
	extension = NULL;
	return shouldCompress;
}

/* usage example */
/*
 int main(void)
 {	
 int shouldCompressBool;
 shouldCompressBool = shouldCompressFileType("test/test.png");
 switch(shouldCompressBool)
 {
 case 0:
 printf("don't compress\n");
 break;
 case 1:
 printf("compress\n");
 break;
 default:
 printf("default hit is switch\n");
 break;
 }
 return 1;
 }
 */