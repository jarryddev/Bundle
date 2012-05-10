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