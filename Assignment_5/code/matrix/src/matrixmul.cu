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
        float sum = 0;
        

        const int NUM_BLOCKS = M/(BLOCK_SIZE);  // calculate amount of blocks
        
        __shared__  float A_shared[BLOCK_SIZE*BLOCK_SIZE];
        __shared__  float B_shared[BLOCK_SIZE*BLOCK_SIZE];
        //__shared__  float C_shared[BLOCK_SIZE*BLOCK_SIZE];

        //loop through blocks in the grid 
        for(int grid_block = 0; grid_block < 1; grid_block++)       
        {   
            //check grid boundary conditions
            //if(tidx+tidy*grid_block < M*N)
            //{
                //store input matrices into shared memory
                A_shared[tidx+(tidy*BLOCK_SIZE)] = A[(tidx + BLOCK_SIZE* grid_block) +(gidy*M/*added M*/)];
                B_shared[tidx+(tidy*BLOCK_SIZE)] = B[gidx*N + (tidy+BLOCK_SIZE*  grid_block)];


                __syncthreads();
             //}
         // loop through elements within the block
         //for(int a_row = 0; a_row < BLOCK_SIZE; a_row++)
          //  {
            for(int b_row = 0; b_row < BLOCK_SIZE; b_row++)
                {
                
                   sum += A_shared[tidx  + (b_row  * BLOCK_SIZE)] 
                        * B_shared[tidy  + (b_row  * BLOCK_SIZE)];
	           
                
	        }
          //   }
           output[gidx+(gidy*N) ] = sum;
        }
//printf("%d a_shared : %f\n",blockIdx.x, A_shared[tidx+(tidy*BLOCK_SIZE)]);
//printf("%d b_shared : %f\n",blockIdx.y, B_shared[tidx+(tidy*BLOCK_SIZE)]);
        
        
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


