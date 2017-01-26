#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "pa1.h"

#define MAX_NUM_ARGUMENTS 12
int thread_count;

int main(int argc, char* argv[]) {
    if (argc > NUM_ARGUMENTS) {
        printf("Invalid number of arguments");
        exit(1);
    }
   
    long thread; /* Use long in case of a 64-bit system */
    int property_Index, substring_Length, substring_Partitions; 
    
    pthread_t* thread_handles;

    /*Get number of threads from command line */
    property_Index = strtol(argv[1], NULL, 10);
    thread_count = strtol(argv[2], NULL, 10);
    substring_Length = strtol(argv[3], NULL, 10);
    substring_Partitions = strtol(argv[4], NULL, 10);

    // Allocate object on the heap
    thread_handles = malloc(thread_count*sizeof(pthread_t));

    /* Create our threads */
    for (thread = 0; thread < thread_count; thread++) 
        pthread_create(&thread_handles[thread], NULL, Construct, (void*) thread);

    
    printf("Hello from the main thread! \n");

    // Threads will wait until child terminates
    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);


    free(thread_handles);
    return 0;
}

void *Construct(void* rank) {
    long my_rank = (long) rank; 

    sleep(RandomBetween(0.1, 0.5));
    printf("Hello from thread %ld of %d \n", my_rank, thread_count);

    return NULL;
}


float RandomBetween(float smallNumber, float bigNumber)
{
    float diff = bigNumber - smallNumber;
    return (((float) rand() / RAND_MAX) * diff) + smallNumber;
}
