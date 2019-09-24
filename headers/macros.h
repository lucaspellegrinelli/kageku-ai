#ifndef MACROS_H
#define MACROS_H

#include "stdio.h"

// Define an assert
#define DEBUG
#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n) \
if(!(n)) { \
printf("%s - Failed", #n); \
printf("\nOn %s ", __DATE__); \
printf("\nAt %s ", __TIME__); \
printf("\nIn File %s ", __FILE__); \
printf("\nAt Line %d\n", __LINE__); \
exit(1); \
}
#endif

// Converts a file and a rank to a 120-board square
#define FILE_RANK_TO_SQUARE(f, r) ((21 + (f)) + ((r) * 10))

#endif
