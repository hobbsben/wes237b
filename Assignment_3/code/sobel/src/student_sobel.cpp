#include "sobel.h"
#include <stdlib.h>
#include <iostream>
#include <stdlib.h>
//#include <arm_neon.h>
using namespace std;
using namespace cv;


int sobel_kernel_x[3][3] = {
    { 1,  0, -1},
    { 2,  0, -2},
    { 1,  0, -1}};


void sobel(const Mat& src, Mat& dst)
{


Mat Gx(3,3,CV_32S);
Mat Gy(3,3,CV_32S);
int HEIGHT=src.rows;
int WIDTH=src.cols;

int S1,S2=0;

for(int i=0; i<3; i++)
{
    for(int j=0; j<3; j++)
    {
    Gx.at<int>(i,j)=sobel_kernel_x[i][j];//putting sobel_kernel_x into Mat Gx
    }
}
//cout<<Gx;

//cout<<src<<endl;
Gy=Gx.t();   
//cout<<Gy<<endl;
for(int i=0;i<=HEIGHT-3;i++)
{
	for(int j=0;j<=HEIGHT-3;j++)
	{
            S1=0;
            S2=0;                           
		for(int k=0;k<3;k++)
		{   
		int z=0;
    	S1+=(Gx.at<int>(k,z+0)*(int)src.at<uint8_t>(i+k,j+z+0));
	S1+=(Gx.at<int>(k,z+1)*(int)src.at<uint8_t>(i+k,j+z+1)); 
	S1+=(Gx.at<int>(k,z+2)*(int)src.at<uint8_t>(i+k,j+z+2)); 

	S2+=(Gy.at<int>(k,z+0)*(int)src.at<uint8_t>(i+k,j+z+0));   
 	S2+=(Gy.at<int>(k,z+1)*(int)src.at<uint8_t>(i+k,j+z+1)); 					 	 S2+=(Gy.at<int>(k,z+2)*(int)src.at<uint8_t>(i+k,j+z+2)); 

        dst.at<uint8_t>(i,j)=(uint8_t)sqrt(S1*S1+S2*S2);
        //cout<<i<<","<<j<<",,,"<<(int)(abs(S1*S1)+abs(S2*S2))<<endl;
		}	
	}
    
}


}//end void;

/*
void sobel(const Mat& src, Mat& dst)
{
    // TODO
}
*/

void sobel_unroll(const Mat& src, Mat& dst)
{

/*
Mat Gx(3,3,CV_32S);
Mat Gy(3,3,CV_32S);
int HEIGHT=src.rows;
int WIDTH=src.cols;

int S1,S2=0;

for(int i=0; i<3; i++)
{
    for(int j=0; j<3; j++)
    {
    Gx.at<int>(i,j)=sobel_kernel_x[i][j];//putting sobel_kernel_x into Mat Gx
    }
}
//cout<<Gx;

//cout<<src<<endl;
Gy=Gx.t();   
//cout<<Gy<<endl;
  for(int i=0;i<=HEIGHT-3;i++)
  {
	for(int j=0;j<=HEIGHT-3;j++)
	{
            S1=0;
            S2=0;                           
		for(int k=0;k<3;k++)
		{   
		int z=0;
    	S1+=(Gx.at<int>(k,z+0)*(int)src.at<uint8_t>(i+k,j+z+0));
	S1+=(Gx.at<int>(k,z+1)*(int)src.at<uint8_t>(i+k,j+z+1)); 
	S1+=(Gx.at<int>(k,z+2)*(int)src.at<uint8_t>(i+k,j+z+2)); 

	S2+=(Gy.at<int>(k,z+0)*(int)src.at<uint8_t>(i+k,j+z+0));   
 	S2+=(Gy.at<int>(k,z+1)*(int)src.at<uint8_t>(i+k,j+z+1)); 					 	 S2+=(Gy.at<int>(k,z+2)*(int)src.at<uint8_t>(i+k,j+z+2)); 

        //cout<<i<<","<<j<<",,,"<<(int)(abs(S1*S1)+abs(S2*S2))<<endl;
		}
	dst.at<uint8_t>(i,j)=(uint8_t)sqrt(S1*S1+S2*S2);	
	}
   }

*/
}//end void unrolled;


void sobel_neon(const Mat& src, Mat& dst)
{
/*
  int init_zeros[4] = {0, 0, 0, 0};
  uint8_t out_sum[8] = {0, 0, 0, 0, 0, 0, 0, 0};
 
    int16x4_t Gx_32x = vld1q_s16(init_zeros);
    int16x4_t Gy_32x = vld1q_s16(init_zeros);
    uint8x8_t in_32x = vld1q_u8(init_zeros);
    int16x4_t S1_32x = vld1q_s16(init_zeros);
    int16x4_t S2_32x = vld1q_s16(init_zeros);
 
    for(int i=0; i<HEIGHT; i++)
    {
        temp = vld1q_f32(init_zeros);
 
	for(int j=0; j<WIDTH;j+=1)
        {
		for(int k=0; k<3;k++)
		{
		Int z=0;
		Gx_32x = vld1q_s16(Gx + (k*3) + z);
		Gy_32x = vld1q_s16(Gy + (k*3) + z);
		in_32x = vld1q_u8(src + (i*HEIGHT) + j);
		S1_32x = vmlaq_s16(S1_32x, Gx_32x, (int)in_32x4);
		S2_32x = vmlaq_s16(S1_32x, Gy_32x, (int)in_32x4);
		int16x4_t prodS1 = vmulq_s16(S1_32x, S1_32x);
		int16x4_t prodS2 = vmulq_s16(S1_32x, S1_32x);
		int16x4_t sum = vaddq_s16(prodS1, prodS2);
		vst1q_u8(out_sum,sum);

 		}
	//vst1q_f32(output_sum, temp);
	dst[i] = out_sum[0] + out_sum[1] + out_sum[2] + out_sum[3];
        }

   } */
}// void end neon

