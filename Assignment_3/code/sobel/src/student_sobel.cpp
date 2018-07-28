#include "sobel.h"
#include <stdlib.h>
#include <iostream>
#include <stdlib.h>
using namespace std;
using namespace cv;


int sobel_kernel_x[3][3] = {
    { 1,  0, -1},
    { 2,  0, -2},
    { 1,  0, -1}};



void sobel(const Mat& src, Mat& dst)
{
    //int S1,S2=0;

//Mat Gx(3,3,CV_32FC1,0.0);//basically just renaming sobel_kernel_x here and putting it into MAT.
//Mat Gy(3,3,CV_32FC1,0.0);
Mat Gx(3,3,CV_32S);
Mat Gy(3,3,CV_32S);
int HEIGHT=src.rows;
int WIDTH=src.cols;
//Mat mag(HEIGHT,WIDTH,CV_32FC1,0.0);//holding magnitude of S1,S2;
//Mat src_mat(HEIGHT+3,HEIGHT+3,CV_32FC1,0.0);
//Mat dst_mat(HEIGHT+3,HEIGHT+3,CV_32FC1,0.0);
Mat src_mat(HEIGHT,WIDTH,CV_32S,0.0);
Mat dst_mat(HEIGHT,WIDTH,CV_32S,0.0);
//uint16_t S1,S2=0;
int S1,S2=0;

for(int i=0; i<3; i++)
{
    for(int j=0; j<3; j++)
    {
    Gx.at<int>(i,j)=sobel_kernel_x[i][j];//putting sobel_kernel_x into Mat Gx
    }
}
//cout<<Gx;

// CANT convert int-> uint.
//CAN convert uint->int.

for(int i=0; i<HEIGHT; i++)
{
    for(int j=0; j<WIDTH; j++)
    {
    src_mat.at<int>(i,j)=(int)src.at<uint8_t>(i,j);
    //cout<<src_mat.at<int>(i,j)<<endl;
    }
}

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
            for(int z=0;z<3;z++)               
            {                       
    //S1+=(Gx.at<int>(k,z)*(int)src.at<uint8_t>(i+k,j+z));   
    //S2+=(Gy.at<int>(k,z)*(int)src.at<uint8_t>(i+k,j+z));   
    S1+=Gx.at<int>(k,z)*src_mat.at<int>(i+k,j+z);   
    S2+=Gy.at<int>(k,z)*src_mat.at<int>(i+k,j+z);
            }                       
        }                        //end
        //dst_mat.at<int>(i,j)=(int)sqrt(S1*S1+S2*S2);
        dst.at<uint8_t>(i,j)=(uint8_t)sqrt(S1*S1+S2*S2);
        //cout<<i<<","<<j<<",,,"<<(int)(abs(S1*S1)+abs(S2*S2))<<endl;
    }
}
//cout<<dst_mat<<endl;
for(int i=0;i<HEIGHT-3;i++)
{
    for(int j=0;j<HEIGHT-3;j++)
    {
    //dst_mat.convertTo(dst_mat,CV_8U);
   
    //dst.at<uint8_t>(i,j)=dst_mat.at<uint8_t>(i,j);
    //dst.at<uint8_t>(i,j)=dst_mat.at<uint8_t>(i,j);
    //cout<<(int)dst_mat.at<uint8_t>(i,j)<<endl;   
   
    }
}
//cout<<dst<<endl;

}//end void;

/*
void sobel(const Mat& src, Mat& dst)
{
    // TODO
}
*/

void sobel_unroll(const Mat& src, Mat& dst)
{
    // TODO
}

void sobel_neon(const Mat& src, Mat& dst)
{
    // TODO
}
