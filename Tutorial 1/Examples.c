#include "stdio.h"
#include "stdlib.h"
#include "arcadia_test_framework.h"                    
#include "arcadia_string_lib.h"
#include "stdbool.h"


#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

static const char *TAP_PATH_ENV = "TAP_PATH";

char * get_enviroment_var(const char* name, char *env_value);


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

    char *greeting = "Hi everyone! ";
    int words = arcadia_string_count_words(greeting);
    printf("Number of words in ...%s... is %u", greeting, words);

    ARCADIA_ASSERT(arcadia_string_count_words("Hi everyone!") == 2, "The number of words is correct");
    ARCADIA_ASSERT(arcadia_string_count_words("Hi everyone! ") == 2, "The number of words is correct");
    ARCADIA_ASSERT(arcadia_string_count_words("Hi everyone!  ") == 2, "The number of words is correct");
    ARCADIA_ASSERT(arcadia_string_count_words(" Hi everyone!  ") == 2, "The number of words is correct");
    ARCADIA_ASSERT(arcadia_string_count_words("  Hi everyone!  ") == 2, "The number of words is correct");
    ARCADIA_ASSERT(arcadia_string_count_words("  H i everyone!  ") == 3, "The number of words is correct");
    ARCADIA_ASSERT(arcadia_string_count_words("  H i everyone!  o") == 4, "The number of words is correct");

    ARCADIA_ASSERT(arcadia_string_len(NULL) == 0, "The length a null string is zero");
    ARCADIA_ASSERT(arcadia_string_len("H") == 1, "The length of a character is one");
    ARCADIA_ASSERT(arcadia_string_len("How it is going\0 1000..") == 15, "The length the string is 15");
    ARCADIA_ASSERT(arcadia_string_len("Never ! again") == 13, "The length a null string is 13");

    char *another_greeting = "Hi no one!";
    int another_greeting_length = arcadia_string_len(another_greeting);
    int greeting_length = arcadia_string_len(greeting);
    
    // Dealing with dynamic memory allocation

    char *greeting_allocated = malloc(300 * sizeof(*greeting_allocated));
    if (greeting_allocated == NULL)
    {
        printf("There was an error allocating memory for greeting_allocated variable.");
        exit(1);
    }

    arcadia_string_copy(greeting, greeting_allocated, greeting_length);

    ARCADIA_ASSERT(
        arcadia_string_comp(greeting, greeting_allocated, greeting_length) == 0, 
        "Both greetings must contain the same letters.");

    ARCADIA_ASSERT(
        arcadia_string_comp(greeting, another_greeting, MIN(greeting_length, another_greeting_length)) < 0, 
        "The second string comes alphabetically after the first one"
    );

    free(greeting_allocated);

    // Enviroment variables
    char *tap_env = NULL;
    char *error_desc = get_enviroment_var(TAP_PATH_ENV, tap_env);

    if (error_desc == NULL)
    {
        printf("\n%s env variable value is %s", TAP_PATH_ENV, tap_env);
    }
    else
    {
        printf(error_desc);
    }

    free(tap_env);

    return error_code;
}


char * get_enviroment_var(const char* name, char *env_value)
{
    size_t env_size = 0;
    
    errno_t error_code = getenv_s(&env_size, NULL, 0, name);
     
    if (env_size == 0)
    {
        return "Enviroment var is not defined";
    }
    
    env_value = (char *)malloc(env_size * sizeof(*env_value));
    if (env_value == NULL)
    {
        return "Cannot allocate memory for returning value";
    }
    
    error_code = getenv_s(&env_size, env_value, env_size, name);
    // Use of string parsers
    if (error_code != 0)
    {
        return "There was an error getting the enviroment var";
    }
    printf("\nenv value %s\n", env_value);
    
    return NULL;    
}
