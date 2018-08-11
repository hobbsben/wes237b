#include "main.h"
#include "cycletime.h"

using namespace std;
using namespace cv;

#define FADD(a,b)   ((a)+(b))
#define FMUL(a,b,q) (((a)*(b))>>(q))
#define FCONV(a, q1, q2) (((q2)>(q1)) ? (a)<<((q2)-(q1)) : (a)>>((q1)-(q2)))
#define TOFIX(d, q) ((int)( (d)*(double)(1<<(q)) ))
#define TOFLT(a, q) ( (double)(a) / (double)(1<<(q)) )

int main(int argc, const char *argv[]) {

	int len = 1024;
	float arr_cos_flt[len];
	int arr_cos_fix[len];
	
	int freq = 512;
	Mat image_flt, image_fix, result_flt, result_fix;
	image_flt = Mat(200, len+88, CV_32F);
	image_fix = Mat(200, len+88, CV_8U);

        cout << "WES237B Assignment4_p_2 - fixpoint" << endl;
_get:
	cout << "enter read and write exponents: q1 q2" << endl;

	int q1, q2;
	scanf("%d %d", &q1, &q2);
	
	if(q1>14 || q2>14){
	  cout << "exponent must be less or equal to 14" << endl;
	  goto _get;
	}	

	for (int j=0; j<len; j++){
	  arr_cos_flt[j] = cos( 2.0*(float)((j)%freq)/(float)(freq) * M_PI);
	  arr_cos_fix[j] = TOFIX(arr_cos_flt[j], q1);
	}
	
	for(int j=0; j<len; j++){
	  image_flt.at<float>(100+50*arr_cos_flt[j], j+44) = 1;
	  image_fix.at<uchar>(100+50*TOFLT(FCONV(arr_cos_fix[j], q1, q2), q2), j+44) = 255;
	}

	/* drawing signals */
	imshow("float", image_flt);
	moveWindow("float", 20,0);
	imshow("fix", image_fix);
	moveWindow("fix", 20,400);
	waitKey();
	
	int klen = 5;
	float kernel_flt[] = {0.0625, 0.125, 0.25, 0.5, 0.75};	
	int kernel_fix[klen];
	for(int j=0; j<klen; j++){
	  kernel_fix[j] = TOFIX(kernel_flt[j], q1);
	}
	float res_flt[len+klen-1];
	int res_fix[len+klen-1];

	for(int j=0; j<len+klen-1; j++){
	  res_flt[j]=0;
	  res_fix[j]=0;
	}

/* DO NOT MODIFY ANYTHING ABOVE THIS LINE */
	
	/*1D convolution using float */
        unsigned int c_start;
        init_counters(1, 0);
        c_start = get_cyclecount();
	/* insert your code here */


	/* ********************* */
        cout << "float CPU Cycles: " << get_cyclecount() - c_start << endl;
	
	/*1D convolution using fixed point */
        init_counters(1, 0);
        c_start = get_cyclecount();
	/* insert your code here */


	/* ********************* */
        cout << "fixpoint CPU Cycles: " << get_cyclecount() - c_start << endl;

/* DO NOT MODIFY ANYTHING BELOW THIS LINE */

	/* calculating RMSE */
        double _se = 0;
	for(int j=0; j<len; j++){
	  float _d = (abs(res_flt[j] - TOFLT(FCONV(res_fix[j], 2*q1, q2), q2)));
	  _se += _d*_d;
	}
	double _mse = _se/(float)len;
	double _rmse = sqrt(_mse);

        cout << "RMSE: " << _rmse << endl;
	
	/* drawing results */
	for(int j=0; j<len; j++){
	  image_flt.at<float>(100+50*res_flt[j], j+44) = 1;
	  image_fix.at<uchar>(100+50*TOFLT(FCONV(res_fix[j], 2*q1, q2), q2), j+44) = 255;
	}
	imshow("float", image_flt);
	moveWindow("float", 20,0);
	imshow("fix", image_fix);
	moveWindow("fix", 20,400);
	waitKey();
 
  return 0;
}
