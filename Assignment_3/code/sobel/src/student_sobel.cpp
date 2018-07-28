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
            for(int z=0;z<3;z++)               
            {                       
    S1+=(Gx.at<int>(k,z)*(int)src.at<uint8_t>(i+k,j+z));   
    S2+=(Gy.at<int>(k,z)*(int)src.at<uint8_t>(i+k,j+z));   
            }                       
        }                        //end
        dst.at<uint8_t>(i,j)=(uint8_t)sqrt(S1*S1+S2*S2);
        //cout<<i<<","<<j<<",,,"<<(int)(abs(S1*S1)+abs(S2*S2))<<endl;
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
	{   int z=0;
    	S1+=(Gx.at<int>(k,z)*(int)src.at<uint8_t>(i+k,j+z+0));
	S1+=(Gx.at<int>(k,z+1)*(int)src.at<uint8_t>(i+k,j+z+1)); 
	S1+=(Gx.at<int>(k,z+2)*(int)src.at<uint8_t>(i+k,j+z+2)); 
	//S1+=(Gx.at<int>(k+3,z+3)*(int)src.at<uint8_t>(i+k+3,j+z+3));    
	S2+=(Gy.at<int>(k,z+0)*(int)src.at<uint8_t>(i+k,j+z+0));   
 	S2+=(Gy.at<int>(k,z+1)*(int)src.at<uint8_t>(i+k,j+z+1)); 					 	 S2+=(Gy.at<int>(k,z+2)*(int)src.at<uint8_t>(i+k,j+z+2)); 
  	//S2+=(Gy.at<int>(k+3,z+3)*(int)src.at<uint8_t>(i+k+3,j+z+3)); 
        
        dst.at<uint8_t>(i,j)=(uint8_t)sqrt(S1*S1+S2*S2);
        //cout<<i<<","<<j<<",,,"<<(int)(abs(S1*S1)+abs(S2*S2))<<endl;
	}
    }
}


}//end void;

void sobel_neon(const Mat& src, Mat& dst)
{
    // TODO
}
