#include "filter.h"
#include "cuda_timer.h"

__global__
void kernel_filter(const uchar * input, uchar * output, const uint height, const uint width)
{
	// TODO: Implement a blur filter for the camera (averaging an NxN array of pixels
 //printf("%d\n", threadIdx.x);
//****************************************************************
 int idx = blockIdx.x * blockDim.x + threadIdx.x;
 int idy = blockIdx.y * blockDim.y + threadIdx.y;
  if (idx > width -3 || idy > height-3 || idx < 2 || idy < 2) {
      return;
  }
  
  float c = 0.0f;

// printf("/n/n output: %0.2c", output[idx+idy]; 

	for (int i = -2; i <= 2; i++) 
	{
		for (int j = -2; j <= 2; j++)
		{
     		 c += input[(idy+i)*width +idx+j];
               
   		 }
		
  	}
if(c > 255*25){c=6375;} 
output[(idy)*width +idx] =c/25.0f;    //calculate the average by dividing by 0

// check to see if pixel is greater than 0
if(c/25 > 255 || c/25 < 0)
	{	
	printf(" %.2f \n",c/25);
	}
//****************************************************************

}//end kernel_filter 
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
void filter_gpu(const uchar * input, uchar * output, const uint height, const uint width)
{
       // printf("\n\n testing filter_gpu function /n/n");

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
        //printf("\n\n testing filter_gpu function /n/n");
	kernel_filter<<<grid, block>>>(input, output, height, width);
	timer.stop();
//        printf("\n\n testing filter_gpu function /n/n");
	cudaDeviceSynchronize();

	float time_kernel = timer.getElapsed();
}





