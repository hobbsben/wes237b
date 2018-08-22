#include "filter.h"
#include "cuda_timer.h"
#include "math.h"
#include <iostream>

using namespace std;


__global__
void kernel_sobel_filter(const uchar * input, uchar * output, const uint height, const uint width)
{ 

        int idx = blockIdx.x * blockDim.x + threadIdx.x;
        int idy = blockIdx.y * blockDim.y + threadIdx.y;

	const int sobel_x[3][3] = {
		{-1, 0, 1},
		{-2, 0, 2},
		{-1, 0, 1}
	};
	const int sobel_y[3][3]  = {
		{-1, -2, -1},
		{0,   0,  0},
		{1,   2,  1}
	};


	// TODO

	   float sumx = 0.0f;
           float sumy = 0.0f;
	   for(int i = 0; i < 3; i++)
	   {   
                for (int j = 0; j < 3; j++)
		{
		      sumx += sobel_x[i][j] * input[(idy+i)*width +idx+j];
                      sumy += sobel_y[i][j] * input[(idy+i)*width +idx+j];
                       
 		}
	   }

	   output[(idy)*width +idx] = sqrt(pow(sumx,2) + pow(sumy,2));
        
}


inline int divup(int a, int b)
{
	if (a % b)
		return a / b + 1;
	else
		return a / b;
}

/**
 * Wrapper for calling the kernel.
 */
void sobel_filter_gpu(const uchar * input, uchar * output, const uint height, const uint width)
{
	const int size = height * width * sizeof(uchar);

	CudaSynchronizedTimer timer;


        // Launch the kernel
	const int grid_x = 64;
	const int grid_y = 64;
     
        int block_x = divup(width, grid_x); // calculate the grid size
        int block_y = divup(height, grid_y); // calculate the grid size
        
	// dim3 grid(1, 1, 1);  // TODO
	// dim3 block(1, 1, 1); // TODO
        dim3 grid(grid_x, grid_y, 1);  // TODO
	dim3 block(block_x, block_y, 1); // TODO
	timer.start();
	kernel_sobel_filter<<<grid, block>>>(input, output, height, width);
	timer.stop();

	cudaDeviceSynchronize();

	float time_kernel = timer.getElapsed();
}


void sobel_filter_cpu(const uchar * input, uchar * output, const uint height, const uint width)
{
	const int sobel_x[3][3] = {
		{-1, 0, 1},
		{-2, 0, 2},
		{-1, 0, 1}
	};
	const int sobel_y[3][3]  = {
		{-1, -2, -1},
		{0,   0,  0},
		{1,   2,  1}
	};

	for (uint y = 1; y < height - 1; ++y)
	{
		for (uint x = 1; x < width - 1; ++x)
		{

			const int pixel_x = (int) (
					(sobel_x[0][0] * input[x-1 + (y-1) * width]) + 
					(sobel_x[0][1] * input[x   + (y-1) * width]) + 
					(sobel_x[0][2] * input[x+1 + (y-1) * width]) +
					(sobel_x[1][0] * input[x-1 + (y  ) * width]) + 
					(sobel_x[1][1] * input[x   + (y  ) * width]) + 
					(sobel_x[1][2] * input[x+1 + (y  ) * width]) +
					(sobel_x[2][0] * input[x-1 + (y+1) * width]) + 
					(sobel_x[2][1] * input[x   + (y+1) * width]) + 
					(sobel_x[2][2] * input[x+1 + (y+1) * width])
					);
			const int pixel_y = (int) (
					(sobel_y[0][0] * input[x-1 + (y-1) * width]) + 
					(sobel_y[0][1] * input[x   + (y-1) * width]) + 
					(sobel_y[0][2] * input[x+1 + (y-1) * width]) +
					(sobel_y[1][0] * input[x-1 + (y  ) * width]) + 
					(sobel_y[1][1] * input[x   + (y  ) * width]) + 
					(sobel_y[1][2] * input[x+1 + (y  ) * width]) +
					(sobel_y[2][0] * input[x-1 + (y+1) * width]) + 
					(sobel_y[2][1] * input[x   + (y+1) * width]) + 
					(sobel_y[2][2] * input[x+1 + (y+1) * width])
					);

			float magnitude = sqrt((float)(pixel_x * pixel_x + pixel_y * pixel_y));

			if (magnitude < 0){ magnitude = 0; }
			if (magnitude > 255){ magnitude = 255; }

			output[x + y * width] = magnitude;
		}
	}
}



