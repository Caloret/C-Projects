#include "stdio.h"
#include "assert.h"

#define CUSTOM_ASSERT(x, y) do {                       \
    if ((x) == (y))                            \
    {                                           \
        printf("\nTEST PASSED %s\n", (#x));       \
    }                                           \
    else                                            \
    {                                           \
        printf("\nTEST FAILED %s\n", (#x));       \
    }                                           \
} while(0);                                     \

// This type is also defined in the c libraries, but we include
// them for learning purposes.
typedef unsigned int u_int;

u_int count_words(char *phrase);

int main(void)
{
    int n1 = 8;
    int n2 = 10;

    int *p1 = &n1;
    int *p2 = &n2;

    // Dereferencing pointers
    printf("Variable n1 value: %d \n", n1);
    printf("Variable p1 value: %d \n", *p1);
    printf("Variable n2 value: %d \n", n2);
    printf("Variable n2 value: %d \n", *p2);

    *p1 = n2;
    printf("Pointer p1 value: %d \n", *p1);
    printf("Variable n1 value: %d \n", n1);
    printf("Variable p1 value: 0x%p \n", (void *)p1);
    
    // Dealing with strings
    char *greeting = "Hi everyone! ";
    u_int words = count_words(greeting);
    printf("Number of words in ...%s... is %u", greeting, words);

    CUSTOM_ASSERT(count_words("Hi everyone!"), 2);
    CUSTOM_ASSERT(count_words("Hi everyone! "), 2);
    CUSTOM_ASSERT(count_words("Hi everyone!  "), 2);
    CUSTOM_ASSERT(count_words(" Hi everyone!  "), 2);
    CUSTOM_ASSERT(count_words("  Hi everyone!  "), 2);
    CUSTOM_ASSERT(count_words("  H i everyone!  "), 3);
    CUSTOM_ASSERT(count_words("  H i everyone!  o"), 4);

    return 0;
}

u_int count_words(char *phrase)
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