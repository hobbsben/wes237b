#include <stdio.h>
#include <stdlib.h>

#include "matrixmul.h"
#include "cuda_error.h"

#define BLOCK_SIZE 16

__global__ void block_mm_kernel(const float* A, const float* B, float* output, int M, int N) 
{
   // Block index
            int bidx = blockIdx.x;
            int bidy = blockIdx.y;

            int tidx = threadIdx.x;
            int tidy = threadIdx.y;
            __shared__ float A_shared[BLOCK_SIZE][BLOCK_SIZE];
            __shared__ float B_shared[BLOCK_SIZE][BLOCK_SIZE];

       int aBegin = M * BLOCK_SIZE * bidy;
       int aEnd   = aBegin + M - 1;
       int aStep  = BLOCK_SIZE;

       int bBegin = BLOCK_SIZE * bidx;
       int bStep  = BLOCK_SIZE * N;

       float sum = 0;
        // loop through blocks
       for (int a = aBegin, b = bBegin; a <= aEnd; a += aStep, b += bStep) 
	{
A_shared[tidy][tidx] = A[M*tidy+tidx +a];                                                                 
 B_shared[tidy][tidx]= B[N*tidy+tidx +b];                                                                 
    // sync the threads
        __syncthreads();                                                                                  
//accumulation
            for (int k=0;k< BLOCK_SIZE;++k)                                                              
 sum+=A_shared[tidy][k]*B_shared[k][tidx];                                                                
                                                                                                             
// sync it
       __syncthreads();                                                                                  
        }                                                                                                     
        int c = N*BLOCK_SIZE* bidy + BLOCK_SIZE * bidx;                                                       
        output[ N * tidy + tidx +c] = sum;             
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


