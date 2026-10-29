/* Програма заповнює масив квадратами чисел 1..5 і друкує його:
 * 1 4 9 16 25 */
#include <stdio.h>

static void fill_squares(int *out, int count)
{
    for (int i = 0; i < count; i++)
        out[i] = (i + 1) * (i + 1);
}

int main(void)
{
    int squares[5];
    fill_squares(squares, 5);
    for (int i = 0; i < 5; i++)
        printf("%d ", squares[i]);
    printf("\n");
    return 0;
}
