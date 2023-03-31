#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

// Function declaration

void *thr_sort(void *arg);
void multithreadQsort(void qsort (int *, int, int), int *unsorted, int size, int numberOfThreads);
void swap(int *pointA, int *pointB);
int partition(int *unsorted, int left, int right);
int partitionWithRandomPivot(int *unsorted, int left, int right);
void recQsort(int *unsorted, int left, int right);
void loopQsort(int *unsorted, int left, int right);
int testQsort(int *sortedArray, int length);
void argumentsIntoIntArray(char **input, int size, int *output);
void printArray(int *array, int size);

// Argument to send in to the function the threads are to run.

typedef struct
{
        int *list,
            left,
            right;
        /*
         * 'right' is the same as 'end' - pointer.
         * Maybe a nicer solution is to replace the intgers left and right
         * with pointers.
         */
        void (*func) (int *start, int left, int right);
} thr_arg;

// Thread functions

void *thr_sort(void *arg)
{
        thr_arg *input = (thr_arg*)arg;
        input->func(input->list,input->left,input->right);
        pthread_exit(NULL);
}

void multithreadQsort(void qsort (int*, int, int), int *unsorted, int size, int numberOfThreads)
{
        if (numberOfThreads > size)
        {
                numberOfThreads = size;
        }
        int splitArray = size / numberOfThreads,
            remainder = size % numberOfThreads;
        pthread_t tids[numberOfThreads];
        thr_arg arg[numberOfThreads];
        for (int i = 1, position = 0; i <= numberOfThreads; i++)
        {
                int index = i - 1;
                arg[index].func = qsort;
                arg[index].list = &unsorted[position];
                arg[index].left = position;
                if (i <= remainder)
                {
                        position += splitArray + 1;
                }
                else
                {
                        position += splitArray;
                }
                arg[index].right = position - 1;
                pthread_create(&tids[index],NULL,thr_sort,&arg[index]);
        }
        for (int i = 0; i < numberOfThreads; i++)
        {
                void *rv = NULL;
                pthread_join(tids[i],&rv);
        }
        qsort(unsorted,0,size - 1);
}

// Quicksort helper functions

void swap(int *pointA, int *pointB)
{
        int temp = *pointA;
        *pointA = *pointB;
        *pointB = temp;
}

int partition(int *unsorted, int left, int right)
{
        int leftPointer = left - 1,
            rightPointer = right,
            pivot = unsorted[right];
        while (1)
        {
                while (unsorted[++leftPointer] < pivot);
                while (rightPointer > 0 && unsorted[--rightPointer] > pivot);
                if (leftPointer >= rightPointer)
                {
                        break;
                }
                swap(&unsorted[leftPointer],&unsorted[rightPointer]);
        }
        swap(&unsorted[leftPointer],&unsorted[right]);
        return leftPointer;
}

int partitionWithRandomPivot(int *unsorted, int left, int right)
{
        srand(time(NULL));
        swap(&unsorted[left + rand() % (right - left)],&unsorted[right]);
        return partition(unsorted,left,right);
}

// Quicksort functions

void recQsort(int *unsorted, int left, int right)
{
        if (right - left <= 0)
        {
                return;
        }
        //int p = partition(unsorted, left, right);
        int p = partitionWithRandomPivot(unsorted, left, right);
        recQsort(unsorted,left,p - 1);
        recQsort(unsorted,p + 1, right);
}

void loopQsort(int *unsorted, int left, int right)
{
        int stack[right - left + 1],
            top = -1;
        stack[++top] = left;
        stack[++top] = right;
        for (; top >= 0;)
        {
                right = stack[top--];
                left = stack[top--];
                int p = partitionWithRandomPivot(unsorted,left,right);
                if (p - 1 > left)
                {
                        stack[++top] = left;
                        stack[++top] = p - 1;
                }
                if (p + 1 < right)
                {
                        stack[++top] = p + 1;
                        stack[++top] = right;
                }
        }
}

// Test function

int testQsort(int *sortedArray, int length)
{
        for (int h = 0, i = 1; i < length; i++, h++)
        {
                if (sortedArray[h] > sortedArray[i])
                {
                        return 0;
                }
        }
        return 1;
}

// Helper functions

void argumentsIntoIntArray(char **input, int size, int *output)
{
        for (int i = 0; i < size; i++)
        {
                output[i] = atoi(input[i]);
        }
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

// Main function

int main(int argc, char *argv[])
{
        if (argc < 2 || argc > 21)
        {
                fprintf(stderr,"usage %s <number> <number> ... up to 20 numbers\n",argv[0]);
                exit(1);
        }
        int length = argc - 1,
            unsortedRec[length],
            unsortedLoop[length],
            unsortedMultithread[length];
        argumentsIntoIntArray(argv + 1,length,unsortedRec);
        argumentsIntoIntArray(argv + 1,length,unsortedLoop);
        argumentsIntoIntArray(argv + 1,length,unsortedMultithread);
        printf("Unsorted array: ");
        printArray(unsortedRec,length);

        recQsort(unsortedRec,0,length - 1);
        printf("Recursively sorted array: ");
        printArray(unsortedRec,length);
        if (testQsort(unsortedRec,length))
        {
                printf("testQsort: Pass\n");
        }
        else
        {
                printf("testQsort: Fail\n");
        }

        loopQsort(unsortedLoop,0,length - 1);
        printf("Iterative sorted array: ");
        printArray(unsortedLoop,length);
        if (testQsort(unsortedLoop,length))
        {
                printf("testQsort: Pass\n");
        }
        else
        {
                printf("testQsort: Fail\n");
        }

        multithreadQsort(recQsort,unsortedMultithread,length, 4);
        printf("Multithreaded quicksorted array: ");
        printArray(unsortedMultithread,length);
        if (testQsort(unsortedMultithread,length))
        {
                printf("testQsort: Pass\n");
        }
        else
        {
                printf("testQsort: Fail\n");
        }

        exit(0);
}
