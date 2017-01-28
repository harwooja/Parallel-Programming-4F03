#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include "pa1.h"

//global veraible for how mant segments are verified
//global for checked segments from verify, M_seg_counter


#define MAX_NUM_ARGUMENTS 8
int thread_count;

pthread_mutex_t mutexLock;
struct SOB* strObj;

int global_checked_seg = 0;
int global_verified_seg = 0;

int main(int argc, char* argv[]) {
    if (argc > MAX_NUM_ARGUMENTS) {
        printf("Invalid number of arguments");
        exit(1);
    }
   
    long thread; /* Use long in case of a 64-bit system */
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
    strObj = malloc(sizeof(struct SOB));

    /* Initialize structure paramaters */

    (*strObj).substring_Length = substring_Length;
    (*strObj).number_of_Segments = substring_Partitions;
    (*strObj).charArray[(substring_Length + substring_Partitions) + 1];
    (*strObj).charIndex = 0;
    (*strObj).currentLength = 0;
    (*strObj).c0 = c0;
    (*strObj).c1 = c1;
    (*strObj).c2 = c2;
    (*strObj).number_of_threads = thread_count;
    (*strObj).property_num = property_Index;
    

    /* Create our threads */
    for (thread = 0; thread < thread_count; thread++) 
        pthread_create(&thread_handles[thread], NULL, Construct, (void*) thread);

    //printf("Hello from the main thread! \n");

    // Threads will wait until child terminates
    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);

    free(thread_handles);
    return 0;
}

void *Construct(void* rank) {
    long my_rank = (long) rank; 
    
    //printf("Hello from thread %ld of %d \n", my_rank, thread_count);
    LOOP: usleep(RandomBetween(0.1, 0.5));
  
    pthread_mutex_lock(&mutexLock);
        /** Pull out values from our resource **/
        int substring_Length = (*strObj).substring_Length;
        int number_of_Segments = (*strObj).number_of_Segments;
        int currentStringLength = (*strObj).currentLength;
        int charIndex = (*strObj).charIndex;
        int property_number = (*strObj).property_num;
        int thread_num = (*strObj).number_of_threads;
        char c0 = (*strObj).c0;
        //printf("c0 = %c \n", c0);
        char c1 = (*strObj).c1;
        //printf("c1 = %c \n", c1);
        char c2 = (*strObj).c2;
        //printf("c2 = %c \n", c2);

        if (currentStringLength < (number_of_Segments * substring_Length) ) {
           char assignedChar = 'a' + my_rank;  
           (*strObj).charArray[charIndex] = assignedChar;  
           (*strObj).charIndex = charIndex + 1;
           (*strObj).currentLength = currentStringLength + 1;

           printf("Char: %c, Index: %i \n", (*strObj).charArray[charIndex], charIndex);
        }
    pthread_mutex_unlock(&mutexLock);

    if (currentStringLength < (number_of_Segments * substring_Length))
        goto LOOP;
    else
        verify(property_number, number_of_Segments, thread_num, substring_Length, my_rank, c0, c1, c2 );
    
    return NULL;
}



float RandomBetween(float smallNumber, float bigNumber)
{
    float diff = bigNumber - smallNumber;
    return (((float) rand() / RAND_MAX) * diff) + smallNumber;
}


void *verify(int property_Index, int M, int N, int L, int rank, char c0, char c1, char c2 ){
    int num_of_checks;
    if (rank == 0){ //each thread gets work thread 0 picks up the remainer. 
        num_of_checks = (M/N) + (M%N);
    }else{
        num_of_checks = M/N;
    }
    for (int i = 0; i < num_of_checks; i++ ){
        pthread_mutex_lock(&mutexLock); //grab lock
        int start = global_checked_seg*L;
        int end = start+L;
            printf("checking segment %d for property %d correctness from thread %d.\n", global_checked_seg, property_Index, rank);
            global_checked_seg++;
        pthread_mutex_unlock(&mutexLock); //release lock
        int* resultingCount = count(start, end,c0,c1,c2);

        if (property_Index == 0){
            bool eq = resultingCount[0] + resultingCount[1] == resultingCount[2]; 
              //printf("f0 == 0, %d + %d = %d \n",resultingCount[0], resultingCount[1], resultingCount[2] ); 
            if (eq){
                pthread_mutex_lock(&mutexLock); //grab lock
                    global_verified_seg++;
                    printf("Number of verified segments %d \n",global_verified_seg);
                pthread_mutex_unlock(&mutexLock); //release lock
            }
        }else if (property_Index == 1){
            bool eq = resultingCount[0] + 2*(resultingCount[1]) == resultingCount[2];  
            if (eq){
                pthread_mutex_lock(&mutexLock); //grab lock
                    global_verified_seg++;
                    printf("Number of verified segments %d \n",global_verified_seg);
                pthread_mutex_unlock(&mutexLock); //release lock
            }
        }else if (property_Index == 2){
            bool eq = resultingCount[0] * resultingCount[1] == resultingCount[2];  
            if (eq){
                pthread_mutex_lock(&mutexLock); //grab lock
                    global_verified_seg++;
                    printf("Number of verified segments %d \n",global_verified_seg);
                pthread_mutex_unlock(&mutexLock); //release lock
            }              
        }else if (property_Index == 3){
            bool eq = resultingCount[0] * resultingCount[1] == resultingCount[2];  
            if (eq){
                pthread_mutex_lock(&mutexLock); //grab lock
                    global_verified_seg++;
                    printf("Number of verified segments %d \n",global_verified_seg);
                pthread_mutex_unlock(&mutexLock); //release lock
            }            
        }else{
            printf("Invalid Property Number submitted! \n");
        }

    }
}

int* count(int start, int end, char c0, char c1, char c2){
    int i = start;
    int c0_count = 0;
    int c1_count = 0;
    int c2_count = 0;
    for (i; i < end; i++){ //(*strObj).charArray[i]
           // printf("counting, string char = %c \n", (*strObj).charArray[i]);
            if ((*strObj).charArray[i] == c0){
                c0_count++;
            }else if((*strObj).charArray[i] == c1){
                c1_count++;
            }else if ((*strObj).charArray[i] == c2){
                c2_count++;
            }else{}
        }
    int* CharCount = (int*) malloc(sizeof(int)*3);
    CharCount[0] = c0_count;
    CharCount[1] = c1_count;
    CharCount[2] = c2_count;

    return CharCount;  
}
//}    
  


