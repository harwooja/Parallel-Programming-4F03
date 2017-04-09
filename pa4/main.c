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

    MPI_Init(&argc, &argv); // Creates communicator
    MPI_Comm_size(MPI_COMM_WORLD, &numOfCommunicators);
    MPI_Comm_rank(MPI_COMM_WORLD, &communicator_rank);

    MPI_Datatype mpixel;       /* single pixel */
    MPI_Datatype cmpixel;      /* row/contiguous pixels */
    MPI_Datatype ampixel;      /* 2D array of pixels */

    if (communicator_rank == 0) {
        extractCLIArgs(argv);

        // Read in PPM image
        Image *inputPPM = ImageRead(input_ppm_filename);
        input_ppm_height = ImageHeight(inputPPM);
        input_ppm_width = ImageWidth(inputPPM); 

        int numOfChunks = input_ppm_height / numOfCommunicators;


        /* Create an MPI pixel datatype */
        mpi_pixel_init(&mpixel);

        /* Create a 1D array (contiguous) pixels */
        MPI_Type_contiguous(input_ppm_height, mpixel, &cmpixel);
        MPI_Type_commit(&cmpixel);

        /* Create a 2D array from a 1D array of pixels */
        MPI_Type_contiguous(input_ppm_width, cmpixel, &ampixel);
        MPI_Type_commit(&ampixel);

        int numRows = input_ppm_height;
        int numCols = input_ppm_width;
        // Get row indexes 
        for (int rowIndex = 0; rowIndex < input_ppm_height; rowIndex++) {
            int minRow = rowIndex - blurRadius;
            int maxRow = rowIndex + blurRadius;
            
            if (minRow < 0)
                minRow = 0;
            if (maxRow > input_ppm_height)
                maxRow = input_ppm_height;
        
            struct Pixel *chunkData= {{0}};
            chunkData = populateMatrix(inputPPM, numRows, numCols);
            MPI_Send(&chunkData, 1, ampixel, 1, MPI_ANY_TAG, MPI_COMM_WORLD);
        }
  
    } 

    /* Free up the types and finalize MPI */
    MPI_Type_free(&ampixel);
    MPI_Type_free(&cmpixel);
    MPI_Type_free(&mpixel);
    MPI_Finalize();
    return 0;
}



void extractCLIArgs(char* argv[]){
        // Copy blur radius CLI argument to variable
        blurRadius = strtol(argv[1], NULL, 10);
        // Copy input ppm file name  CLI argument into char pointer
        int input_ppm_length = strlen(argv[2]);
        input_ppm_filename = (char*)malloc(sizeof(char) * input_ppm_length);
        strcpy(input_ppm_filename, argv[2]);
        // Copy output ppm file name CLI argument into char pointer
        int output_ppm_length = strlen(argv[3]);
        output_ppm_filename = (char*)malloc(sizeof(char) * output_ppm_length);
        strcpy(output_ppm_filename, argv[3]);  
}

/** 
Populates our sub-matrices with pixel data for subprocesses
Note: R Channel = 0, G Channel = 1, B Channel = 2 
**/
struct Pixel *populateMatrix(Image *matrix, int numRows, int numCols) {
    struct Pixel *pix= (struct Pixel *)malloc(numRows * numCols * sizeof(Pixel));
    // Loop through pixel rows, create struct for each pixel and store into our sub-matrix
    for (int row = 0; row < numRows; row++)
        for (int col = 0; col < numCols; col++) {
            pix[row][col].rChannel = ImageGetPixel(matrix, row, col, 0);
            pix[row][col].gChannel = ImageGetPixel(matrix, row, col, 1);
            pix[row][col].bChannel = ImageGetPixel(matrix, row, col, 2);  
        }
    return &pix;
}



/*
 * Create a MPI datatype of a pixel.
 */
int mpi_pixel_init(MPI_Datatype *mpi_pixel)
{
    int count = 3;               /* number of blocks in the struct */
    int blocks[3] = {1, 1, 1};   /* set up 3 blocks */
    MPI_Datatype types[3] = {    /* pixel internal types */
            MPI_UNSIGNED_CHAR,
            MPI_UNSIGNED_CHAR,
            MPI_UNSIGNED_CHAR
    };
    MPI_Aint dis[3] = {          /* internal displacements */
            offsetof(struct Pixel, rChannel),
            offsetof(struct Pixel, gChannel),
            offsetof(struct Pixel, bChannel)
    };

    MPI_Type_create_struct(count, blocks, dis, types, mpi_pixel);
    MPI_Type_commit(mpi_pixel);

    return(EXIT_SUCCESS);
} 