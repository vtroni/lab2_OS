/* Програма створює запис про студента, робить його копію з іншою оцінкою
 * і друкує обидва: Ivan 90 / Ivan 75 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student {
    char *name;
    int grade;
};

static struct student student_make(const char *name, int grade)
{
    struct student s;
    s.name = malloc(strlen(name) + 1);
    if (s.name == NULL)
        exit(1);
    strcpy(s.name, name);
    s.grade = grade;
    return s;
}

static void student_free(struct student *s)
{
    free(s->name);
}

int main(void)
{
    struct student first = student_make("Ivan", 90);
    struct student second = student_make(first.name, 75);

    printf("%s %d\n", first.name, first.grade);
    printf("%s %d\n", second.name, second.grade);

    student_free(&first);
    student_free(&second);
    return 0;
}
