#ifndef ARCADIA_TEST_FRAMEWORK_H
#define ARCADIA_TEST_FRAMEWORK_H

#include "assert.h"

#define ARCADIA_ASSERT(x, message) do {  \
    if ((x))                            \
    {                                           \
        printf("\nTEST PASSED %s\n", (#message));       \
    }                                           \
    else                                            \
    {                                           \
        printf("\nTEST FAILED %s\n", (#message));       \
        printf("\nFile: %s\t\tLine: %d\n", __FILE__, __LINE__);       \
    }                                           \
    printf("\n\t %s\n", (#x));                  \
} while(0);                                     \

#endif

