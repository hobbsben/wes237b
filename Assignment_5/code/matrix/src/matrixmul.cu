#include <stdio.h>
#include <stdlib.h>

#include "matrixmul.h"
#include "cuda_error.h"

#define BLOCK_SIZE 16

__global__ void block_mm_kernel(const float* A, const float* B, float* output, int M, int N) 
{
	int gidx = blockIdx.x * blockDim.x + threadIdx.x;
	int gidy = blockIdx.y * blockDim.y + threadIdx.y;
	int tidx = threadIdx.x;
        int tidy = threadIdx.y;
        float sum = 0.0;        

        const int NUM_BLOCKS = (N*M)/BLOCK_SIZE;  // calculate amount of blocks
        
        __shared__  float A_shared[BLOCK_SIZE*BLOCK_SIZE];
        __shared__  float B_shared[BLOCK_SIZE*BLOCK_SIZE];
        __shared__  float C_shared[BLOCK_SIZE*BLOCK_SIZE];

        //store input matrices into shared memory
        for(int h = 0; h < M; h+=BLOCK_SIZE)
        { 
            A_shared[tidx+tidy*BLOCK_SIZE + h] = A[tidx+tidy*BLOCK_SIZE + h];
            B_shared[tidx+tidy*BLOCK_SIZE + h] = B[tidx+tidy*BLOCK_SIZE + h];
            __syncthreads();
        

        // calculate sum
           for(int i = 0; i < NUM_BLOCKS; i++)
           {
	       //if(tidx < BLOCK_SIZE || tidy < BLOCK_SIZE)
	      // {
	           C_shared[tidx+tidy] += A_shared[gidx+gidy*M] * B_shared[gidx*N+gidy];
	      // }
           }
           output[gidx+gidy] = C_shared[tidx+tidy];
        }
        
        
}//endline


inline int divup(int a, int b)
{
	if (a % b)
		return a / b + 1;
	else
		return a / b;
}


float run_mm_gpu(const float* A, const float* B, float* C, int M, int N)
{
	// Profiling
	float time_gpu;

	cudaEvent_t start, stop;
	CudaSafeCall(cudaEventCreate(&start));
	CudaSafeCall(cudaEventCreate(&stop));

	CudaSafeCall(cudaEventRecord(start, 0));
	
	dim3 dimGrid(divup(N, BLOCK_SIZE), divup(N, BLOCK_SIZE));
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);

	// Launch kernel 
	block_mm_kernel<<<dimGrid, dimBlock>>>(A, B, C, M, N);

	CudaCheckError();

	CudaSafeCall(cudaThreadSynchronize());

	CudaSafeCall(cudaEventRecord(stop, 0));
	CudaSafeCall(cudaEventSynchronize(stop));
	
	CudaSafeCall(cudaEventElapsedTime(&time_gpu, start, stop));

	// Free Memory
	CudaSafeCall(cudaEventDestroy(start));
	CudaSafeCall(cudaEventDestroy(stop));

	return time_gpu;
}


