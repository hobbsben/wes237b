#include "sobel.h"
#include <math.h>
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

int sobel_kernel_t[3][3] = {
    { 1,2,1},
    {0,0,0},
    { -1,-2,-1}};

int sobel_kernel_4x4[4][4] = {
    { 1,  0, -1, 0},
    { 2,  0, -2, 0},
    { 1,  0, -1, 0}};




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
			for(int z=0; z<3; z++)
			{
    	S1+=(Gx.at<int>(k,z)*(int)src.at<uint8_t>(i+k,j+z));
	S2+=(Gy.at<int>(k,z)*(int)src.at<uint8_t>(i+k,j+z)); 

        dst.at<uint8_t>(i,j)=(uint8_t)sqrt(S1*S1+S2*S2);
        //cout<<i<<","<<j<<",,,"<<(int)(abs(S1*S1)+abs(S2*S2))<<endl;
			}
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
// /*
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
 	S2+=(Gy.at<int>(k,z+1)*(int)src.at<uint8_t>(i+k,j+z+1));
	S2+=(Gy.at<int>(k,z+2)*(int)src.at<uint8_t>(i+k,j+z+2)); 

        //cout<<i<<","<<j<<",,,"<<(int)(abs(S1*S1)+abs(S2*S2))<<endl;
		}
	dst.at<uint8_t>(i,j)=(uint8_t)sqrt(S1*S1+S2*S2);	
	}
   }

// */
}//end void unrolled;

/*****************Pat Sobel******************/ 
void sobel_neon(const Mat& src, Mat& dst)  
{
int vert_sobel1[4] = { 1,  0, -1, 0};
int vert_sobel2[4] = { 2,  0, -2, 0};
int vert_sobel3[4] = { 1,  0, -1, 0};

// initialize horizontal sobel filter and store as neon vars
short int hor_sobel1[4] = { 1,  0, -1, 0};
short int hor_sobel2[4] = { 2,  0, -2, 0};
short int hor_sobel3[4] = { 1,  0, -1, 0};
int16x4_t sobel_row1 = vld1_s16(hor_sobel1);
int16x4_t sobel_row2 = vld1_s16(hor_sobel2);
int16x4_t sobel_row3 = vld1_s16(hor_sobel3);

int accumulate_row1 = 0;
int accumulate_row2 = 0;
int accumulate_row3 = 0;

int HEIGHT = src.rows;
int WIDTH = src.cols;

short int zeros[4] = { 0,  0, 0, 0};	
int16x4_t neon_source_row1 = vld1_s16(zeros);
int16x4_t neon_source_row2 = vld1_s16(zeros);
int16x4_t neon_source_row3 = vld1_s16(zeros);

// initialize neon zero
int16x4_t neon_zero = vld1_s16(zeros);

//ionitalize accumulator
int16x4_t accumulate_row = vld1_s16(zeros);

short int src_row1[4] = {0,0,0,0};
short int src_row2[4] = {0,0,0,0};
short int src_row3[4] = {0,0,0,0}; 

// convert src to 16 bit integer:
Mat src_16;
src_16 = Mat(HEIGHT, WIDTH, CV_16SC1);
src.convertTo(src_16, CV_16SC1);

const short int *src_16_ptr;
src_16_ptr = src_16.ptr<short int>(0);  // initalize Mat pointer for src

short int accum[4] = {0,0,0,0};

Mat sobel_t_mat;
sobel_t_mat= (Mat_<short int>(3, 3) <<  1,2,1, 0,0,0, -1,-2,-1);


Mat accum_t;
accum_t = (Mat_<short int>(3, 3) << 0,0,0,0);
short int accum_t_short = 0;


short int accum_total = 0;
short int accum_t_total = 0;

short int accum_r = 0;


	for(int i=0; i < HEIGHT; i++)
    	{
        
		for(int j=0; j < WIDTH;j++)
		{
			// input 
			neon_source_row1 = vld1_s16(src_16_ptr + i + WIDTH * j);	
	                neon_source_row2 = vld1_s16(src_16_ptr + i + WIDTH * (j+1));		   				neon_source_row3 = vld1_s16(src_16_ptr + i + WIDTH * (j+2));	

			// 2d convolution of 3x4 source block and sobel kernel
			//accumMat Gy(3,3,CV_32S);ulate_row = vmla_s16(neon_zero, sobel_row1,  neon_source_row1);
			accumulate_row = vmla_s16(accumulate_row, sobel_row2, neon_source_row2);
			accumulate_row = vmla_s16(accumulate_row, sobel_row3, neon_source_row3);

			vst1_s16(accum, accumulate_row);
						 			
			// accumulate with transposed sobel filter
			accum_t_short +=sobel_t_mat.at<int>(i,j)*(int)src.at<uint8_t>(i,j);  
 			accum_t_short+=sobel_t_mat.at<int>(i,j+1)*(int)src.at<uint8_t>(i,j+1);  
			accum_t_short+=sobel_t_mat.at<int>(i,j+2)*(int)src.at<uint8_t>(i,j+2);  

			accum_t_short+=sobel_t_mat.at<int>(i+2,j)*(int)src.at<uint8_t>(i+2,j); 				accum_t_short+=sobel_t_mat.at<int>(i+2,j+1)*(int)src.at<uint8_t>(i+2,j+1);
			accum_t_short+=sobel_t_mat.at<int>(i+2,j+2)*(int)src.at<uint8_t>(i+2,j+2);  		
			
			accum_r = accum[0]+accum[1]+accum[2]+accum[3];
			
			
			dst.at<uint8_t>(i,j)=(uint8_t)sqrt((double)accum_r * (double)accum_t_short + (double)accum_t_short*(double)accum_r);
       		     

			/*src_row1[0] = (int)src(i+WIDTH*(j)); 
			 j));
			src_row1[2] = src(2+i+WIDTH*(j));
			src_row1[3] = src(3+i+WIDTH*(j));

			src_row2[0] = src(i+WIDTH*(j+1));
			src_row2[1] = src(1+i+WIDTH*(j+1));
			src_row2[2] = src(2+i+WIDTH*(j+1));
			src_row2[3] = src(3+i+WIDTH*(j+1));

			src_row3[0] = src(i+WIDTH*(j+2));
			src_row3[1] = src(i+1+WIDTH*(j+2));
			src_row3[2] = src(i+2+WIDTH*(j+2));
			src_row3[3] = src(i+3+WIDTH*(j+2));

			// store source blocks into neon vars
			neon_source_row1 = vld1_s16(src_row1);
			neon_source_row2 = vld1_s16(src_row2);
			neon_source_row3 = vld1_s16(src_row3);*/	
				
                       
					
		}
	}
 
}



/*
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
//}    //  void end neon

