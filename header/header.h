#include <sys/types.h>
#include <glib.h>
#include <stdio.h>


//typedef unsigned int    guint; // todo: use glib's 

typedef struct{
  guint hash; // 4
  off_t offset_start; // 8
  size_t size; // 8 
} offset, *offset_p;

FILE* header_file_open(char *);
void header_write_offset(FILE *,offset_p,unsigned int);
offset_p header_read_offset(FILE *, guint);
unsigned int header_init(FILE *, unsigned int);
void header_close(FILE *);
unsigned int header_get_head(FILE *);


