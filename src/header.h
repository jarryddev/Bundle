#include <sys/types.h>
#include <stdio.h>
#include "types.h"

/**
 * Bundle header offset struct
 */
typedef struct{
  ub4 hash; // 4
  off_t offset_start; // 8
  size_t size; // 8
} header_offset, *offset_p;

FILE* header_file_open(char *);
void header_write_offset(FILE *,offset_p,unsigned int);
offset_p header_read_offset(FILE *, ub4);
unsigned int header_init(FILE *, unsigned int);
void header_close(FILE *);
unsigned int header_get_head(FILE *);
void print_offset(offset_p);

