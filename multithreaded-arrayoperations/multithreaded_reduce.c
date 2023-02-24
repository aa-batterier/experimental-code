/*
 * The code works like it should, but this reduce can not
 * handle a list where it is important that every element
 * is handled in a certain sequens.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *reduce_thr_fn(void *arg);
int reduce(int fun (int, int), int *array, int size, int numberOfThreads);
int sum(int a, int b);
int difference(int a, int b);
void printArray(int *array, int size);
void printArray2(int *start, int *end);
void inputIntoArray(char **input, int *output, int size);

typedef struct
{
        int *start,
            *end,
            *returnValue;
        int (*func) (int, int);
} reduce_thr_arg;

void *reduce_thr_fn(void *arg)
{
        reduce_thr_arg *input = (reduce_thr_arg*)arg;
        input->returnValue = (int*)malloc(sizeof(int));
        *input->returnValue = input->func(*input->start,*(input->start + 1));
        for (int *iterator = input->start + 2; iterator != input->end; iterator++)
        {
                *input->returnValue = input->func(*input->returnValue,*iterator);
        }
        pthread_exit(NULL);
}

int reduce(int fun (int, int), int *array, int size, int numberOfThreads)
{
        if (numberOfThreads > size)
        {
                numberOfThreads = size;
        }
        pthread_t tids[numberOfThreads];
        int splitArray = size / numberOfThreads,
            remainder = size % numberOfThreads;
        reduce_thr_arg arg[numberOfThreads];
        for (int i = 1, position = 0; i <= numberOfThreads; i++)
        {
                int index = i - 1;
                arg[index].func = fun;
                arg[index].start = &array[position];
                if (i <= remainder)
                {
                        position = position + splitArray + 1;
                }
                else
                {
                        position = position + splitArray;
                }
                arg[index].end = &array[position];
                pthread_create(&tids[index],NULL,reduce_thr_fn,&arg[index]);
        }
        // Make sure all threads have completed.
        for (int i = 0; i < numberOfThreads; i++)
        {
                void *rv = NULL;
                pthread_join(tids[i],&rv);
        }
        int rtV = fun(*arg[0].returnValue,*arg[1].returnValue);
        printf("#%d: %d - %d = %d\n",0,*arg[0].start,*arg[0].end,*arg[0].returnValue);
        printArray2(arg[0].start,arg[0].end);
        printf("#%d: %d - %d = %d\n",1,*arg[1].start,*arg[1].end,*arg[1].returnValue);
        printArray2(arg[1].start,arg[1].end);
        for (int i = 2; i < numberOfThreads; i++)
        {
                printf("#%d: %d - %d = %d\n",i,*arg[i].start,*arg[i].end,*arg[i].returnValue);
                printArray2(arg[i].start,arg[i].end);
                rtV = fun(rtV,*arg[i].returnValue);
                free(arg[i].returnValue);
        }
        return rtV;
}

int sum(int a, int b)
{
        return a + b;
}

int difference(int a, int b)
{
        return a - b;
}

void printArray(int *array, int size)
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
        for (int *iterator = start; iterator != end; iterator++)
        {
                printf("%d ",*iterator);
        }
        printf("]\n");
}

void inputIntoArray(char **input, int *output, int size)
{
        int i = 0;
        for (char **iterator = input; **iterator != ']'; iterator++)
        {
                if (**iterator == '[')
                {
                        continue;
                }
                output[i] = atoi(*iterator);
                i++;
        }
}

int main(int argc, char **argv)
{
        if (argc < 4 || argc > 24 || *argv[2] != '[' || *argv[argc - 1] != ']')
        {
                fprintf(stderr,"usage: %s <number of threads> <[ <number> <number> <...max 20> ]>\n",argv[0]);
                exit(1);
        }
        int size = argc - 4,
            array[size],
            numberOfThreads = atoi(argv[1]);
        inputIntoArray(&argv[2],array,size);
        printArray(array,size);
        int sumResult = reduce(sum,array,size,numberOfThreads);
        printf("Result of sum: %d\n",sumResult);
        int diffResult = reduce(difference,array,size,numberOfThreads); // Will be wrong.
        printf("Result of difference: %d\n",diffResult);
        exit(0);
}
