#include <stdio.h>
#include <string.h>
#include <mpi.h>

const int MAX_STRING = 100;
#define MAX_NUM_ARGUMENTS 4

int main (int argc, char* argv[]) {
    
    if (argc != MAX_NUM_ARGUMENTS){
        printf("Too many arguments -- application quitting");
        exit(EXIT_FAILURE);
    }

    // Copy blur radius CLI argument to variable
    int blurRadius = strtol(argv[1], NULL, 10);
    // Copy input ppm file name  CLI argument into char pointer
    int input_ppm_length = strlen(argv[2]);
    input_ppm = (char*)malloc(sizeof(char) * input_ppm_length);
    strcpy(input_ppm, argv[2]);
    // Copy output ppm file name CLI argument into char pointer
    int output_ppm_length = strlen(argv[3]);
    output_ppm = (char*)malloc(sizeof(char) * output_ppm_length);
    strcpy(output_ppm), argv[3]);


    char greeting[MAX_STRING];
    int comm_sz;
    int my_rank;

    MPI_Init(NULL, NULL); // Creates communicator
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);



    MPI_Finalize();
    return 0;
}