#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void *thr_fn(void *arg);
void mappend(int fun (int), int *array, int size, int numberOfThreads);
int timesTwo(int input);
void printArray(int *array, const int size);
void printArray2(int *start, int *end);
void inputToArray(char **input, int *output);

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
void mappend(int fun (int), int *array, int size, int numberOfThreads)
{
        if (numberOfThreads > size)
        {
                numberOfThreads = size;
        }
        pthread_t tids[numberOfThreads];
        int splitArray = size / numberOfThreads,
            remainder = size % numberOfThreads;
        thr_arg arg[numberOfThreads];
        for (int i = 1, position = 0; i <= numberOfThreads; i++)
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
        for (int i = 0; i < numberOfThreads; i++)
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

void inputToArray(char **input, int *output)
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

// Main function

const int main(const int argc, char **argv)
{
        if ((argc < 3 || argc > 24) && *argv[2] != '[' && *argv[argc - 1] != ']')
        {
                fprintf(stderr,"usage: %s <number of threads> <[ <numbers> ... <max amount is 20> ]>\n",argv[0]);
                exit(0);
        }
        int inputSize = argc - 4,
            inputArray[inputSize],
            numberOfThreads = atoi(argv[1]);
        inputToArray(&argv[2],inputArray);
        printArray(inputArray,inputSize);
        mappend(timesTwo,inputArray,inputSize,numberOfThreads);
        printArray(inputArray,inputSize);
        exit(0);
}
