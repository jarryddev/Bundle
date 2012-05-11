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
#include "BundleCheckFileType.c"

const char ext_to_ignore[] = ".DS_Store .exe";
static unsigned int fileCountForHeader=0;



/////////////////////////////////////////////////////////////
// Returns a pointer to the filename in a fullpath passed.
/////////////////////////////////////////////////////////////
char *filename(char *path){
  char *t;
  printf("length: %ld\n", strlen(path));
  while ( (t=strchr(path, '/')) != NULL){
    path=++t;
  }
  return path;
}

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
// Functions for writing asset data to pak file
/////////////////////////////////////////////////////////////
void printData(FILE *someFile, char *text)
{
  fprintf(someFile,"%s\n", text); /*writes*/
}

static int packageSourceFolder(char **source, char *desintation, char *extensions[])
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
  unsigned compressed=0;

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
          if(strstr(p->fts_path, ext_to_ignore) != NULL) //ignore DS_Store and exe
            {
              printf("Found file to ignore: %s\n", p->fts_path);
            }
          else
            {
	      			printf("\t>file: %s\n", p->fts_path);
              print
Data(pakFile, p->fts_path);

							FILE *tempFile;
							
							if(shouldCompressFileType(p->fts_path, extensions) == 1)
							{
								/* 	compress the fts_path file and write
	               		the compressed data one to pak file
	              */
								compress_one_file(p->fts_path, "temp.txt");
	 							tempFile = fopen("temp.txt","rb");
							}
              else
							{
									tempFile = fopen(p->fts_path,"rb");
							}
              char byte;
              off_t offset = ftell(pakFile);

              //get the size of the file
              fseek(tempFile, 0L, SEEK_END);
              long size = ftell(tempFile);
              fseek(tempFile, 0L, SEEK_SET);

              char *fileName = p->fts_path;
              char *tempFileName = strdup(fileName);

              //update header
	      			//offset_p off= malloc(HEADER_OFFSET_SIZE);
	      			header_offset off;

              off.hash = __ac_X31_hash_string( filename(fileName) );
              off.size= size;
              off.offset_start= offset;

              header_write_offset(pakFile, &off, f_index++);

							// print the file info
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

							//delete the temporary file
              if (remove("temp.txt") == -1)
                perror("Error in deleting temp file");
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

  if(argc >= 3)
    {
			if(argc == 3)
			{
				printf("No extenstions to compress\n");
			}
			char *extensions[7];
			//get number of compression extension types passed in
			int argCount = argc - 3;
			int index;
			for(index = 0; index < argCount; index++)
			{
				extensions[index] = argv[3 + index];
			}
      
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

      if((rc = packageSourceFolder(sourcePath, destination, extensions) != 0))
        {
          printf("Failed to create Bundle\n");
          rc = 1;
        }
      else
        {
          printf("Bundle created: %s\n", destination);
        }
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
