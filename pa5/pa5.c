//TJ Walker 1204996 and Jake Harwood 1226732
//assignment 4 comp sci 4F03

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
extern "C"{
	# include "ppmFile.h"
}
#include "ppmFile.h"

const int MAX_STRING = 100;
#define MAX_NUM_ARGUMENTS 4

int blurRadius; 
char * input_ppm_filename; 
char * output_ppm_filename;
Image * inputPPM;
Image * outputPPM; 
void blurFilter(int myRank, int numOfProcesses); 
void blur( int x, int y); 
/*  main  */
int main(int argc, char **argv) {

int i;

 if (argc != MAX_NUM_ARGUMENTS) {
    printf("Too many arguments -- application quitting");
    exit(1);
  }
    blurRadius = atoi(argv[1]);
    input_ppm_filename= argv[2];
    output_ppm_filename = argv[3];
	inputPPM = ImageRead(input_ppm_filename);
  	outputPPM = ImageCreate(inputPPM->width, inputPPM->height);
	int communicator_rank;
   	int numOfCommunicators;
    
   	MPI_Init(NULL, NULL);
   	MPI_Comm_size(MPI_COMM_WORLD, &numOfCommunicators);
   	MPI_Comm_rank(MPI_COMM_WORLD, &communicator_rank);
	blurFilter(communicator_rank, numOfCommunicators);

	if (communicator_rank == 0){
		ImageWrite(outputPPM, output_ppm_filename); 
	}

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

}

void blur( int x, int y){
	
	int	minX;  
	int	minY; 
	int	maxX; 
	int	maxY; 
	minX = x - blurRadius;
	minY = y - blurRadius;
	maxX = x + blurRadius;
	maxY = y + blurRadius;

	if (minX < 0) {
		minX = 0;
	}
	if (minY < 0) {
		minY = 0;
	}
	if (maxX >= inputPPM->width){
		maxX = inputPPM->width ;
	}  
	if (maxY >= inputPPM->height){
		maxY = inputPPM->height ;
	}
    
    unsigned long int rgbBLUR[3] = {0, 0, 0};
    
	int i;
	int j;
	int k;
	for (j = minY; j <= maxY; j++){
		for (i = minX; i <= maxX; i++){
			for (k = 0; k < 3; k++){
				rgbBLUR[k] += ImageGetPixel(inputPPM, i, j, k);
				int numOfPixels = ((maxX - minX + 1) * (maxY - minY + 1));
				ImageSetPixel(outputPPM, x, y, k, (unsigned char)( rgbBLUR[k] / numOfPixels ));
			}
		}
	}
}

void blurFilter(int myRank, int numOfProcesses)
{
	int i; 
	int j;
	for (j = myRank; j < inputPPM->height; j += numOfProcesses){
		for (i = 0; i < inputPPM->width; i++){
			blur(i, j);
		}
	}
	for (j = myRank; j < outputPPM->height; j += numOfProcesses){
			MPI_Send((void*)(outputPPM->data + j * outputPPM->width * 3), outputPPM->width * 3, MPI_UNSIGNED_CHAR,0,0,MPI_COMM_WORLD);
		}
	if (myRank == 0){
		for (j = 1; j < outputPPM->height; j++){
			if (j % numOfProcesses > 0){
				MPI_Recv( (void*)(outputPPM->data + j * outputPPM->width * 3),outputPPM->width * 3,MPI_UNSIGNED_CHAR,j % numOfProcesses,0,MPI_COMM_WORLD,NULL);
			}
		}
	}		
}



