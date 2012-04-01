#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *fh;
int main(int argc, char **argv) {
  long a=1;
  long b=2;
  int c=10;
  char *p= malloc(sizeof(a)*2+1);
  bzero(p, sizeof(a)*2);
  memcpy(p, &a, sizeof(a));
  memcpy(p+sizeof(a), (void*)&b, sizeof(b));
  memcpy(p+sizeof(int)+sizeof(long), &c, sizeof(c));
  printf("%x\n", *(p+sizeof(long)));
  free(p);
}
