//
//  main.cpp
//  Lab2
//
//  Created by Alireza on 7/6/17.
//  Copyright © 2017 Alireza. All rights reserved.
//

#include "main.h"
#include "cycletime.h"
#include "timer.h"

using namespace std;
using namespace cv;

int main(int argc, const char * argv[])
{       
        unsigned int opencv_cachemisscount;
	unsigned int c_start;
	unsigned int opencv_c, student_c;

	cout << "WES237B HW 3 " << endl;

	VideoCapture cap("input.raw");

	Mat frame, gray, dct_org, dct_student, diff_img;
	char key=0;
	float mse;
	int fps_cnt = 0;

	int WIDTH  = 64;
	int HEIGHT = 64;

	// 1 argument on command line: WIDTH = HEIGHT = arg
	if(argc >= 2)
	{
		WIDTH = atoi(argv[1]);
		HEIGHT = WIDTH;
	}
	// 2 arguments on command line: WIDTH = arg1, HEIGHT = arg2
	if(argc >= 3)
	{
		HEIGHT = atoi(argv[2]);
	}


	initDCT(WIDTH, HEIGHT);

	float avg_perf = 0.f;
	int count = 0;
        int i_start = 0;
         //configure register 0x0 to read PMUEVENT[48] (table A.8.1 of ARM Cortex-A9 rev. r4p1, available on piazza)   // from lab 3
        //  cache_config(0x0, 0x90/*event_number*/);   // from lab3
        cache_config(0x0, 0x03/*event_number*/);   // (software increment, data cache miss)
        
	int cache_start = 0;
        int cache_start2 = 0; 
	while(key != 'q')
	{
		cap >> frame;
		if(frame.empty()){ break; }

		cvtColor(frame, gray, CV_BGR2GRAY);
		resize(gray, gray, Size(WIDTH, HEIGHT));
		gray.convertTo(gray, CV_32FC1);

		init_counters(1, 0);

		cache_start = cache_read(0x0);
		// OpenCV DCT
		c_start = get_cyclecount();              // read cycle count 
		dct(gray, dct_org);
		opencv_c = get_cyclecount() - c_start;   // get cycle count again and calculate difference
	        //opencv_cachemisscount = cache_read(0x03);  // call cash miss count reading function from cycletime.h
               // cout << "\n OpenCV method Cache Misses: " << opencv_cachemisscount/*- i_start */ <<
		cout << "OpenCV Cache Miss Count: " <<  cache_read(0x0) - cache_start /*- i_start*/ << endl;
                 
		cache_start2 = cache_read(0x0);
		// Your DCT
		LinuxTimer t;
		c_start = get_cyclecount();
		dct_student = student_dct(gray);              
		cout << "Our DCT Cache Miss Count: " <<  cache_read(0x0) - cache_start2 << "\n" /*- i_start*/ << endl;
		
		student_c = get_cyclecount() - c_start;
		t.stop();
		float myTimer = t.getElapsed();

		gray.convertTo(gray, CV_8UC1);

		absdiff(dct_org, dct_student, diff_img); 

		/* calculating RMSE */
		diff_img = diff_img.mul(diff_img);
		Scalar se = sum(diff_img);
		mse = se[0]/((float)HEIGHT*WIDTH);

		
		count++;

		//cout << opencv_cachemisscount << "   \n"  << endl;  //print opencv cache misses
		cout << opencv_c << "  " << student_c << "  "
			<< (double)myTimer/1000000000.0 << endl;

		float perf = 100.f*(float)opencv_c/(float)student_c;
		avg_perf += perf;
		printf("Performance: %.3f%% (avg: %.3f%%), RMSE: %.4f\n", perf, avg_perf/count, sqrt(mse));

                // opencv_cachemisscount = 0;  // set opencv cache miss count to 0

		Mat inverse;
		idct(dct_student, inverse);
		inverse.convertTo(inverse, CV_8U);

		imshow("Original", gray);
		imshow("IDCT Output", inverse);
		moveWindow("IDCT Output", 500, 0);
		key = waitKey(10);
                
	}

	// cout << "Cache Misses: " << pmn_read(0x0) - i_start << endl;
	// cout << "Cache Misses: " << cache_read(0x0) /*- i_start */ << endl;
        		
	return 0;
}




