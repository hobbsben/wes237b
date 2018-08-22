#include <stdio.h>
#include <stdlib.h>

#include "matrixmul.h"
#include "cuda_error.h"

#define BLOCK_SIZE 16

__global__ void block_mm_kernel(const float* A, const float* B, float* output, int M, int N) 
{
int idx = blockIdx.x * blockDim.x + threadIdx.x;
int idy = blockIdx.y * blockDim.y + threadIdx.y;
	if(1)
	{
	output[idy*M+idx]=A[idy*M+idx]*B[idy*M+idx];
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


