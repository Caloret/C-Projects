#ifndef ARCADIA_TEST_FRAMEWORK_H
#define ARCADIA_TEST_FRAMEWORK_H

#include "assert.h"
#include "stdio.h"

#define ARCADIA_ASSERT(x, message) do {  \
    if ((x))                            \
    {                                           \
        fprintf(stdout, "\nTEST PASSED %s\n", (#message));       \
    }                                           \
    else                                            \
    {                                           \
        fprintf(stderr, "\nTEST FAILED %s\n", (#message));       \
        fprintf(stderr, "\nFile: %s\t\tLine: %d\n", __FILE__, __LINE__);       \
    }                                           \
    fprintf(stdout, "\n\t %s\n", (#x));                  \
} while(0)                                     \

#endif

