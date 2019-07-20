#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void bubble_sort(double *array, int length) // sort an array, from small to big
{
    if(length == 0)
        return;

    for(int i=0; i<length-1; i++)
    {
        for(int j=i+1; j<length; j++) // get i(left) and j(right)
        {
            if(array[i] > array[j]) // if left one is bigger than right one
            {
                double tmp = array[i]; //exchange
                array[i] = array[j];
                array[j] = tmp;
            }
        }
    }
}

int compare(const void *x, const void *y)
{
    return (*(int*)y - *(int*)x);
}

int main()
{
    srand(time(NULL));

    int *numbers = malloc(sizeof(void)); // a dynamic array
    int numbers_length = 0;

    /*double tmp;
    printf("Numbers:");
    while(scanf("%lf", &tmp) == 1) // scan some numbers
    {
        numbers[numbers_length] = tmp;
        numbers_length++;
    }*/

    while(numbers_length<=254)
        numbers[numbers_length++] = rand() % 100000;

    printf("before: ");
    for(int i=0;i<numbers_length;i++)
    {
        if(i % 20 == 0)
            putchar('\n');
        printf("%5d", numbers[i]);
        if(i < numbers_length-1)
            printf(", ");
    }

    qsort(numbers, numbers_length, sizeof(int), compare); // sort the array

    printf("\n\nafter:  ");
    for(int i=0;i<numbers_length;i++)
    {
        if(i % 20 == 0)
            putchar('\n');
        printf("%5d", numbers[i]);
        if(i < numbers_length-1)
            printf(", ");
    }
    printf("\n");

    return 0;
}
