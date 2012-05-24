#include <sys/time.h>
#include <stdio.h>
struct timeval tv;

int profiler_start();
suseconds_t profiler_getTime();