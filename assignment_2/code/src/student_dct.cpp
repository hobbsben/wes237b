#include "main.h"
#include "math.h"
//#ifndef min
//#define min(a,b)	((((a)<(b))? (a):(b))
#include "stdio.h"

using namespace cv;


Mat matx;
Mat maty;

// Helper function
float sf(int in){
	if (in == 0)
		return 0.70710678118; // = 1 / sqrt(2)
	return 1.;
}

// Initialize LUT
void initDCT(int WIDTH, int HEIGHT)
{
	HEIGHT=WIDTH;
	int Nx, Ny = HEIGHT;
        matx.create(HEIGHT,WIDTH,CV_32FC1);
        maty.create(HEIGHT,WIDTH,CV_32FC1);
	
	
	float* matx_ptr = matx.ptr<float>();
	float* maty_ptr = maty.ptr<float>();

	for(int kx=0;kx<HEIGHT;kx++)//k is the resolution which is going to be 0 to N;
        {
            for(int x=0;x<HEIGHT;x++)
                {
	//matx.at<float>(kx,x)=sf(kx)*cos(M_PI/((float)HEIGHT)*(x+1./2.)*(float)kx);
	matx_ptr[kx*HEIGHT+x]=sf(kx)*sqrt(2./HEIGHT)*cos((M_PI/((float)HEIGHT))*(x+1./2.)*(float)kx);
                }
        }

	for(int ky=0;ky<WIDTH;ky++)
        {
            for(int y=0;y<WIDTH;y++)
                {
        //maty.at<float>(ky,y)=sf(ky)*cos(M_PI/((float)WIDTH)*(y+1./2.)*(float)ky);
	maty_ptr[ky*WIDTH+y]=sf(ky)*sqrt(2./WIDTH)*cos(M_PI/((float)WIDTH)*(y+1./2.)*(float)ky);
                }
        }
}

/*
// Baseline: O(N^4)
Mat student_dct(Mat input)
{
	const int HEIGHT = input.rows;
	const int WIDTH  = input.cols;
	float scale = 2./sqrt(HEIGHT*WIDTH);
	int kx=0;
	int ky=0;
	int x=0;
	int y=0;
	Mat result = Mat(HEIGHT, WIDTH, CV_32FC1);

	// Note: Using pointers is faster than Mat.at<float>(x,y)
	// Try to use pointers for your LUT as well
	float* result_ptr = result.ptr<float>();
	float* input_ptr  = input.ptr<float>();
	float* matx_ptr   = matx.ptr<float>();
	float* maty_ptr   = maty.ptr<float>();
	for(int kx = 0; kx < HEIGHT; kx++)
	{
		for(int ky = 0; ky < WIDTH; ky++)
		{
			float value = 0.f;

			for(int x = 0; x < HEIGHT; x++)
			{
				for(int y = 0; y < WIDTH; y++)
				{
					value += input_ptr[x * WIDTH + y]
					//*matx_ptr[kx* HEIGHT + x]                     // part 1 LUT
					//*matx_ptr[ky * WIDTH + y];                    // part 1 LUT
					//*matx.at<float>(kx,x)
					//*maty.at<float>(ky,y);
					* cos(M_PI/((float)HEIGHT)*(x+1./2.)*(float)kx) // part 0
					* cos(M_PI/((float)WIDTH)*(y+1./2.)*(float)ky); // part 0
					
				}
			}
			// TODO
			// --- Incorporate the scale in the LUT coefficients ---
			// --- and remove the line below ---
			value = scale * sf(kx) * sf(ky) * value;  //part 0
		        
			result_ptr[kx * WIDTH + ky] = value;
		}
	}

	return result;
}
*/
//end of baseline

// DCT as matrix multiplication
Mat student_dct(Mat input)
{
	// -- Works only for WIDTH == HEIGHT
	assert(input.rows == input.cols);

	// -- Matrix multiply with OpenCV
	//Mat output = matx * input * maty.t();
	//output+=(1/8)*output;
//end of second DCT


int HEIGHT = input.rows;
int WIDTH  = input.cols;
Mat output(HEIGHT,WIDTH,CV_32FC1,0.0);
Mat C(HEIGHT,WIDTH,CV_32FC1,0.0);

int blocksize=8;
//float* output_ptr  = output.ptr<float>();//float* input_ptr   = input.ptr<float>();
//float* matx_ptr    = matx.ptr<float>();//float* maty_ptr    = maty.ptr<float>();



for(int k=0; k <HEIGHT+blocksize; k=k+blocksize)
{
	for(int j=0; j<WIDTH+blocksize; j=j+blocksize)
    	{
		for(int i=0; i<HEIGHT;i++)
		{
		
			for(int jj =j; jj<min(j+blocksize,HEIGHT); jj++)
            		{
               		 	for(int kk=k; kk<min(k+blocksize,WIDTH); kk++)
                 		{
		
                 		   C.at<float>(i,jj)+=matx.at<float>(i,kk)*input.at<float>(kk,jj);
              	 		}
        
            		}
        	}

    	}
}

Mat Transpose=maty.t();

for(int k=0; k <HEIGHT+blocksize; k=k+blocksize)
{
	for(int j=0; j <WIDTH+blocksize; j=j+blocksize)
    {
		for(int i=0; i<HEIGHT;i++)
		{
			for(int jj=j; jj<min(j+blocksize,HEIGHT); jj++)
            		{
                		for(int kk=k; kk<min(k+blocksize,WIDTH); kk++)
               	     		{
				
            			  output.at<float>(i,jj)+=C.at<float>(i,kk)*Transpose.at<float>(kk,jj);
               			 }

                    	 }
                 }

    }
}

return output;  // for the 2nd and 3rd DCTs
}


