#include <stdlib.h>
#include <stdio.h>

// It would be greate to make this functions work on all types.

// Array operation functions:
// ---------------------

// Destructive
void mapcan(int func (int), int *array, int size)
{
        for (int i = 0; i < size; i++)
        {
                array[i] = func(array[i]);
        }
}

//Non-destructive
int *mapcar(int func (int), int *array, int size)
{
        int *result = (int*)calloc(size,sizeof(int));
        for (int i = 0; i < size; i++)
        {
                result[i] = func(array[i]);
        }
        return result;
}

int foldl(int func (int, int), int startValue, int *array, int size)
{
        int result = func(startValue,array[0]);
        for (int i = 1; i < size; i++)
        {
                result = func(result,array[i]);
        }
        return result;
}

int foldr(int func (int, int), int startValue, int *array, int size)
{
        int result = func(startValue, array[size - 1]);
        for (int i = size - 2; i >= 0; i--)
        {
                result = func(result,array[i]);
        }
        return result;
}

int reducel(int func (int, int), int *array, int size)
{
        return foldl(func,array[0],&array[1],size - 1);
}

int reducer(int func (int, int), int *array, int size)
{
        return foldr(func,array[size - 1],&array[1],size - 2);
}

//Destructive
void nreverse(int *array, int size)
{
        for (int first = 0, last = size - 1; first != last; first++, last--)
        {
                int temp = array[first];
                array[first] = array[last];
                array[last] = temp;
        }
}

// Non-destructive
int *reverse(int *array, int size)
{
        int *result = (int*)calloc(size,sizeof(int));
        for (int first = 0, last = size - 1; first < size && last >= 0; first++, last--)
        {
                result[first] = array[last];
        }
        return result;
}

// ----------------------

// Functions to send in as arguments (function pointers):
// --------------------

int timesTwo(int n)
{
        return 2 * n;
}

int sum(int a, int b)
{
        return a + b;
}

int difference(int a, int b)
{
        return a - b;
}

int product(int a, int b)
{
        return a * b;
}

// -------------------

// Help functions:
// ---------------

void printArray(int *array, int size)
{
        printf("[ ");
        for (int i = 0; i < size; i++)
        {
                printf("%d ",array[i]);
        }
        printf("]\n");
}

// -------------

const int main(const int argc, char *argv[])
{
        int array[] = {1,2,3,4,5,6,7,8,9};
        int size = 9;
        printf("Start array: ");
        printArray(array,size);
        int *mapArray = mapcar(timesTwo,array,size);
        printf("Mapcar with timesTwo: ");
        printArray(mapArray,size);
        free(mapArray);
        printf("Reducel with sum: %d\n",reducel(sum,array,size));
        printf("Reducel with product: %d\n",reducel(product,array,size));
        printf("Reducel with difference: %d\n",reducel(difference,array,size));
        // Doesn't work, returns -26, should return -27
        printf("Reducer with difference: %d\n",reducer(difference,array,size));
        printf("Foldl with sum and start value 10: %d\n"
               ,foldl(sum,10,array,size));
        int *reverseArray = reverse(array,size);
        printf("Reverse: ");
        printArray(reverseArray,size);
        free(reverseArray);
        exit(0);
}
