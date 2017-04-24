#include <stdio.h>
#include <cuda.h>

__global__ void hello(){
	int myID = (blockIdx.z * gridDim.x * gridDim.y +
				blockIdx.y * gridDim.x +
				blockIdx.x) * blockDim.x + threadIdx.x;
	printf("Hello World from thread %i \n", myID);
}

int main(){
	dim3 b(3,3,3); 
	hello<<<b,32>>>();
	cudaDeviceSynchronize();
	return 1;

}