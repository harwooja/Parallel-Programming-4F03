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

    //MPI_Init(&argc, &argv); // Creates communicator
    MPI_Init(NULL,NULL);
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
        printf("h: %d w: %d \n",input_ppm_height,input_ppm_width );

        int numOfChunks = input_ppm_height / numOfCommunicators;
        /* Create an MPI pixel datatype */
        mpi_pixel_init(&mpixel);

        /* Create a 1D array (contiguous) pixels */
        MPI_Type_contiguous(input_ppm_height, mpixel, &cmpixel);
        MPI_Type_commit(&cmpixel);

        /* Create a 2D array from a 1D array of pixels */
        MPI_Type_contiguous(input_ppm_width, cmpixel, &ampixel);
        MPI_Type_commit(&ampixel);

        int numCols = input_ppm_width;
        // Get row indexes 
        for (int rowIndex = 0; rowIndex < input_ppm_height; rowIndex++) {
            //printf("rowIndex: %d \n", rowIndex);
            
            int minRow = rowIndex - blurRadius;
            int maxRow = rowIndex + blurRadius;
            
            if (minRow < 0)
                minRow = 0;
            if (maxRow > input_ppm_height)
                maxRow = input_ppm_height;
            
            
            //printf("minRow: %d \n", minRow);
            //printf("maxRow: %d \n", maxRow);
        
            //**chunkData= {0};
            
            Pixel ** chunkData = populateMatrix(inputPPM, minRow, maxRow, numCols);
            //unsigned char test = ((Pixel *)chunkData + (0 * numCols) + 0)->rChannel;
            //printf("tester: %u \n", test);
            int chunkSize = (maxRow-minRow)*numCols;
            int processRank = 1;
            
            MPI_Send(&chunkSize, 1, MPI_INT, processRank, 0, MPI_COMM_WORLD);
            //MPI_Send(chunkData, chunkSize, ampixel, processRank, MPI_ANY_TAG, MPI_COMM_WORLD);
            
        }
        MPI_Type_free(&ampixel);
        MPI_Type_free(&cmpixel);
        MPI_Type_free(&mpixel); 
    } else { 
        int chunkSize; 
        printf("rank other than 0 \n");
        MPI_Recv(&chunkSize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //Pixel ** myChunk = {0};
        //myChunk = MPI_Recv(chunkData,chunksz+1 , ampixel, 0, MPI_ANY_TAG, MPI_COMM_WORLD);

    }
    /* Free up the types and finalize MPI */
    
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
Pixel **populateMatrix(Image *matrix, int minRow, int maxRows, int numCols) {
    int rowSize = maxRows - minRow + 1;
    int colSize = numCols;
    Pixel **pix= (Pixel **)malloc(rowSize * colSize * sizeof(Pixel));
    //Loop through pixel rows, create struct for each pixel and store into our sub-matrix
    for (int row = 0; row < rowSize-1; row++) //changed from minrow
        for (int col = 0; col < numCols; col++) {
            //printf("row: %d, col: %d \n", row+minRow, col);
            ((Pixel *)pix + (row * numCols) + col)->rChannel = ImageGetPixel(matrix,  col, row+minRow, 0);
            ((Pixel *)pix + (row * numCols) + col)->gChannel = ImageGetPixel(matrix,  col, row+minRow, 1);
            ((Pixel *)pix + (row * numCols) + col)->bChannel = ImageGetPixel(matrix,  col, row+minRow, 2);
            //unsigned char test = ((Pixel *)pix + (row * numCols) + col)->rChannel;
            //printf("tester: %u \n", test);
        }
     return pix;
}


/*
 * Create a MPI datatype of a pixel.
 */
int mpi_pixel_init(MPI_Datatype *mpi_pixel)
{
    struct Pixel pixel; //added
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