/* 
* Lab: memReorder.c
*
* Demonstration of processor memory reordering in multi-CPU system
*
* Author: Vladimir Krapp
* Created on: 18/3/14
*/
#define _GNU_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


int varA = 0;
int varB = 0;

int varX = 0;
int varY = 0;

void* threadOne(void* arg)
{
    varA = 1;
    varX = varB;
    return 0;
}

void* threadTwo(void* arg)
{
    varB = 1;
    varY = varA;
    return 0;
}


int main(void)
{
    pthread_t t[2];
    pthread_attr_t at[2];
    cpu_set_t cpuset;
   
    int i = 0;

    int res[4] = {0, 0, 0, 0};

    for(; i < 1e5; ++i)
    {
        
        CPU_ZERO(&cpuset);
        CPU_SET(0, &cpuset);
        pthread_attr_init(&at[0]);
        pthread_attr_setaffinity_np(&at[0], sizeof(cpuset), &cpuset);

        CPU_ZERO(&cpuset);
        CPU_SET(1, &cpuset);
        pthread_attr_init(&at[1]);
        pthread_attr_setaffinity_np(&at[1], sizeof(cpuset), &cpuset);

        pthread_create(&t[0], &at[0] , threadOne, NULL);
        pthread_create(&t[1], &at[1] , threadTwo, NULL);
        pthread_join(t[0], NULL);
        pthread_join(t[1], NULL);

        if(varX == 0 && varY == 0)
        {
            res[0] += 1;
        }
        if(varX == 0 && varY == 1)
        {
            res[1] += 1;
        } 
        if(varX == 1 && varY == 0)
        {
            res[2] += 1;
        }
        if(varX == 1 && varY == 1)
        {
            res[3] += 1;
        }
        varA = 0;
        varB = 0;
        varX = 0;
        varY = 0;

    }

    printf("varX = 0, varY = 0: %d\n", res[0]);
    printf("varX = 0, varY = 1: %d\n", res[1]);
    printf("varX = 1, varY = 0: %d\n", res[2]);
    printf("varX = 1, varY = 1: %d\n", res[3]);

	return 0;
}

/************************************************************/
