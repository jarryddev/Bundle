#include "BundleCheckFileType.h"

/////////////////////////////////////////////////////////////
// Function for checking file Extension
// should compress file? return 1 for yes, 0 for no
/////////////////////////////////////////////////////////////
int shouldCompressFileType(char *filePath)
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
	printf("Extension length is %d\n", extLength);
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
	printf("Extension is: %s\n", extension);
	
	int shouldCompress = 0;
	
	//Add extension checks here:
	// possibly only files that need to be compressed
	if(strcmp("png", extension) == 0)
	{
		printf("png file\n");	
		shouldCompress = 1;
	}
	else if(strcmp("jpeg", extension) == 0)
	{
		printf("jpeg file\n");
		shouldCompress = 1;
	}
	else if(strcmp("ogg", extension) == 0)
	{
		printf("ogg file\n");
		shouldCompress = 1;
	}
	else if(strcmp("wav", extension) == 0)
	{
		printf("wav file\n");
		shouldCompress = 1;
	}     
	/* add more file types here */
	else /* if file not here, dont compress */
	{
		shouldCompress = 0;
	}	
	
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