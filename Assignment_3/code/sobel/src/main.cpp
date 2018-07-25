#include <iostream>
#include <cmath>
#include <opencv2/highgui/highgui.hpp>

#include "sobel.h"
#include "cycletime.h"

using namespace std;
using namespace cv;


int main(int argc, const char * argv[])
{
	cout << "WES237B lab 3\n";

	VideoCapture cap("input.raw");

	int WIDTH  = 384;
	int HEIGHT = 384;

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

	init_counters(1, 0);

	Mat frame, gray;
	char key=0;

	unsigned int c_start;
	unsigned int student_c[3];
	int count = 0;
	float avg_perf[3] = {0,0,0};
	unsigned long long avg_cycles[3] = {0,0,0};

	while(key != 'q')
	{
		cap >> frame;
		if(frame.empty())
		{
			waitKey();
			break;
		}

		cvtColor(frame, gray, CV_BGR2GRAY);
		resize(gray, gray, Size(WIDTH, HEIGHT));

		Mat s_x, s_y, cv_sobel_out;
		Sobel(gray, s_x, CV_8U, 1, 0, 3, 1, 0, BORDER_ISOLATED);
		Sobel(gray, s_y, CV_8U, 0, 1, 3, 1, 0, BORDER_ISOLATED);
		addWeighted(s_x, 0.5, s_y, 0.5, 0, cv_sobel_out);

		Mat sobel_out[3];
		sobel_out[0] = Mat::zeros(gray.size(), CV_8U);
		sobel_out[1] = Mat::zeros(gray.size(), CV_8U);
		sobel_out[2] = Mat::zeros(gray.size(), CV_8U);

		// Apply filter

		c_start = get_cyclecount();
		sobel(gray, sobel_out[0]);
		student_c[0] = get_cyclecount() - c_start;
		
		c_start = get_cyclecount();
		sobel_unroll(gray, sobel_out[1]);
		student_c[1] = get_cyclecount() - c_start;
		
		c_start = get_cyclecount();
		sobel_neon(gray, sobel_out[2]);
		student_c[2] = get_cyclecount() - c_start;
		
		count++;
		
		// RMSE and performance

		Mat diff_img[3];
		float mse[3];
		float perf[3];
		const char* descr[3] = {"Baseline", "Unrolled", "NEON"};
		for(int i = 0; i < 3; i++)
		{
			absdiff(sobel_out[0], sobel_out[i], diff_img[i]);
			diff_img[i] = diff_img[i].mul(diff_img[i]);
			Scalar se = sum(diff_img[i]);
			mse[i] = se[0]/((float)HEIGHT*WIDTH);
			
			perf[i] = (float)student_c[0]/(float)student_c[i];
			avg_perf[i] += perf[i];
			avg_cycles[i] += student_c[i];
			printf("%s - Perf: %.2fx (avg: %.2fx), Cycles: %u (avg: %llu)\n",
					descr[i], perf[i], avg_perf[i]/count, student_c[i], avg_cycles[i]/count);

			if(i > 0)
			{
				if(sqrt(mse[i]) > 0.001)
				{
					printf("*** ERROR: RMSE between Baseline and %s is %.3f\n", descr[i], sqrt(mse[i]));
				}
			}
		}
		printf("\n");

		// Display results
		imshow("Input", gray);
		imshow("Sobel OpenCV", cv_sobel_out);
		imshow("Sobel", sobel_out[0]);
		cvMoveWindow("Sobel", 500, 0);

		key = waitKey(10);
	}

	return 0;
}
