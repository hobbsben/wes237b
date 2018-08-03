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


int sobel_row1[4] = {1, 0, -1, 0};
int sobel_row2[4] = {2,  0, -2, 0}; 
int sobel_row3[4] = {1,  0, -1, 0};

int sobel_row1t[4] = {1, 2, 1, 0};
int sobel_row2t[4] = {0,  0, 0, 0}; 
int sobel_row3t[4] = {-1,  -2, -1, 0};
  


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

Gy=Gx.t();   

for(int i=0;i<=HEIGHT-3;i++)
{
	for(int j=0;j<=HEIGHT-3;j++)
	{
            S1=0;
            S2=0;                           
		for(int k=0;k<3;k++)
		{   
			for(int z=0; z<3; z++)
			{
    	S1+=(Gx.at<int>(k,z)*(int)src.at<uint8_t>(i+k,j+z));
	S2+=(Gy.at<int>(k,z)*(int)src.at<uint8_t>(i+k,j+z)); 
			}
		}
	 dst.at<uint8_t>(i,j)=(uint8_t)sqrt(S1*S1+S2*S2);
	} 
}

}//end void;

void sobel_unroll(const Mat& src, Mat& dst)
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
Gy=Gx.t();   

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
 	S2+=(Gy.at<int>(k,z+1)*(int)src.at<uint8_t>(i+k,j+z+1)); 		
	S2+=(Gy.at<int>(k,z+2)*(int)src.at<uint8_t>(i+k,j+z+2)); 
		
	dst.at<uint8_t>(i,j)=(uint8_t)sqrt(S1*S1+S2*S2);	
		}
	}
   }


}//end void unrolled;


void sobel_neon(const Mat& src, Mat& dst)
{


Mat Gx(3,3,CV_32S);
Mat Gy(3,3,CV_32S);
Mat src_all(3,3,CV_32S);
src.convertTo(src_all,CV_32S);
int HEIGHT=src.rows;
int WIDTH=src.cols;
int src_row1[4]={0,0,0,0};
int src_row2[4]={0,0,0,0};
int src_row3[4]={0,0,0,0};


int S1,S2=0;

for(int i=0; i<3; i++)
{
    for(int j=0; j<3; j++)
    {
    Gx.at<int>(i,j)=sobel_kernel_x[i][j];//putting sobel_kernel_x into Mat Gx
    }
}

Gy=Gx.t();   

  int init_zeros[4] = {0, 0, 0, 0};
  int out_sum[4] = {0, 0, 0, 0};

 
    int32x4_t Gx_32x1 = vld1q_s32(init_zeros);
    int32x4_t Gy_32x1 = vld1q_s32(init_zeros);
    int32x4_t Gx_32x2 = vld1q_s32(init_zeros);
    int32x4_t Gy_32x2 = vld1q_s32(init_zeros);
    int32x4_t Gx_32x3 = vld1q_s32(init_zeros);
    int32x4_t Gy_32x3 = vld1q_s32(init_zeros);
    int32x4_t src_32x = vld1q_s32(init_zeros);
    int32x4_t sobel_32x = vld1q_s32(init_zeros);
    int32x4_t S1_32x = vld1q_s32(init_zeros);
    int32x4_t S2_32x = vld1q_s32(init_zeros);
 
    for(int i=0; i<HEIGHT-3; i+=1)
    {
        S1_32x = vld1q_s32(init_zeros);
        S2_32x = vld1q_s32(init_zeros);
 
	for(int j=0; j<WIDTH-3;j+=1)
        {

		for(int k=0; k<3;k++)
		{
			int z=0;
			
		src_row1[1]=(int)src.at<uint8_t>(i+k,j+z+1);
		src_row1[2]=(int)src.at<uint8_t>(i+k,j+z+2);
		src_row1[3]= 0;
		
		src_row2[0]=(int)src.at<uint8_t>(i+k,j+z+0+1);
		src_row2[1]=(int)src.at<uint8_t>(i+k,j+z+1+1);
		src_row2[2]=(int)src.at<uint8_t>(i+k,j+z+2+1);
		src_row2[3]= 0;	
		src_row3[0]=(int)src.at<uint8_t>(i+k,j+z+0+2);
		src_row3[1]=(int)src.at<uint8_t>(i+k,j+z+1+2);
		src_row3[2]=(int)src.at<uint8_t>(i+k,j+z+2+2);
		src_row3[3]= 0;
		
		//1
		src_32x = vld1q_s32(src_row1 + k); 
		sobel_32x = vld1q_s32(sobel_row1 + k); 
		S1_32x = vmlaq_s32(S1_32x, sobel_32x, src_32x);

		sobel_32x = vld1q_s32(sobel_row1t + k); 
		S2_32x = vmlaq_s32(S2_32x, sobel_32x, src_32x);

		int32x4_t prodS1 = vmulq_s32(S1_32x, S1_32x);
		int32x4_t prodS2 = vmulq_s32(S2_32x, S2_32x);
		int32x4_t sum = vaddq_s32(prodS1, prodS2);		
		//2
		src_32x = vld1q_s32(src_row2 + k); 
		sobel_32x = vld1q_s32(sobel_row2 + k); 
		S1_32x = vmlaq_s32(S1_32x, sobel_32x, src_32x);

		sobel_32x = vld1q_s32(sobel_row2t + k); 
		S2_32x = vmlaq_s32(S2_32x, sobel_32x, src_32x);

		 prodS1 = vmulq_s32(S1_32x, S1_32x);
		 prodS2 = vmulq_s32(S2_32x, S2_32x);
		 sum = vaddq_s32(prodS1, prodS2);
		//3
		src_32x = vld1q_s32(src_row3 + k); 
		sobel_32x = vld1q_s32(sobel_row3 + k); 
		S1_32x = vmlaq_s32(S1_32x, sobel_32x, src_32x);

		sobel_32x = vld1q_s32(sobel_row3t + k); 
		S2_32x = vmlaq_s32(S2_32x, sobel_32x, src_32x);

		 prodS1 = vmulq_s32(S1_32x, S1_32x);
		 prodS2 = vmulq_s32(S2_32x, S2_32x);
		 sum = vaddq_s32(prodS1, prodS2);
		
		vst1q_s32(out_sum,sum);
		
 	    }
	//out_sum.converTo(out_sum,CV_32S);
	
	dst.at<uint8_t>(i,j+0) = (uint8_t)sqrt(out_sum[0]);
	dst.at<uint8_t>(i,j+1) = (uint8_t)sqrt(out_sum[1]);
	dst.at<uint8_t>(i,j+2) = (uint8_t)sqrt(out_sum[2]);
	//dst.at<uint8_t>(i,j+3) = (uint8_t)sqrt(out_sum[3]);        


	}

   } 
}// void end neon

