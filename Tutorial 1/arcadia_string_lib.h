#ifndef ARCADIA_STRING_LIB_H
#define ARCADIA_STRING_LIB_H

#include "stdint.h"
#include "stdlib.h"

typedef struct arcadia_string {
    int len;
    char* data;
} arcadia_string;

arcadia_string *arcadia_string_create(const char *str);

void arcadia_string_destroy(arcadia_string *str);

int arcadia_string_len(const arcadia_string *str);

int arcadia_string_comp(const arcadia_string *first, const arcadia_string *second, int len);

int arcadia_string_count_words(const arcadia_string *phrase);

void arcadia_string_copy(const arcadia_string *source, arcadia_string *target, int size);

#endif