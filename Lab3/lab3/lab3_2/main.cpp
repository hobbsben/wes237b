#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <cmath>
#include <fstream>

#include "main.h"
#include "cycletime.h"
#include <arm_neon.h>

using namespace std;

// ----------------------------------------------
// Run a FIR filter on the given input data
// ----------------------------------------------
void fir(float *coeffs, float *input, float *output, int length, int filterLength)
// ----------------------------------------------
{

	// TODO
	float temp=0;	
	for(int i=0; i<length; i++)
	{
		temp=0;
		for(int k=0; k<filterLength;k++)
		{	
		temp += input[i+k] * coeffs[k];
		}
		output[i]=temp;
	}
	
}

//UNROLLED	
/*
{
	// TODO
	float temp0=0, temp1=0, temp2 = 0, temp3 =0;	
	for(int i=0; i<length; i++)
	{
		temp0=0;
		temp1=0;
		temp2=0;		
		temp3=0;
		for(int k=0; k<filterLength;k+=4)
		{	
		temp0 += input[i+k] * coeffs[k];
		temp1 += input[i+k+1] * coeffs[k+1];
		temp2 += input[i+k+2] * coeffs[k+2];
		temp3 += input[i+k+3] * coeffs[k+3];
		}
		output[i]=temp0 + temp1+temp2+temp3;
	}
	
}
*/

// ----------------------------------------------
// Run a FIR filter on the given input data using NEON
// ----------------------------------------------
void fir_neon(float *coeffs, float *input, float *output, int length, int filterLength)
// ----------------------------------------------
{
		
	// TODO
	float init_zeros[4] = {0, 0, 0, 0};
	float output_sum[4] = {0, 0, 0, 0};

	float32x4_t coeff_32x4 = vld1q_f32(init_zeros);
	float32x4_t in_32x4 = vld1q_f32(init_zeros);
	float32x4_t temp = vld1q_f32(init_zeros);
	
	for(int i=0; i<length; i++)
	{
		
		temp = vld1q_f32(init_zeros);

		for(int k=0; k<filterLength;k+=4)
		{
	
			coeff_32x4 = vld1q_f32(coeffs + k);
			in_32x4 = vld1q_f32(input + i + k);
			temp = vmlaq_f32(temp, coeff_32x4, in_32x4);

		}
		vst1q_f32(output_sum, temp);
		output[i] = output_sum[0] + output_sum[1] + output_sum[2] + output_sum[3];

		
	}
	

}


// ----------------------------------------------
// Create filter coefficients
// ----------------------------------------------
void designLPF(float* coeffs, int filterLength, float Fs, float Fx)
// ----------------------------------------------
{
	float lambda = M_PI * Fx / (Fs/2);

	for(int n = 0; n < filterLength; n++)
	{
		float mm = n - (filterLength - 1.0) / 2.0;
		if( mm == 0.0 ) coeffs[n] = lambda / M_PI;
		else coeffs[n] = sin( mm * lambda ) / (mm * M_PI);
	}
}



// ----------------------------------------------
int main( void )
// ----------------------------------------------
{
	const int SAMPLES    = 10000; // Size of input data
	const int FILTER_LEN = 64; // Size of filter
	
	unsigned int c_start;
	unsigned int opencv_c, student_c;

	// Initialize coefficients
	float coeffs[FILTER_LEN];
	
	designLPF(coeffs, FILTER_LEN, 44.1, 2.0);

	ifstream ifile("input.txt");
	ifstream gfile("golden_output.txt");

	for(int nruns = 0; nruns < 10; nruns++)
	{
		// Create inputs
		float input[SAMPLES];

		for(int i = 0; i < SAMPLES; i++)
		{
			ifile >> input[i];
		}

		// Pad inputs
		const int PADDED_SIZE = SAMPLES + 2*FILTER_LEN;
		const int OUTPUT_SIZE = SAMPLES + FILTER_LEN;

		float paddedInput[PADDED_SIZE];
		float output1[OUTPUT_SIZE];
		float output2[OUTPUT_SIZE];

		for(int i = 0; i < PADDED_SIZE; i++)
		{
			if(i < FILTER_LEN || i >= SAMPLES+FILTER_LEN)
			{
				paddedInput[i] = 0;
			}
			else{ paddedInput[i] = input[i - FILTER_LEN]; }
		}


		init_counters(1, 0);

		// Run once
		fir(coeffs, paddedInput, output1, PADDED_SIZE, FILTER_LEN);


		// Measure baseline
		c_start = get_cyclecount();
		fir(coeffs, paddedInput, output1, PADDED_SIZE, FILTER_LEN);
		opencv_c = get_cyclecount() - c_start;
		
		// Measure optimized
		c_start = get_cyclecount();
		if(FILTER_LEN % 4 != 0){ return -1; }
		fir_neon(coeffs, paddedInput, output2, PADDED_SIZE, FILTER_LEN);
		student_c = get_cyclecount() - c_start;

		// Check for errors
		if(FILTER_LEN != 64)
		{
			cout << "WARNING: Golden output for filter size 64 only!" << endl;
		}
		float mse1 = 0, mse2 = 0;
		float golden;
		for(int i = 0; i < OUTPUT_SIZE; i++)
		{
			gfile >> golden;
			float diff1 = output1[i] - golden;
			float diff2 = output2[i] - golden;
			mse1 += diff1 * diff1;
			mse2 += diff2 * diff2;
		}

		float perf = 100.f*(float)opencv_c/(float)student_c;
		printf("Perf: %.2f, RMSE1: %.2f, RMSE2: %.2f\n", perf, sqrt(mse1), sqrt(mse2));
		


		// Print result
		//printArray(input, SAMPLES, false);
		//cout << "\n";
		//printArray(paddedInput, PADDED_SIZE, false);
		//cout << "\n";
		//printArray(output1, OUTPUT_SIZE, false);
		//cout << "\n";
		//cout << endl;
	}

	return 0;
}


