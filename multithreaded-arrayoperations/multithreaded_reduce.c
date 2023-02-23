#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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
                        arg[index].end = &array[position];
                }
                else
                {
                        position = position + splitArray;
                        arg[index].end = &array[position];
                }
                pthread_create(&tids[index],NULL,reduce_thr_fn,&arg[index]);
        }
        // Make sure all threads have completed.
        for (int i = 0; i < numberOfThreads; i++)
        {
                void *rv = NULL;
                pthread_join(tids[i],&rv);
        }
        int rtV = fun(*arg[0].returnValue,*arg[1].returnValue);
        for (int i = 2; i < numberOfThreads; i++)
        {
                rtV = fun(rtV,*arg[i].returnValue);
                free(arg[i].returnValue);
        }
        return rtV;
}

int sum(int a, int b)
{
        return a + b;
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

int main(void)
{
        int array[] = {1,2,3,4,5,6,7,8,9},
            size = 9;
        printArray(array,size);
        int result = reduce(sum,array,size,4);
        printf("Result of sum: %d\n",result);
        // Add difference test to se in the input values comes in in the correct order.
        exit(0);
}
