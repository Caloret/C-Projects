#include "stdio.h"
#include "stdlib.h"
#include "arcadia_test_framework.h"                    
#include "arcadia_string_lib.h"
#include "arcadia_math_lib.h"
#include "stdbool.h"

static const char *TAP_PATH_ENV = "TAP_PATH";

char * get_enviroment_var(const char* name, char **env_value);

struct string_test
{
    arcadia_string *string;
    int words_number;
};

int main(void)
{
    errno_t error_code = 0;

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

    struct string_test test_data[9] = {
        { .string = arcadia_string_create("Hi everyone!"), .words_number = 2 },
        { .string = arcadia_string_create("Hi everyone! "), .words_number = 2 },
        { .string = arcadia_string_create("Hi everyone!  "), .words_number = 2 },
        { .string = arcadia_string_create(" Hi everyone!  "), .words_number = 2 },
        { .string = arcadia_string_create("  Hi everyone!  "), .words_number = 2 },
        { .string = arcadia_string_create("  H i everyone!  "), .words_number = 3 },
        { .string = arcadia_string_create("  H i everyone!  o"), .words_number = 4 },
        { .string = arcadia_string_create(NULL), .words_number = 0 },
        { .string = arcadia_string_create("H"), .words_number = 1 },
    };

    ARCADIA_ASSERT(arcadia_string_count_words(test_data[0].string) == test_data[0].words_number, "The number of words is equal to 2");
    ARCADIA_ASSERT(arcadia_string_count_words(test_data[1].string) == test_data[1].words_number, "The number of words is equal to 2");
    ARCADIA_ASSERT(arcadia_string_count_words(test_data[2].string) == test_data[2].words_number, "The number of words is equal to 2");
    ARCADIA_ASSERT(arcadia_string_count_words(test_data[3].string) == test_data[3].words_number, "The number of words is equal to 2");
    ARCADIA_ASSERT(arcadia_string_count_words(test_data[4].string) == test_data[4].words_number, "The number of words is equal to 2");
    ARCADIA_ASSERT(arcadia_string_count_words(test_data[5].string) == test_data[5].words_number, "The number of words is equal to 3");
    ARCADIA_ASSERT(arcadia_string_count_words(test_data[6].string) == test_data[6].words_number, "The number of words is equal to 4");
    ARCADIA_ASSERT(arcadia_string_count_words(test_data[7].string) == test_data[7].words_number, "The length a null string is zero");
    ARCADIA_ASSERT(arcadia_string_count_words(test_data[8].string) == test_data[8].words_number, "The length of a character is one");

    // Free the string test objects
    for (int i = 0; i < 9; i++) arcadia_string_destroy(test_data[i].string);


    arcadia_string *greeting = arcadia_string_create("Hi no one!");
    arcadia_string *another_greeting = arcadia_string_create("Hi buddie!");
    // Dealing with dynamic memory allocation

    arcadia_string *greeting_allocated = arcadia_string_create(NULL);
    if (greeting_allocated == NULL)
    {
        printf("There was an error allocating memory for greeting_allocated variable.");
        exit(1);
    }

    arcadia_string_copy(greeting, greeting_allocated, greeting->len);

    ARCADIA_ASSERT(
        arcadia_string_comp(greeting, greeting_allocated, greeting->len) == 0, 
        "Both greetings must contain the same letters.");

    ARCADIA_ASSERT(
        arcadia_string_comp(greeting, another_greeting, MIN(greeting->len, another_greeting->len)) > 0, 
        "The second string comes alphabetically after the first one"
    );

    arcadia_string_destroy(greeting);
    arcadia_string_destroy(another_greeting);
    arcadia_string_destroy(greeting_allocated);

    // Enviroment variables
    char *tap_env;
    char *error_desc = get_enviroment_var(TAP_PATH_ENV, &tap_env);

    if (error_desc == NULL)
    {
        printf("\n%s env variable value is %s", TAP_PATH_ENV, tap_env);
    }
    else
    {
        printf(error_desc);
    }

    free(tap_env);


    // VLAs
    


    return error_code;
}


char * get_enviroment_var(const char* name, char **env_value)
{
    size_t env_size = 0;
    
    errno_t error_code = getenv_s(&env_size, NULL, 0, name);
     
    if (env_size == 0)
    {
        return "Enviroment var is not defined";
    }
    
    char *buffer = (char *)malloc(env_size * sizeof(*buffer));
    if (env_value == NULL)
    {
        return "Cannot allocate memory for returning value";
    }
    
    error_code = getenv_s(&env_size, buffer, env_size, name);
    // Use of string parsers
    if (error_code != 0)
    {
        return "There was an error getting the enviroment var";
    }

    *env_value = buffer;

    return NULL;    
}
