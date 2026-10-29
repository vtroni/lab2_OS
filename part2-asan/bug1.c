/* Програма має надрукувати середнє арифметичне чисел 10, 20, 30, 40, 50: 30.00 */
#include <stdio.h>
#include <stdlib.h>

double average(const int *values, int count)
{
    long sum = 0;
    for (int i = 0; i < count; i++)
        sum += values[i];
    return (double)sum / count;
}

int main(void)
{
    int count = 5;
    int *values = malloc(count * sizeof *values);
    if (values == NULL)
        return 1;
    for (int i = 0; i < count; i++)
        values[i] = (i + 1) * 10;

    printf("average = %.2f\n", average(values, count));
    free(values);
    return 0;
}
