
#include "sobel.h"

using namespace std;
using namespace cv;


int sobel_kernel_x[3][3] = {
	{ 1,  0, -1},
	{ 2,  0, -2},
	{ 1,  0, -1}};



Mat Gx(3,3, CV_32FC1, &sobel_kernel_x);

void sobel(const Mat& src, Mat& dst)
{
	//int S1,S2=0;
int rows=src.rows;
int cols=src.cols;
Mat S1(3,3,CV_32FC1);
Mat S2(rows,cols,CV_32FC1);
//float S1,S2;
Mat Gy(3,3,CV_32FC1);
Gy=Gx.t();
Mat mag(rows,cols,CV_32FC1);
	
for(int i=0; i<rows-2; i++)
{
	for(int j=0; j<cols-2; j++)
	{
	S1=Gx**src.at<float>(Range(i,i+2),Range(j,j+2));
	//S1+=Gx.*src;
	//S1+=Gx.*src;
	//S2+=Gy.*src;
	//S2+=Gy.*src;
	//mag(i+1,j+1)=sqrt(S1*S1+S2*S2);
	}
}
}//end void;

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

