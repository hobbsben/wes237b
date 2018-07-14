//
//  main.cpp
//  Lab2
//
//  Created by Alireza on 7/6/17.
//  Copyright © 2017 Alireza. All rights reserved.
//

#include "main.h"
#include "cycletime.h"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, const char * argv[])
{
	unsigned int c_start;
	unsigned int opencv_c, student_c;

	cout << "WES237B lab 2" << endl;

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
	long long unsigned int avg_cycles = 0;
	int count = 0;

	while(key != 'q')
	{
		cap >> frame;
		if(frame.empty()){ break; }

		cvtColor(frame, gray, CV_BGR2GRAY);
		resize(gray, gray, Size(WIDTH, HEIGHT));
		gray.convertTo(gray, CV_32FC1);

		init_counters(1, 0);

		// OpenCV DCT
		c_start = get_cyclecount();
		dct(gray, dct_org);
		opencv_c = get_cyclecount() - c_start;

		// Your DCT
		c_start = get_cyclecount();
		dct_student = student_dct(gray);
		student_c = get_cyclecount() - c_start;

		gray.convertTo(gray, CV_8UC1);

		absdiff(dct_org, dct_student, diff_img); 

		/* calculating RMSE */
		diff_img = diff_img.mul(diff_img);
		Scalar se = sum(diff_img);
		mse = se[0]/((float)HEIGHT*WIDTH);

		
		count++;

		float perf = 100.f*(float)opencv_c/(float)student_c;
		avg_perf += perf;
		avg_cycles += student_c;
		printf("Perf: %.3f%% (avg: %.3f%%), Cycles: %d (avg: %d), RMSE: %.4f\n",
				perf, avg_perf/count,
				student_c, avg_cycles/count,
				sqrt(mse));


		Mat inverse;
		idct(dct_student, inverse);
		inverse.convertTo(inverse, CV_8U);

		imshow("Original", gray);
		imshow("IDCT Output", inverse);
		moveWindow("IDCT Output", 500, 0);
		key = waitKey(10);
	}

	return 0;
}




