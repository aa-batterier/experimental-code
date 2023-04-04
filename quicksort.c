#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

// Function declaration

void *thr_sort(void *arg);
//void multithreadQsort(void qsort (int *, int, int), int *unsorted, int size, int numberOfThreads);
void multithreadQsort(void qsort (int *, int*), int *unsorted, int size, int numberOfThreads);
void swap(int *pointA, int *pointB);
//int partition(int *unsorted, int left, int right);
int *partition(int *left, int *right);
//int partitionWithRandomPivot(int *unsorted, int left, int right);
int *partitionWithRandomPivot(int *left, int *right);
//void recQsort(int *unsorted, int left, int right);
void recQsort(int *left, int *right);
//void loopQsort(int *unsorted, int left, int right);
void loopQsort(int *left, int *right);
int testQsort(int *sortedArray, int *originalArray, int length);
int isInArray(int member, int *array, int length);
void argumentsIntoIntArray(char **input, int size, int *output);
void printArray(int *array, int size);

// Argument to send in to the function the threads are to run.

typedef struct
{
        /*
        int *list,
            left,
            right;
        */
        int *left,
            *right;
        //void (*func) (int *start, int left, int right);
        void (*func) (int *left, int *right);
} thr_arg;

// Thread functions

void *thr_sort(void *arg)
{
        thr_arg *input = (thr_arg*)arg;
        //input->func(input->list,input->left,input->right);
        input->func(input->left,input->right);
        pthread_exit(NULL);
}

//void multithreadQsort(void qsort (int*, int, int), int *unsorted, int size, int numberOfThreads)
void multithreadQsort(void qsort (int*, int*), int *unsorted, int size, int numberOfThreads)
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
                //arg[index].list = &unsorted[position];
                //arg[index].left = position;
                arg[index].left = &unsorted[position];
                if (i <= remainder)
                {
                        position += splitArray + 1;
                }
                else
                {
                        position += splitArray;
                }
                arg[index].right = &unsorted[position - 1];
                pthread_create(&tids[index],NULL,thr_sort,&arg[index]);
        }
        for (int i = 0; i < numberOfThreads; i++)
        {
                void *rv = NULL;
                pthread_join(tids[i],&rv);
        }
        //qsort(unsorted,0,size - 1);
        qsort(&unsorted[0],&unsorted[size - 1]);
}

// Quicksort helper functions

void swap(int *pointA, int *pointB)
{
        int temp = *pointA;
        *pointA = *pointB;
        *pointB = temp;
}

//int partition(int *unsorted, int left, int right)
int *partition(int *left, int *right)
{
        /*
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
        */
        int *pivot = right;
        left--;
        while (1)
        {
                /*
                 * Under stand what the difference is between *(++pointer) and *++pointer
                 * and &(++pointer) and &++pointer.
                 */
                //while (*(++left) < *pivot);
                while (*++left < *pivot);
                //while (*right > 0 && *(--right) > *pivot);
                while (*right > 0 && *--right > *pivot);
                if (left >= right)
                {
                        break;
                }
                swap(left,right);
        }
        swap(left,pivot);
        return left;
}

//int partitionWithRandomPivot(int *unsorted, int left, int right)
int *partitionWithRandomPivot(int *left, int *right)
{
        srand(time(NULL));
        //swap(&unsorted[left + rand() % (right - left)],&unsorted[right]);
        swap(left + rand() % (right - left),right);
        //return partition(unsorted,left,right);
        return partition(left,right);
}

// Quicksort functions

//void recQsort(int *unsorted, int left, int right)
void recQsort(int *left, int *right)
{
        if (right - left <= 0)
        {
                return;
        }
        //int p = partition(unsorted, left, right);
        //int *p = partition(unsorted, left, right);
        //int p = partitionWithRandomPivot(left, right);
        int *p = partitionWithRandomPivot(left, right);
        //recQsort(unsorted,left,p - 1);
        recQsort(left,p - 1);
        //recQsort(unsorted,p + 1, right);
        recQsort(p + 1, right);
}

//void loopQsort(int *unsorted, int left, int right)
void loopQsort(int *left, int *right)
{
        int *stack[right - left + 1],
            top = -1;
        stack[++top] = left;
        stack[++top] = right;
        for (; top >= 0;)
        {
                right = stack[top--];
                left = stack[top--];
                if (right == left)
                {
                        return;
                }
                //int p = partition(left,right);
                //int *p = partition(left,right);
                //int p = partitionWithRandomPivot(left,right);
                int *p = partitionWithRandomPivot(left,right);
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

int testQsort(int *sortedArray, int *originalArray, int length)
{
        if (!isInArray(sortedArray[0],originalArray,length))
        {
                return 0;
        }
        for (int h = 0, i = 1; i < length; i++, h++)
        {
                if (sortedArray[h] > sortedArray[i] || !isInArray(sortedArray[0],originalArray,length))
                {
                        return 0;
                }
        }
        return 1;
}

int isInArray(int member, int *array, int length)
{
        for (int i = 0; i < length; i++)
        {
                if (member == array[i])
                {
                        return 1;
                }
        }
        return 0;
}

// Helper functions

// TODO: Make this function multithreaded.
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
        if (argc < 2 || argc > 21 || strcmp(argv[2],"[") != 0 || strcmp(argv[argc - 1],"]") != 0)
        {
                fprintf(stderr,"usage %s <number of threads> [ <number> <number> ... up to 20 numbers ]\n",argv[0]);
                exit(1);
        }

        char **inputArray = argv + 3;
        int length = argc - 4,
            threads = atoi(argv[1]),
            originalArray[length],
            unsortedRec[length],
            unsortedLoop[length],
            unsortedMultithread[length];

        // TODO: Run all of these in sperate threads.
        argumentsIntoIntArray(inputArray,length,originalArray);
        argumentsIntoIntArray(inputArray,length,unsortedRec);
        argumentsIntoIntArray(inputArray,length,unsortedLoop);
        argumentsIntoIntArray(inputArray,length,unsortedMultithread);

        printf("Unsorted array: ");
        printArray(unsortedRec,length);

        //recQsort(unsortedRec,0,length - 1);
        recQsort(unsortedRec,&unsortedRec[length - 1]);
        printf("Recursively sorted array: ");
        printArray(unsortedRec,length);
        if (testQsort(unsortedRec,originalArray,length))
        {
                printf("testQsort: Pass\n");
        }
        else
        {
                printf("testQsort: Fail\n");
        }

        //loopQsort(unsortedLoop,0,length - 1);
        loopQsort(unsortedLoop,&unsortedLoop[length - 1]);
        printf("Iterative sorted array: ");
        printArray(unsortedLoop,length);
        if (testQsort(unsortedLoop,originalArray,length))
        {
                printf("testQsort: Pass\n");
        }
        else
        {
                printf("testQsort: Fail\n");
        }

        multithreadQsort(recQsort,unsortedMultithread,length,threads);
        printf("Multithreaded quicksorted array: ");
        printArray(unsortedMultithread,length);
        if (testQsort(unsortedMultithread,originalArray,length))
        {
                printf("testQsort: Pass\n");
        }
        else
        {
                printf("testQsort: Fail\n");
        }

        exit(0);
}
