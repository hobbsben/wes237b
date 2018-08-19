
#include "fir_gpu.h"
#include "cuda_timer.h"
#include <iostream>

#include <cuda_runtime.h>
#define BLOCK_SIZE 64


// Baseline
__global__
void fir_kernel1(const float *coeffs, const float *input, float *output, int length, int filterLength)
{
        int id = blockIdx.x * blockDim.x + threadIdx.x;
	//if(id < length-filterLength){
	// TODO

	   float sum = 0.0f;
	   for(int i = 0; i < filterLength; i++)
	   {   
		sum += coeffs[i]*input[i+id];	
 
	   }

	   output[id] = sum;
        }
//}



// Coefficients in shared memory
// Here we suppose that filterLength and BLOCK_SIZE is always 64
__global__
void fir_kernel2(const float *coeffs, const float *input, float *output, int length, int filterLength)
{
	//TODO

  int id = blockIdx.x * blockDim.x + threadIdx.x;

	__shared__  float c[64];
	//for(int i=0; i<64;i++)
	//{c[i]=coeffs[i];}
	c[threadIdx.x] = coeffs[threadIdx.x];
	__syncthreads();
// TODO

	   float sum = 0.0f;
	   for(int i = 0; i < filterLength; ++i)
	   {   
		sum += c[i]*input[i+id];	


	   output[id] = sum;
	}
}
// Coefficients and inputs in shared memory
// Here we suppose that filterLength and BLOCK_SIZE is always 64
__global__
void fir_kernel3(const float *coeffs, const float *input, float *output, int length, int filterLength)
{
	// TODO
int id = blockIdx.x * blockDim.x + threadIdx.x;

	__shared__  float c[64];
	c[threadIdx.x] = coeffs[threadIdx.x];
	 __shared__  float k[BLOCK_SIZE+64];
	k[threadIdx.x] = input[blockDim.x*blockIdx.x + threadIdx.x];
	k[threadIdx.x+BLOCK_SIZE] = input[blockDim.x*blockIdx.x + threadIdx.x+BLOCK_SIZE];
	__syncthreads();

	   float sum = 0.0f;
	   for(int i = 0; i < filterLength; ++i)
	   {   
		sum += c[i]*k[threadIdx.x+i];	

	        
	   }
	output[id] = sum;
}


inline int divup(int a, int b)
{
	if (a % b)
		return a / b + 1;
	else
		return a / b;
}

void fir_gpu(const float *coeffs, const float *input, float *output, int length, int filterLength)
{
	const int output_size = length - filterLength;

	CudaSynchronizedTimer timer;
        
        int grid_size = divup( length, BLOCK_SIZE); // calculate the grid size

	const int block_size = BLOCK_SIZE;

        dim3 block(block_size, 1, 1);
        dim3 grid(grid_size, 1, 1);

	timer.start();
	// TODO Launch kernel here
        //void fir_kernel1(const float *coeffs, const float *input, float *output, int length, int filterLength)
        // fir_kernel1<<<grid,block>>>(coeffs, input, output, output_size, filterLength);
        //fir_kernel2<<<grid,block>>>(coeffs, input, output, output_size, 64);
        fir_kernel3<<<grid,block>>>(coeffs, input, output, output_size, 64);
	
	timer.stop();

	cudaDeviceSynchronize();

	CudaCheckError();

	float time_gpu = timer.getElapsed();
	
	//std::cout << "Kernel Time: " << time_gpu << "ms\n";
}



