//TJ Walker 1204996 and Jake Harwood 1226732
//assignment 5 comp sci 4F03


#include <stdlib.h>
#include <stdio.h>
extern "C" {
	#include "ppmFile.h"
}

#define MAX_NUM_ARGUMENTS 4

int blurRadius;
char * input_ppm_filename;
char * output_ppm_filename;
Image * inputPPM;
Image * outputPPM;

__device__ void blur(int x, int y, int blurRadius, int width, int height, unsigned char *input, unsigned char *output) {
	

	int minX = x - blurRadius;
	int minY = y - blurRadius;
	int maxX = x + blurRadius;
	int maxY = y + blurRadius;

	if (minX < 0) {
		minX = 0;
	}
	if (minY < 0) {
		minY = 0;
	}
	if (maxX >= width){
		maxX = width;
	}  
	if (maxY >= height){
		maxY = height;
	}

	unsigned long int rgbBLUR[3] = {0, 0, 0};

	for (int j = minY; j <= maxY; j++){
		for (int i = minX; i <= maxX; i++){
			for (int k = 0; k < 3; k++){
				rgbBLUR[k] += input[j * width * 3 + i * 3 + k];
				int numOfPixels = ((maxX - minX + 1) * (maxY - minY + 1));
				output[y * width * 3 + x * 3 + k] = (unsigned char)(rgbBLUR[k] / numOfPixels);
			}
		}
	}
}

__global__ void blurFilter(int width, int height, int blurRadius, unsigned char * inputImgData, unsigned char * outputImgData) {
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int	y = blockIdx.y * blockDim.y + threadIdx.y;
	if (x < width && y < height){
		blur(x, y, blurRadius, width, height, inputImgData, outputImgData);
	}	
}



/* main */

int main(int argc, char **argv) {

	if (argc != MAX_NUM_ARGUMENTS) {
		printf("Too many arguments -- application quitting");
		exit(1);
	}

    blurRadius = atoi(argv[1]);
    input_ppm_filename= argv[2];
    output_ppm_filename = argv[3];
    inputPPM = ImageRead(input_ppm_filename);
	outputPPM = ImageCreate(inputPPM->width, inputPPM->height);
	int imageSize= inputPPM->width * inputPPM->height * 3;
	unsigned char *inputImgData, *outputImgData;

	int threadsPerBlock = 32;

	dim3 gridCustom(ceil((double)inputPPM->width / threadsPerBlock), ceil((double)inputPPM->height / threadsPerBlock), 1);
	dim3 blockCustom(threadsPerBlock, threadsPerBlock, 1);
	
	cudaMalloc(&inputImgData, imageSize);
	cudaMalloc(&outputImgData, imageSize);
	cudaMemcpy(inputImgData, inputPPM->data, imageSize, cudaMemcpyHostToDevice);

	blurFilter<<<gridCustom, blockCustom>>>(inputPPM->width, inputPPM->height, blurRadius, inputImgData, outputImgData);
	cudaDeviceSynchronize();

	cudaMemcpy(outputPPM->data, outputImgData, imageSize, cudaMemcpyDeviceToHost);
	ImageWrite(outputPPM, argv[3]);

	cudaFree(inputImgData);
	cudaFree(outputImgData);
	
	free(inputPPM);
	free(outputPPM);

	return 0;
}


