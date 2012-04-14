//
//      BundleHandler.c
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
#include "header.h"
#include "BundleHandler.h"

const char ext_to_ignore[] = ".DS_Store .exe";
static unsigned int fileCountForHeader=0;

/////////////////////////////////////////////////////////////
// Count the number of files for header length calculation
/////////////////////////////////////////////////////////////
static int countFiles(char **source)
{
  FTS *ftsp;
  FTSENT *p, *chp;
  int fts_options = FTS_COMFOLLOW | FTS_LOGICAL | FTS_NOCHDIR;

  if ((ftsp = fts_open(source, fts_options, NULL)) == NULL) {
    warn("fts_open");
    return -1;
  }

  /* Initialize ftsp with as many argv[] parts as possible. */
  chp = fts_children(ftsp, 0);
  if (chp == NULL) {
    return 0;               /* no files to traverse */
  }

  while ((p = fts_read(ftsp)) != NULL)
    {
      switch (p->fts_info) {
      case FTS_F:

        if(strstr(p->fts_path, "DS_Store") == NULL) //wasn't a DS_Store file.
          {
            //increment the file count
            fileCountForHeader++;
          }
        break;
      default:
        break;
      }
    }
  fts_close(ftsp);
  return 0;
}

/////////////////////////////////////////////////////////////
// Compression/Decompression
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

  printf("\t>Read %ld bytes, Wrote %ld bytes,Compression factor %4.2f%%\n",
         total_read, file_size(outfilename),
         (1.0-file_size(outfilename)*1.0/total_read)*100.0);
}
/////////////////////////////////////////////////////////////
// Function for adding pak extension 
/////////////////////////////////////////////////////////////
char * checkPakExtension (char *filePath)
{
	char *tempPath = malloc(strlen(filePath) + 4);
	char *start = filePath;
	strcpy(tempPath, filePath);
	int count = strlen(filePath);
	printf("Count is %d\n", count);
	while(*filePath != '\0')
	{
		filePath++;
	}
	// now we are at end of filename
	filePath--;
	if(*filePath == 'k')
	{
		filePath--;
		if(*filePath == 'a')
		{
			filePath--;
			if(*filePath == 'p')
			{
				filePath--;
				if(*filePath == '.')
				{
					printf("pak entension exists\n");
					return start;
				}
			}
		}
	}
	else
	{
		//add pak entension
		strcat(tempPath, ".pak");
	}
	free(tempPath);
}
/////////////////////////////////////////////////////////////
// Function for checking file Extension
/////////////////////////////////////////////////////////////
int shouldCompressFileType(char *filePath)
{
	char *extension;
	char *extTemp;
	int extLength = 0;
	
	while(filePath != '\0')
	{
		filePath++;
	}
	while(filePath != '.')
	{
		filepath--;
		extLength++;
	}
	printf("Extension length is %d\n", extLength);
	extLength -= 1;
	extension = malloc(sizeOf(char) * extLength);
	*extension = filepath++;
	while(filepath != '\0')
	{
		*extension++ = filepath++;
	}
	int i;
	for(i = 0; i < extLength; i--)
	{
		extension--;
	}
	printf("Extension is :%s\n", extension);
	return 1;
}


/////////////////////////////////////////////////////////////
// Functions for writing asset data to pak file
/////////////////////////////////////////////////////////////
void printData(FILE *someFile, char *text)
{
  fprintf(someFile,"%s\n", text); /*writes*/
}

