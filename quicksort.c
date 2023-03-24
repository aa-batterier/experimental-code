/*
 * Strategi for multithreaded quicksort:
 * 1. Start by dividing the array according to how many elements there are in
 *    the array in relation to how many threads are going to sort the array
 *    (do as you have done in the other multithreaded algorithms you have written).
 * 2. Send in the subarrays to the threads and in the threads sort the subarrays.
 * 3. Do a final sort in the main thread on the array.
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

// Function declaration

void *thr_sort(void *arg);
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
        int *start,
            *end,
            left,
            right;
        /*
         * Need to rewrite all the quicksort algorithms so they take two pointers,
         * one pointing to the begining of the array and
         * one pointing to the end of the array instead of
         * one pointer pointing only to the end of the array.
         */
        void (*func) (int *start, int *end, int left, int right);
} thr_arg;

// Thread function

void *thr_sort(void *arg)
{
        thr_arg *input = (thr_arg*)arg;
        input->func(input->start,input->end,input->left,input->right);
        pthread_exit(NULL);
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
                int p = partitionR(unsorted,left,right);
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
            unsortedLoop[length];
        argumentsIntoIntArray(argv + 1,length,unsortedRec);
        argumentsIntoIntArray(argv + 1,length,unsortedLoop);
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
        exit(0);
}
