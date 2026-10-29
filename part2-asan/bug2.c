/* Програма має надрукувати копію рядка (аргумент командного рядка або "Operating Systems")
 * великими літерами: OPERATING SYSTEMS */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *duplicate_upper(const char *source)
{
    char *copy = malloc(strlen(source) + 1);
    if (copy == NULL)
        return NULL;
    strcpy(copy, source);
    for (char *p = copy; *p != '\0'; p++)
        *p = toupper((unsigned char)*p);
    return copy;
}

int main(int argc, char **argv)
{
    const char *source = argc > 1 ? argv[1] : "Operating Systems";
    char *upper = duplicate_upper(source);
    if (upper == NULL)
        return 1;
    printf("%s\n", upper);
    free(upper);
    return 0;
}