static int ptree(char **source, char *desintation)
{
  FTS *ftsp;
  FTSENT *p, *chp;
  int fts_options = FTS_COMFOLLOW | FTS_LOGICAL | FTS_NOCHDIR;
  int rval = 0; // remove if unused!
  char line[512];
  FILE *pakFile;
  FILE *file;
  unsigned int start_offset= 0;
  unsigned int f_index=0; // file index

  if (file = fopen(desintation, "r"))
    {
      printf("File exists, would you like to overwrite it y/n ?  ");

      int answer;
      int loop=1;
      fclose(file);

      while (loop){

        answer = getchar();

        switch(answer){
        case 'y':
	  loop=0;
          break;
        case 'n':
          printf("Please run Bundle again with the correct output path.\n");
	  exit(1);
        case 10: // \n
          break;
        default:
          printf("not sure ? ;) \n (y/n) >");
        }
      }
    }else {
    fclose(file);
  }
  

  pakFile = fopen(desintation,"wb+");
  
  printf("\n----------------\n");

  // initialize header with nuber of files
  if ((start_offset=header_init(pakFile, fileCountForHeader)) == -1){
    // perror("bundler_header");
    fprintf(stderr, "Cannot initialize header...exiting\n");
    return 0;
  }

  if ((ftsp = fts_open(source, fts_options, NULL)) == NULL) {
    warn("fts_open");
    return -1;
  }

  /* Initialize ftsp with as many argv[] parts as possible. */
  chp = fts_children(ftsp, 0);
  if (chp == NULL) {
    return 0;               /* no files to traverse */
  }

  printf("\nCreating the Bundle\n\n");

  while ((p = fts_read(ftsp)) != NULL)
    {
      switch (p->fts_info)
        {
        case FTS_D:
          printf(">dir: %s\n\n", p->fts_path);
          printData(pakFile, p->fts_path);
          break;
        case FTS_F:
          if(strstr(p->fts_path, ext_to_ignore) != NULL) //ignore DS_Store files on mac
            {
              printf("Found DS_Store file, ignoring. Mac ftw?\n");
            }
          else
            {
              printf("\t>file: %s\n", p->fts_path);
              printData(pakFile, p->fts_path);

              // compress the fts_path file and write
              // the compressed data one to pak file
              compress_one_file(p->fts_path, "temp.txt");

              // get the data from the file
              FILE *tempFile;
              tempFile = fopen("temp.txt","rb");

              char byte;
              off_t offset = ftell(pakFile);

              //get the size of the file
              fseek(tempFile, 0L, SEEK_END);
              long size = ftell(tempFile);
              fseek(tempFile, 0L, SEEK_SET);

              // print the file info
              char *fileName = p->fts_path;
              char *tempFileName = strdup(fileName);

              //update header
              offset_p off= malloc(sizeof(header_offset));

              off->hash = __ac_X31_hash_string(fileName);
              off->size= size;
              off->offset_start= offset;
              header_write_offset(pakFile, off, f_index++);
              printf("writing offset:\n");
	      print_offset(off);
              free(off);

              if(tempFileName)
                {
                  printf("\t>The offset for %s is %d\n", tempFileName, (unsigned int)offset);
                  printf("\t>The compressed size of %s is %lu\n", basename(tempFileName), size);
                  printf("\t>%s was added to the bundle\n\n", basename(tempFileName));
                  free(tempFileName);
                }

              // copy the data from the temp compressed file to the pak file
              fseek(pakFile, start_offset, SEEK_SET);
              while (!feof(tempFile))
                {
                  fread(&byte, sizeof(char), 1, tempFile);
                  fwrite(&byte, sizeof(char), 1, pakFile);
                }


              fclose(tempFile); // done!

              if (remove("temp.txt") == -1)
                perror("Error in deleting a file");
              break;
            }
        default:
          break;
        }
    }
  fclose(pakFile);
  fts_close(ftsp);
  return 0;
}

int main(int argc, char *const argv[])
{
  int rc;

  if(argc == 3)
    {
      printf("\n"); // this adds a space for readability in terminal
      char **sourcePath = malloc(2 * (sizeof *sourcePath));
      sourcePath[0] = malloc(strlen(argv[1]) + 1);
      if(!sourcePath[0])
      printf("Could not start package process\n");
      strcpy(sourcePath[0], argv[1]);
      sourcePath[1] = NULL;
      countFiles(sourcePath);

      char *destination = malloc(strlen(argv[2]));
      strcpy(destination, argv[2]);

      if((rc = ptree(sourcePath, destination) != 0))
        {
          printf("Failed to create Bundle\n");
          rc = 1;
        }
      else
        {
          printf("Bundle created: %s\n", destination);
        }


      // header file tests...
      FILE *tfh = fopen(destination, "rb+");

      unsigned int num_files  = header_get_head(tfh);
      printf("header :: num files : %d\n", num_files);

      // try to read an offset

            offset_p* offsets= header_get_offsets(tfh);
      int i=0;

      for (i=0; i< num_files; i++){
              print_offset(offsets[i]);
      }

      //      free(offsets);

      fclose(tfh);

      free(sourcePath[0]);
      free(sourcePath);
      free(destination);
    }
  else
    {
      printf("Usage: BundleHandler <source_path> <destination path>\n");
    }

  printf("\n"); // again, this improves readability. imo :)




  return rc;
}
