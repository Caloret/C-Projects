#ifndef ARCADIA_STRING_LIB_H
#define ARCADIA_STRING_LIB_H

#include "stdint.h"

int arcadia_string_len(char *source);

int arcadia_string_comp(char *first, char *second, int len);

int arcadia_string_count_words(char *phrase);

void arcadia_string_copy(char *source, char *target, int size);

#endif