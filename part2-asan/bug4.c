/* Програма рахує кількість слів у кожному з трьох речень:
 * 4 7 1 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_words(const char *text)
{
    char *work = malloc(strlen(text) + 1);
    if (work == NULL)
        return -1;
    strcpy(work, text);

    int count = 0;
    for (char *tok = strtok(work, " "); tok != NULL; tok = strtok(NULL, " "))
        count++;
    free(work);
    return count;
}

int main(void)
{
    const char *sentences[] = {
        "Operating systems are fun",
        "A process has its own address space",
        "Segfault",
    };
    for (size_t i = 0; i < sizeof sentences / sizeof sentences[0]; i++)
        printf("%d ", count_words(sentences[i]));
    printf("\n");
    return 0;
}
