#ifndef _HEADER_INCLUDED
#include <sys/types.h>
#include <stdio.h>
#include "khash.h"
#define _HEADER_INCLUDED
#endif

/**
 * Bundle header offset struct
 */
typedef struct{
  khint_t hash; // 4
  long int offset_start; // 8
  size_t size; // 8
  char compressed; // 1
} header_offset, *offset_p;

#define HEADER_OFFSET_SIZE (size_t)(sizeof(khint_t)+sizeof(long int)+sizeof(size_t)+sizeof(char))

FILE* header_file_open(char *);
void header_write_offset(FILE *,offset_p,unsigned int);
offset_p header_read_offset(FILE *, khint_t);
unsigned int header_init(FILE *, unsigned int);
void header_close(FILE *);
unsigned int header_get_head(FILE *);
void print_offset(offset_p);
offset_p *header_get_offsets(FILE *);
