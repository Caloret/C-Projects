#include "arcadia_string_lib.h"
#include "arcadia_math_lib.h"

static int arcadia_string_len_data(const char *input)
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

static int arcadia_string_comp_data(const char *first, const char *second, int len)
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

static void arcadia_string_copy_data(const char *source, char *target, int size)
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

static int arcadia_string_count_words_data(const char *phrase)
{
    const char *start_word_pointer = phrase;
    const char *word_pointer = phrase;
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

arcadia_string *arcadia_string_create(const char *str)
{
    int size = arcadia_string_len_data(str);

    arcadia_string *arc_str = malloc(sizeof(*arc_str) + size);

    arc_str->len = size;
    arc_str->data = malloc(size);
    arcadia_string_copy_data(str, arc_str->data, size);

    return arc_str; 
}

int arcadia_string_len(const arcadia_string *str)
{
    if (str == NULL) return 0;

    return str->len;
}

int arcadia_string_comp(const arcadia_string *first, const arcadia_string *second, int len)
{
    return arcadia_string_comp_data(first->data, second->data, len);
}

int arcadia_string_count_words(const arcadia_string *phrase)
{
    if (phrase == NULL) return 0;

    if (phrase->data == NULL) return 0;

    return arcadia_string_count_words_data(phrase->data);
}

void arcadia_string_copy(const arcadia_string *source, arcadia_string *target, int size)
{
    int final_size = MIN(size, source->len);

    arcadia_string_copy_data(source->data, target->data, final_size);
    
    target->len = final_size;
}

void arcadia_string_destroy(arcadia_string *str)
{
    if (str == NULL) return;

    if (str->data != NULL)
    {
        free(str->data);
    }

    free(str);

    str = NULL;
}