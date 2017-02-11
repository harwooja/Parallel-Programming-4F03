#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "sstr.h"
#include "fileio.h"

#define MAX_NUM_ARGUMENTS 8
int thread_count;


int main(int argc, char* argv[]) {
    if (argc > MAX_NUM_ARGUMENTS) {
        printf("Invalid number of arguments");
        exit(1);
    }
   
    initiateFilePointer();
    
    int property_Index, substring_Length, substring_Partitions;
    char c0,c1,c2;    
    pthread_t* thread_handles;

    /*Get number of threads from command line */
    property_Index = strtol(argv[1], NULL, 10);
    thread_count = strtol(argv[2], NULL, 10);
    substring_Length = strtol(argv[3], NULL, 10);
    substring_Partitions = strtol(argv[4], NULL, 10);
    c0 = (*argv[5]);
    c1 = (*argv[6]);
    c2 = (*argv[7]);

    
    // Allocate object on the heap
    thread_handles = malloc(thread_count*sizeof(pthread_t));

    /* Initialize structure paramaters */    
    initiateStruct(substring_Length, substring_Partitions, c0, c1, c2, thread_count, property_Index);

    /* Create our threads */
    #pragma omp parallel num_threads(thread_count)
    {
        Construct();
    }

    // Write to file
    writeToFile(returnStrObj());

    free(thread_handles);
    return 0;
}






   
  


