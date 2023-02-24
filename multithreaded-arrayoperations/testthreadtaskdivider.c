#include <stdio.h>
#include <stdlib.h>

void printArray(int *startPos, int *endPos)
{
        printf("[ ");
        for (int *p = startPos; p != endPos; p++)
        {
                printf("%d ",*p);
        }
        printf("]\n");
}

void threaddivider(int *array, int size, int numberOfThreads)
{
        int defaultTasks = size / numberOfThreads,
            remainder = size % numberOfThreads;
        for (int i = 1, *startPos = array, position = 0; i <= numberOfThreads && i <= size; i++)
        {
                int *endPos = NULL;
                if (i <= remainder)
                {
                        position += defaultTasks + 1;
                }
                else
                {
                        position += defaultTasks;
                }
                endPos = &array[position];
                printf("#%d\n",i);
                printf("%d - %d\n",*startPos,*endPos);
                printArray(startPos,endPos);
                startPos = endPos;
        }
}

const int main(const int argc, char **argv)
{
        if (argc != 2)
        {
                fprintf(stderr,"usage: %s <number of threads>\n",argv[0]);
                exit(1);
        }
        int array[] = {0,1,2,3,4,5,6,7,8,9},
            size = 10;
        printArray(array,&array[size]);
        threaddivider(array,size,atoi(argv[1]));
        exit(0);
}
