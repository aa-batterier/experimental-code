#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define MAX_THREADS 4

void *thr_fn(void *arg);
void mappend(int fun (int), int *array, int size);
int timesTwo(int input);
void printArray(int *array, const int size);
void printArray2(int *start, int *end);

typedef struct
{
        int *start,
            *end;
        int (*func) (int);
} thr_arg;

// Thread functions

void *thr_fn(void *arg)
{
        thr_arg *input = (thr_arg*)arg;
        for (int *iterator = input->start; iterator != input->end; iterator++)
        {
                *iterator = input->func(*iterator);
        }
        pthread_exit(NULL);
}

// Operator functions

// Destructive
void mappend(int fun (int), int *array, int size)
{
        pthread_t tids[MAX_THREADS];
        int splitArray = size / MAX_THREADS,
            remainder = size % MAX_THREADS;
        thr_arg arg[MAX_THREADS];
        for (int i = 1, position = 0; i <= MAX_THREADS && i <= size; i++)
        {
                int index = i - 1;
                arg[index].func = fun;
                arg[index].start = &array[position];
                if (i <= remainder)
                {
                        position = position + splitArray + 1;
                        arg[index].end = &array[position];
                }
                else
                {
                        position = position + splitArray;
                        arg[index].end = &array[position];
                }
                pthread_create(&tids[index],NULL,thr_fn,&arg[index]);
        }
        for (int i = 0; i < MAX_THREADS; i++)
        {
                void *rv = NULL;
                pthread_join(tids[i],&rv);
        }
}

// Send in functions

int timesTwo(int input)
{
        return input * 2;
}

// Helper function

void printArray(int *array, const int size)
{
        printf("[ ");
        for (int i = 0; i < size; i++)
        {
                printf("%d ",array[i]);
        }
        printf("]\n");
}

void printArray2(int *start, int *end)
{
        printf("[ ");
        for (int *p = start; p != end; p++)
        {
                printf("%d ",*p);
        }
        printf("]\n");
}

// Main function

const int main(const int argc, char **argv)
{
        int array[] = {1,2,3,4,5,6,7,8,9,10,
                       11,12,13,14,15,16,17,18,19,20,
                       21,22,23,24,25,26,27,28,29,30,
                       31,32,33,34,35,36,37,38,39,40,
                       41,42,43,44,45,46,47,48,49,50,
                       51,52,53,54,55,56,57,58,59,60,
                       61,62,63,64,65,66,67,68,69,70,
                       71,72,73,74,75,76,77,78,79,80,
                       81,82,83,84,85,86,87,88,89,90,
                       91,92,93,94,95,96,97,98,99,100},
            size = sizeof(array) / sizeof(array[0]);
        printArray(array,size);
        mappend(timesTwo,array,size);
        printArray(array,size);
        exit(0);
}
