#include "main.h"
#include "math.h"


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
        matx.create(HEIGHT,WIDTH,CV_32FC1);
        maty.create(HEIGHT,WIDTH,CV_32FC1);
	int Nx, Ny = HEIGHT;


	//Mat matx(HEIGHT,WIDTH,CV_32FC1); //this makes the output all black. ERROR.
	//Mat maty(HEIGHT,WIDTH,CV_32FC1); // KILLS CODE
	float* matx_ptr = matx.ptr<float>();
	float* maty_ptr = maty.ptr<float>();

	for(int kx=0;kx<HEIGHT;kx++)//k is the resolution which is going to be 0 to N;
        {
            for(int x=0;x<HEIGHT;x++)
                {
	//matx.at<float>(kx,x)=sf(kx)*cos(M_PI/((float)HEIGHT)*(x+1./2.)*(float)kx);
	matx_ptr[kx*HEIGHT+x]=sf(kx)*(1./sqrt(HEIGHT))*cos((M_PI/((float)HEIGHT))*(x+1./2.)*(float)kx);
                }
        }

	for(int ky=0;ky<WIDTH;ky++)
        {
            for(int y=0;y<WIDTH;y++)
                {
        //maty.at<float>(ky,y)=sf(ky)*cos(M_PI/((float)WIDTH)*(y+1./2.)*(float)ky);
	maty_ptr[ky*WIDTH+y]=sf(ky)*(1./sqrt(WIDTH))*cos(M_PI/((float)WIDTH)*(y+1./2.)*(float)ky);
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
					*matx_ptr[kx* HEIGHT + x]
					*matx_ptr[ky * WIDTH + y];
					//*matx.at<float>(kx,x)
					//*maty.at<float>(ky,y);
					//* cos(M_PI/((float)HEIGHT)*(x+1./2.)*(float)kx)
					//* cos(M_PI/((float)WIDTH)*(y+1./2.)*(float)ky);
					
				}
			}
			// TODO
			// --- Incorporate the scale in the LUT coefficients ---
			// --- and remove the line below ---
			//value = scale * sf(kx) * sf(ky) * value;
		        
			result_ptr[kx * WIDTH + ky] = 2*value;
		}
	}

	return result;
}
*/
// DCT as matrix multiplication

Mat student_dct(Mat input)
{
	// -- Works only for WIDTH == HEIGHT
	assert(input.rows == input.cols);

	// -- Matrix multiply with OpenCV
	Mat output = matx * input * maty.t();
	output+=output;

	// TODO
	// Replace the line above by your own matrix multiplication code
	// You can use a temp matrix to store the intermediate result

	return output;
}

