//
//	BundleHandler
//  Bundle
//
//  Created by Jarryd Hall on 2/7/12.
//

#include <sys/types.h>
#include <sys/stat.h>
#include <err.h>
#include <fts.h>
#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include "zlib.h"
// #include "BundleHandler.h"

int fileCountForHeader;

static int countFiles(char *const argv[])
{
    FTS *ftsp;
	FTSENT *p, *chp;
	int fts_options = FTS_COMFOLLOW | FTS_LOGICAL | FTS_NOCHDIR;
    
//	char *const path[] = {(argv + 1), NULL};
	
    if ((ftsp = fts_open(argv, fts_options, NULL)) == NULL) {
		warn("fts_open");
		return -1;
	}
    
    /* Initialize ftsp with as many argv[] parts as possible. */
	chp = fts_children(ftsp, 0);
	if (chp == NULL) {
		return 0;               /* no files to traverse */
	}
    
    while ((p = fts_read(ftsp)) != NULL) {
		switch (p->fts_info) {
			case FTS_F:
                //increment the file count
                fileCountForHeader++;
				break;
			default:
				break;
        }
    }

    printf("file count is %d\n*********\n", fileCountForHeader);
    fts_close(ftsp);
    return 0;    
}
/////////////////////////////////////////////////////////////
// Demonstration of zlib utility functions
/////////////////////////////////////////////////////////////
unsigned long file_size(char *filename)
{
    FILE *pFile = fopen(filename, "rb");
    fseek (pFile, 0, SEEK_END);
    unsigned long size = ftell(pFile);
    fclose (pFile);
    return size;
}

int decompress_one_file(char *infilename, char *outfilename)
{
    gzFile infile = gzopen(infilename, "rb");
    FILE *outfile = fopen(outfilename, "wb");
    if (!infile || !outfile) return -1;
    
    char buffer[128];
    int num_read = 0;
    while ((num_read = gzread(infile, buffer, sizeof(buffer))) > 0) 
	{
        fwrite(buffer, 1, num_read, outfile);
    }

    gzclose(infile);
    fclose(outfile);
}

int compress_one_file(char *infilename, char *outfilename)
{
    FILE *infile = fopen(infilename, "rb");
    gzFile outfile = gzopen(outfilename, "wb");
    if (!infile || !outfile) return -1;

    char inbuffer[128];
    int num_read = 0;
    unsigned long total_read = 0, total_wrote = 0;
    while ((num_read = fread(inbuffer, 1, sizeof(inbuffer), infile)) > 0) {
            total_read += num_read;
             gzwrite(outfile, inbuffer, num_read);
        }
    fclose(infile);
    gzclose(outfile);

    printf("Read %ld bytes, Wrote %ld bytes,Compression factor %4.2f%%\n",
                    total_read, file_size(outfilename),
                    (1.0-file_size(outfilename)*1.0/total_read)*100.0);
}

/////////////////////////////////////////////////////////////
// Functions for writing asset data to pak file
/////////////////////////////////////////////////////////////
void printData(FILE *someFile, char *text){
    fprintf(someFile,"%s\n", text); /*writes*/
}

static int ptree(char *const source[], char *const output[])
{
	FTS *ftsp;
	FTSENT *p, *chp;
	int fts_options = FTS_COMFOLLOW | FTS_LOGICAL | FTS_NOCHDIR;
	int rval = 0;
    char line[512];
	
	printf("File count is %d\n", fileCountForHeader);
	
	char *const *outputFile = output;
	FILE *pakFile = fopen(*outputFile,"ab");
    
	if ((ftsp = fts_open(source, fts_options, NULL)) == NULL) {
		warn("fts_open");
		return -1;
	}
	
	/* Initialize ftsp with as many argv[] parts as possible. */
	chp = fts_children(ftsp, 0);
	printf("chp = %s\n", chp->fts_name);
	if (chp == NULL) {
		return 0;               /* no files to traverse */
	}
    
	while ((p = fts_read(ftsp)) != NULL)
	{
		switch (p->fts_info) 
		{
			case FTS_D:
				printf("dir: %s\n", p->fts_path);
                printData(pakFile, p->fts_path);
				break;
			case FTS_F:
				printf("file: %s\n", p->fts_path);
                printData(pakFile, p->fts_path);
                
                // compress the fts_path file and write 
                // the compressed data one to pak file
                compress_one_file(p->fts_path, "/Users/Mac/Desktop/temp.txt");
                
                // get the data from the file
                FILE *tempFile;
                tempFile = fopen("/Users/Mac/Desktop/temp.txt","rb");
                
                char byte;
                long offset = ftell(pakFile);
                
                //get the size of the file
                fseek(tempFile, 0L, SEEK_END);
                long size = ftell(tempFile);
                fseek(tempFile, 0L, SEEK_SET);
                
				// print the file info
				char *fileName = p->fts_path;
                char *tempFileName = strdup(fileName);
                if(tempFileName) 
				{
                    printf("\n*********%s\n\n", basename(tempFileName));
                    printf("The offset for %s is %lu\n", tempFileName, offset);
                    printf("The size of the temp file copying to pak is %lu\n", size);
					printf("\nFILE WAS PACKED\n\n*********\n");
                    free(tempFileName);
                }
				
				// copy the data from the temp file to the pak file
                while (!feof(tempFile)) 
                {
                    fread(&byte, sizeof(char), 1, tempFile);
                    fwrite(&byte, sizeof(char), 1, pakFile);
                }

                fclose(tempFile); // done!
            
                if (remove("/Users/Mac/Desktop/temp.txt") == -1)
                    perror("Error in deleting a file");
				break;
			default:
				break;
        }
    }
	fclose(pakFile);
    //decompress_one_file("/Users/Mac/Desktop/temp.txt", "/Users/Mac/Desktop/lastDecompressedTempFile.txt");
    
    fts_close(ftsp);
    return 0;
}

int main(int argc, char *const argv[])
{
	if(argc == 2)
	{
		countFiles(argv + 1);
		
		int rc;
		if((rc = ptree(argv + 1), != 0)
		{
			rc = 1;
			printf("Completed the packing\n");
		}
		// else
		// 	{
		// 		printf("Failed to pack data\n");
		// 	}
	}
	else
	{
		printf("Please pass the source folder name as the only argument\n");
	}
}