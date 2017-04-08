#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "main.h"
#include "pixel.h"


const int MAX_STRING = 100;
#define MAX_NUM_ARGUMENTS 4

int main (int argc, char* argv[]) {
    
    if (argc != MAX_NUM_ARGUMENTS){
        printf("Too many arguments -- application quitting");
        exit(EXIT_FAILURE);
    }

    int numOfCommunicators;
    int communicator_rank;

    MPI_Init(&argc, &argv); // Creates communicator
    MPI_Comm_size(MPI_COMM_WORLD, &numOfCommunicators);
    MPI_Comm_rank(MPI_COMM_WORLD, &communicator_rank);

    if (communicator_rank == 0) {
        // Copy blur radius CLI argument to variable
        int blurRadius = strtol(argv[1], NULL, 10);
        // Copy input ppm file name  CLI argument into char pointer
        int input_ppm_length = strlen(argv[2]);
        input_ppm_filename = (char*)malloc(sizeof(char) * input_ppm_length);
        strcpy(input_ppm_filename, argv[2]);
        // Copy output ppm file name CLI argument into char pointer
        int output_ppm_length = strlen(argv[3]);
        output_ppm_filename = (char*)malloc(sizeof(char) * output_ppm_length);
        strcpy(output_ppm_filename, argv[3]);

        Image *inputPPM = ImageRead(input_ppm_filename);

        int input_ppm_height = ImageHeight(inputPPM);
        int input_ppm_width = ImageWidth(inputPPM);

        //unsigned  char ImageGetPixel(Image *image, int x, int y, int chan)


        int numOfChunks = input_ppm_height / numOfCommunicators;
        // Get row indexes 
        for (int rowIndex = 0; rowIndex < input_ppm_height; rowIndex++) {
            int minRow = rowIndex - blurRadius;
            int maxRow = rowIndex + blurRadius;
            
            if (minRow < 0)
                minRow = 0;
            if (maxRow > input_ppm_height)
                maxRow = input_ppm_height;
            
            struct Pixel chunkMatrix[maxRow][input_ppm_width] = populateMatrix(inputPPM, maxRow, input_ppm_width);

            // Send out to process 1 .. n
        }
  
    } else {


    }

    MPI_Finalize();
    return 0;
}

/** 
Populates our sub-matrices with pixel data for subprocesses
Note: R Channel = 0, G Channel = 1, B Channel = 2 
**/
struct Pixel populateMatrix (Image *matrix, int numRows, int numCols) {
    struct Pixel retArray[numRows][numCols];
    // Loop through pixel rows, create struct for each pixel and store into our sub-matrix
    for (int row = 0; row < numRows; row++)
        for (int col = 0; col < numCols; col++) {
            struct Pixel pix;
            pix.rChannel = ImageGetPixel(matrix, row, col, 0);
            pix.gChannel = ImageGetPixel(matrix, row, col, 1);
            pix.bChannel = ImageGetPixel(matrix, row, col, 2);  
            retArray[row][col] = pix;
        }
    return retArray;
}

