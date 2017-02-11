#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <omp.h>
#include "sstr.h"


struct sstr *strObj;

void initiateStruct(int substring_Length, int substring_Partitions, char c0, char c1, char c2, int thread_count, int property_Index) {

    if (strObj == NULL)
        strObj = malloc(sizeof(sstr));
    
    (*strObj).substring_Length = substring_Length;
    (*strObj).number_of_Segments = substring_Partitions;
    (*strObj).charArray = malloc(substring_Length * substring_Partitions + 1);
    (*strObj).charIndex = 0;
    (*strObj).currentLength = 0;
    (*strObj).c0 = c0;
    (*strObj).c1 = c1;
    (*strObj).c2 = c2;
    (*strObj).number_of_threads = thread_count;
    (*strObj).property_num = property_Index;
    (*strObj).global_checked_seg = 0;
    (*strObj).global_verified_seg = 0;
}


void *Construct() {
    int my_rank = omp_get_thread_num();
    
    //printf("Hello from thread %ld of %d \n", my_rank, thread_count);
    LOOP: usleep(RandomBetween(0.1, 0.5));
  
    int number_of_Segments, substring_Length, currentStringLength, charIndex, property_number, thread_num;
    char c0, c1, c2;

    #pragma omp critical 
    {
        /** Pull out values from our resource **/
        substring_Length = (*strObj).substring_Length;
        number_of_Segments = (*strObj).number_of_Segments;
        currentStringLength = (*strObj).currentLength;
        charIndex = (*strObj).charIndex;
        property_number = (*strObj).property_num;
        thread_num = (*strObj).number_of_threads;
        c0 = (*strObj).c0;
        c1 = (*strObj).c1;
        c2 = (*strObj).c2;
        
        if (currentStringLength < (number_of_Segments * substring_Length) ) {
           char assignedChar = 'a' + my_rank;  

           (*strObj).charArray[charIndex] = assignedChar;  
           (*strObj).charIndex = charIndex + 1;
           (*strObj).currentLength = currentStringLength + 1;
        }
    }

    
    if (currentStringLength < (number_of_Segments * substring_Length))
        goto LOOP;
    else 
        verify(property_number, number_of_Segments, thread_num, substring_Length, my_rank, c0, c1, c2 );
    
    return NULL;
}


void *verify(int property_Index, int M, int N, int L, int rank, char c0, char c1, char c2 ){
    int num_of_checks;
    if (rank == 0){ //each thread gets work thread 0 picks up the remainer. 
        num_of_checks = (M/N) + (M%N);
    }else{
        num_of_checks = M/N;
    }
    for (int i = 0; i < num_of_checks; i++ ){
            int start, end;
         #pragma omp critical
          {
            start = (*strObj).global_checked_seg*L;
            end = start+L;
            printf(".");
            (*strObj).global_checked_seg++;
          }

        int* resultingCount = count(start, end,c0,c1,c2);

        if (property_Index == 0){
            bool eq = resultingCount[0] + resultingCount[1] == resultingCount[2]; 
          
            if (eq){
                #pragma omp critical 
                {
                   (*strObj).global_verified_seg++;
                   printf(".");
                }
            }
            printf(".");
            
        }else if (property_Index == 1){
            bool eq = resultingCount[0] + 2*(resultingCount[1]) == resultingCount[2];  
            if (eq){
                #pragma omp critical 
                {
                    (*strObj).global_verified_seg++;
                    printf(".");
                }
            }
            printf(".");
            
        }else if (property_Index == 2){
            bool eq = resultingCount[0] * resultingCount[1] == resultingCount[2];  
            if (eq){
                #pragma omp critical 
                {
                    (*strObj).global_verified_seg++;
                    printf(".");
                }
            }
            printf(".");              
        }else if (property_Index == 3){
            bool eq = resultingCount[0] * resultingCount[1] == resultingCount[2];  
            if (eq){
                #pragma omp critical 
                {
                    (*strObj).global_verified_seg++;
                    printf(".");
                }
            }
            printf(".");        
        }else{
            printf("Invalid Property Number submitted! \n");
        }

    }

    return 0;
}

int* count(int start, int end, char c0, char c1, char c2){
        int* charCountArray = (int*) malloc(sizeof(int)*3);

        for (int i= start; i < end; i++)
            if ((*strObj).charArray[i] == c0)
                charCountArray[0]++;
            else if((*strObj).charArray[i] == c1)
                charCountArray[1]++;
            else if ((*strObj).charArray[i] == c2)
                charCountArray[2]++;
            
    return charCountArray;  
}



float RandomBetween(float smallNumber, float bigNumber)
{
    float diff = bigNumber - smallNumber;
    return (((float) rand() / RAND_MAX) * diff) + smallNumber;
}

sstr* returnStrObj() {
    return strObj;
}
