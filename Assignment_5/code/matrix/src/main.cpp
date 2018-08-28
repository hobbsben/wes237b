#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

#include <opencv2/imgproc/imgproc.hpp>
#include <cuda_runtime.h>

#include "cuda_error.h"
#include "matrixmul.h"


using namespace std;
using namespace cv;


// ***********************************************
int main(int argc, char const *argv[])
// ***********************************************
{
	if(argc < 3)
	{
		cout << "Usage: " << argv[0] << " M N" << endl;
		return 1;
	}

	int M = atoi(argv[1]);
	int N = atoi(argv[2]);

	srand(time(0));

	float* h_A;
	float* h_B;
	float* h_C;
	float* h_Ccpu;
	CudaSafeCall(cudaMallocManaged((void**) &h_A,  sizeof(float)*N*M));
	CudaSafeCall(cudaMallocManaged((void**) &h_B,  sizeof(float)*M*N));
	CudaSafeCall(cudaMallocManaged((void**) &h_C,  sizeof(float)*N*N));
	CudaSafeCall(cudaHostAlloc((void**) &h_Ccpu, sizeof(float)*N*N, cudaHostAllocMapped));

	// Initialize matrices
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < M; j++)
		{
			h_A[i * M + j] = rand() / (float)RAND_MAX;
		}
	}

	for(int i = 0; i < M; i++)
	{
		for(int j = 0; j < N; j++)
		{
			h_B[i * N + j] = rand() / (float)RAND_MAX;
		}
	}

	// MM GPU
	float time_gpu = run_mm_gpu(h_A, h_B, h_C, M, N);
        Mat output = Mat(N, N, CV_32F, h_C);

	// Profiling
	float time_cpu;

	cudaEvent_t start, stop;
	CudaSafeCall(cudaEventCreate(&start));
	CudaSafeCall(cudaEventCreate(&stop));

	// MM CPU
	CudaSafeCall(cudaEventRecord(start, 0));

	Mat cv_A = Mat(N, M, CV_32F, h_A);
	Mat cv_B = Mat(M, N, CV_32F, h_B);
	Mat cv_C = Mat(N, N, CV_32F, h_Ccpu);
        
	cv_C = cv_A * cv_B;

	CudaSafeCall(cudaEventRecord(stop, 0));
	CudaSafeCall(cudaEventSynchronize(stop));
	CudaSafeCall(cudaEventElapsedTime(&time_cpu, start, stop));

	CudaSafeCall(cudaEventDestroy(start));
	CudaSafeCall(cudaEventDestroy(stop));

	// Check for errors
	float mse = 0.f;
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			float diff = abs(h_C[i*N + j] - h_Ccpu[i*N + j]);
			mse += diff * diff;
                        		}
	}
	mse /= N*N;

        cout  <<"Pats output: " << setprecision(2) <<  output << endl;
        cout <<"CPU output: " << setprecision(2) << cv_C << endl;


	float rmse = sqrt(mse);

	stringstream ss;
	ss << fixed;
	ss << setprecision(2);
	ss << "Time CPU = " << time_cpu << "ms, Time GPU = " << time_gpu << "ms, Speedup = " << time_cpu/time_gpu << "x, RMSE = ";
	ss << setprecision(5);
	ss << rmse;

	cout << ss.str() << endl;


	// Free memory
	CudaSafeCall(cudaFree(h_A));
	CudaSafeCall(cudaFree(h_B));
	CudaSafeCall(cudaFree(h_C));
	CudaSafeCall(cudaFreeHost(h_Ccpu));

	return 0;
}
