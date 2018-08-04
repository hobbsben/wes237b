#include "main.h"


//#define USE_BASELINE
//#define USE_BASELINE_WITH_LUT
//#define USE_1D_DCT
//#define USE_MM
#define USE_MM_BLOCK

using namespace cv;


Mat LUT_w;
Mat LUT_h;


// Helper function
float sf(int in){
    if (in == 0)
        return 0.70710678118; // 1 / sqrt(2)
    return 1.;
}

// Initialize LUT
void initDCT(int WIDTH, int HEIGHT)
{
	LUT_w = Mat(WIDTH, WIDTH, CV_32F);
	LUT_h = Mat(HEIGHT, HEIGHT, CV_32F);

    float scale = 2.f/sqrtf(HEIGHT*WIDTH);
	float sqrt_scale = sqrtf(scale);
	
	for(int j = 0; j < WIDTH; j++)
	{
		for(int i = 0; i < WIDTH; i++)
		{
			LUT_w.at<float>(i, j) =
				sqrt_scale *
				sf(i) *
				cos(M_PI/((float)WIDTH)*(j+1./2.)*(float)i);
		}
	}

	for(int j = 0; j < HEIGHT; j++)
	{
		for(int i = 0; i < HEIGHT; i++)
		{
			LUT_h.at<float>(i, j) =
				sqrt_scale *
				sf(i) *
				cos(M_PI/((float)HEIGHT)*(j+1./2.)*(float)i);
		}
	}

}



// 1D DCT
#ifdef USE_1D_DCT
Mat dct_1d(Mat input, int len, bool is_row){
    if(is_row){
        Mat result = Mat(1, len, CV_32FC1);//row,col; 1xlen
        float z;
        for(int i=0; i<len; i++){
            z = 0;
            for(int j=0; j<len; j++){
                z += input.at<float>(0, j) * cos(M_PI/((float)len)*(j+1./2.)*(float)i);
            }
            result.at<float>(0, i) = z*sf(i);
        }
        return result;
    }else{
        Mat result = Mat(len, 1, CV_32FC1);
        float z;
        for(int i=0; i<len; i++){
            z = 0;
            for(int j=0; j<len; j++){
                z += input.at<float>(j, 0) * cos(M_PI/((float)len)*(j+1./2.)*(float)i);
            }
            result.at<float>(i, 0) = z*sf(i);
        }
        return result;
    }        
}

// Opt: O(N^3)
Mat student_dct(Mat input)
{
	const int HEIGHT = input.rows;
	const int WIDTH  = input.cols;

    float scale = 2./sqrt(HEIGHT*WIDTH);

    Mat result = Mat(HEIGHT, WIDTH, CV_32FC1);
    Mat rows   = Mat(HEIGHT, WIDTH, CV_32FC1);

    for(int x = 0; x < HEIGHT; x++)
	{
        Mat temp = input.row(x);//1xHEIGHT
        Mat temp_dct = dct_1d(temp, HEIGHT, true);
        temp_dct.copyTo(rows.row(x));
    }
    for(int y = 0; y < WIDTH; y++)
	{
        Mat temp = rows.col(y);//WIDTHx1
        Mat temp_dct = dct_1d(temp, WIDTH, false);
        temp_dct.copyTo(result.col(y));
    }
    return result*scale;
}
#endif // USE_1D_DCT


// Baseline: O(N^4)
#if defined(USE_BASELINE) || defined(USE_BASELINE_WITH_LUT)
Mat student_dct(Mat input)
{
	const int HEIGHT = input.rows;
	const int WIDTH  = input.cols;

    float scale = 2./sqrt(HEIGHT*WIDTH);

    Mat result = Mat(HEIGHT, WIDTH, CV_32FC1);
    
	float *LUT_h_ptr = LUT_h.ptr<float>();
    float *LUT_w_ptr = LUT_w.ptr<float>();

	float* result_ptr = result.ptr<float>();
	float* input_ptr  = input.ptr<float>();

    for(int x = 0; x < HEIGHT; x++)
	{
        for(int y = 0; y < WIDTH; y++)
		{
            //result.at<float>(x, y) = 0;
			float value = 0.f;

            for(int i = 0; i < HEIGHT; i++)
			{
                for(int j = 0; j < WIDTH; j++)
				{
                    //result.at<float>(x, y) += input.at<float>(i, j)
                    //value += input.at<float>(i, j)
                    value += input_ptr[i * WIDTH + j]
#ifndef USE_BASELINE_WITH_LUT
						* cos(M_PI/((float)HEIGHT)*(i+1./2.)*(float)x)
						* cos(M_PI/((float)WIDTH)*(j+1./2.)*(float)y);
#else
						//* LUT_h.at<float>(x, i)
						//* LUT_w.at<float>(y, j); // Note: the LUT includes scaling
						* LUT_h_ptr[x * HEIGHT + i]
						* LUT_w_ptr[y * WIDTH  + j];
#endif
                }
            }
#ifndef USE_BASELINE_WITH_LUT
            //result.at<float>(x, y) = scale * sf(x) * sf(y) * result.at<float>(x, y);
            value = scale * sf(x) * sf(y) * value;
#endif
			result_ptr[x * WIDTH + y] = value;
        }
    }

    return result;
}
#endif // USE_BASELINE*


#if defined(USE_MM) || defined(USE_MM_BLOCK)

// Matrix multiplication code
void naive_square_matmul(const float* A, const float* B, float* C, int size)
{
	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			C[i * size + j] = 0;
			for(int k = 0; k < size; k++)
			{
			    C[i * size + j] += A[i * size + k] * B[k * size + j];
			}
		}
	}
}

// Matrix multiplication with blocking
void matmul_block(const float* A, const float* B, float* C, int size, int bsize)
{
	for(int jj = 0; jj < size; jj += bsize)
	{
		for(int kk = 0; kk < size; kk += bsize)
		{
			for(int i = 0; i < size; i++)
			{
				for(int j = jj; j < ((jj+bsize)>size?size:(jj+bsize)); j++)
				{
					float temp = 0;
					for(int k = kk; k < ((kk+bsize)>size?size:(kk+bsize)); k++)
					{
						temp += A[i * size + k] * B[k * size + j];
					}
					C[i * size + j] += temp;
				}
			}
		}
	}
}


// DCT as matrix multiplication
Mat student_dct(Mat input)
{
	// -- Works only for WIDTH == HEIGHT
	assert(input.rows == input.cols);
	
	// -- Matrix multiply with OpenCV
	//return LUT_w * input * LUT_w.t();


	// -- Student matrix multiply
	Mat LUT_t  = LUT_w.t();
	Mat temp   = Mat(input.size(), CV_32F, 0.f);
	Mat output = Mat(input.size(), CV_32F, 0.f);

#ifndef USE_MM_BLOCK
	naive_square_matmul(LUT_w.ptr<float>(), input.ptr<float>(), temp.ptr<float>(), input.rows);
	naive_square_matmul(temp.ptr<float>(), LUT_t.ptr<float>(), output.ptr<float>(), input.rows);
#else
	matmul_block(LUT_w.ptr<float>(), input.ptr<float>(), temp.ptr<float>(),  input.rows, 32);
	matmul_block(temp.ptr<float>(), LUT_t.ptr<float>(), output.ptr<float>(), input.rows, 32);
#endif

	return output;
}
#endif // USE_MM
