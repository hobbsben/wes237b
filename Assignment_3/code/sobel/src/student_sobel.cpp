
#include "sobel.h"

using namespace std;
using namespace cv;


int sobel_kernel_x[3][3] = {
	{ 1,  0, -1},
	{ 2,  0, -2},
	{ 1,  0, -1}};


void sobel(const Mat& src, Mat& dst)
{
	// TODO
}


void sobel_unroll(const Mat& src, Mat& dst)
{
	// TODO
}

void sobel_neon(const Mat& src, Mat& dst)
{
	// TODO
}

