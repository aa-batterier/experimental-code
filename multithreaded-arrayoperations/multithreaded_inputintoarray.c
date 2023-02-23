/*
 * Write a multithreaded function which converts input in argv
 * into an array.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
        char **inputStart,
             **inputEnd;
        int *outputStart,
            *outputEnd;
} thr_arg;

void *thr_fn(void *arg)
{
        thr_arg *thr = (thr_arg*)arg;
        /*
         * You can probably use the iterators in the struct instead of
         * creating new like I do here. Research if it doesn't fuck up
         * the pointers in the struct if I do so.
         */
        char **inputIterator = thr->inputStart;
        int *outputIterator = thr->outputStart;
        // Is it better to have iterator iterate to inputEnd instead?
        for (; **inputIterator != ']'; inputIterator++, outputIterator++)
        {
                if (**inputIterator == '[')
                {
                        continue;
                }
                *outputIterator = atoi(*inputIterator);
        }
        pthread_exit(NULL);
}

void inputToArray(char **input, int *output, int size, int numberOfThreads)
{
        if (numberOfThreads > size)
        {
                // Understand why and what happen when I do this down below.
                numberOfThreads = size;
        }
        int splitArray = size / numberOfThreads,
            remainder = size % numberOfThreads;
        // Understand what every element in the arrays below sets to.
        pthread_t tids[numberOfThreads];
        thr_arg arg[numberOfThreads];
        for (int i = 1, position = 0; i <= numberOfThreads; i++)
        {
                // Understand why I set i to 1 instead of 0.
                int index = i - 1;
                arg[index].inputStart = &input[position];
                arg[index].outputStart = &output[position];
                if (i <= remainder)
                {
                        position = position + splitArray + 1;
                }
                else
                {
                        position = position + splitArray;
                }
                arg[index].inputEnd = &input[position];
                arg[index].outputEnd = &output[position];
                pthread_create(&tids[index],NULL,thr_fn,&arg[index]);
        }
        // Wait for all the threads.
        for (int i = 0; i < numberOfThreads; i++)
        {
                void *rv = NULL;
                pthread_join(tids[i],&rv);
        }
}

void printArray(int *array, const int size)
{
        printf("[ ");
        for (int i = 0; i < size; i++)
        {
                printf("%d ",array[i]);
        }
        printf("]\n");
}

/*
 * argv[0] = name of program
 * argv[1] = number of threads
 * argv[2 ... 24] = the input which should be converted into an array
 *      where argv[2] = '[' which is the start symbol of the input list
 *      where argv[last] = ']' which represent the end of the list.
 */
const int main(const int argc, char **argv)
{
        if (argc < 4 || argc > 24 || *argv[2] != '[' || *argv[argc - 1] != ']')
        {
                fprintf(stderr,"usage: %s <number of threads> <[ <number> <number> <...max 20> ]>\n",argv[0]);
                exit(1);
        }
        int size = argc - 4,
            array[size];
        inputToArray(&argv[3],array,size,atoi(argv[1]));
        printArray(array,size);
        exit(0);
}
