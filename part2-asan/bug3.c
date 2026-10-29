/* Програма будує однозв'язний список 1..5, друкує його і звільняє пам'ять:
 * 1 2 3 4 5 */
#include <stdio.h>
#include <stdlib.h>

struct node {
    int value;
    struct node *next;
};

static struct node *push_front(struct node *head, int value)
{
    struct node *n = malloc(sizeof *n);
    if (n == NULL)
        exit(1);
    n->value = value;
    n->next = head;
    return n;
}

static void free_list(struct node *head)
{
    struct node *cur = head;
    while (cur != NULL) {
        struct node *next = cur->next;
        free(cur);
        cur = next;
    }
}

int main(void)
{
    struct node *head = NULL;
    for (int i = 5; i >= 1; i--)
        head = push_front(head, i);

    for (struct node *cur = head; cur != NULL; cur = cur->next)
        printf("%d ", cur->value);
    printf("\n");

    free_list(head);
    return 0;
}
