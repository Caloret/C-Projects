#include "arcadia_string_lib.h"


int arcadia_string_len(char *input)
{
    int size = 0;

    if (input == NULL) return size;

    while(*input != '\0')
    {
        size++;
        input++;
    }

    return size;
}

int arcadia_string_comp(char *first, char *second, int len)
{
    if (first == NULL && second == NULL) return 0;

    if (first == NULL) return -1;

    if (second == NULL) return 1;

    int result = 0;

    for(int i = 0; i < len; ++i)
    {
        result = *first - *second;

        if (result != 0) break;

        first++;
        second++;
    }

    return result;
}

void arcadia_string_copy(char *source, char *target, int size)
{
    if (source == NULL)
    {
        target = NULL;
        return;
    }
        
    for(int i = 0; i < size; ++i)
    {
        *target = *source;
        target++;
        source++;
    }
}

int arcadia_string_count_words(char *phrase)
{
    char *start_word_pointer = phrase;
    char *word_pointer = phrase;
    unsigned int words = 0;

    while (*word_pointer != '\0')
    {
        if (*word_pointer == ' ')
        {
            if ((word_pointer - start_word_pointer) > 0)
            {
                words++;
                start_word_pointer = word_pointer;
            }
            start_word_pointer++;
        }

        word_pointer++;
    }

    // Tail separators
    if (start_word_pointer != word_pointer)
    {
        words++;
    }

    return words;
}