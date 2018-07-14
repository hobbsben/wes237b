# Assignment 2 - Due date: 07/21/2018
In this assignment you have to implement different optimizations for 2D Discrete Cosine Transform (DCT).

You are provided with a skeleton code with a naive, four nested loops, implementation of DCT. This code includes a testbench that you can use for developing your codes. In addition, this template code provides you with a CPU cycle counter code that you can use to measure your code’s performance.

You have to use the provided source code for implementing the three following optimization techniques:

## Part 1: Look Up Table (LUT)
DCT coefficients are constant for N-size transforms.

1. Report the performance and cycle count for an input size of 64 (default), without optimizations.
2. Complete the code for initializing the LUTs with DCT coefficients.
3. Use the LUTs to replace the cos() functions in the nested loops and eliminate redundant calculation of the coefficients in each iteration.
4. Also incorporate the scaling inside your LUT, and remove the scaling line in the code.
5. Report the performance and cycle count for an input size of 64, with LUT.

## Part 2: Matrix Multiplication (MM)
Assuming an input of same width and height, DCT transform can be implemented using MM using the following formula:

DCT = CXCT

Where C is the coefficient matrix, and X is your input matrix.

1. Implement a naive matrix multiplication code, for square matrices.
2. Use your code, and one of the LUTs to apply the formula above. Note: You can use OpenCV to get the transpose. Look at the hints in the source code.
3. Report the performance and cycle count for an input size of 64, with naive MM.

## Part 3: Block Matrix Multiplication (BMM)
To increase the performance of your code for bigger image sizes, you have to implement DCT using BMM.

1. Increase the size of your input image to 384 (just pass 384 as command-line argument).
2. Report the performance and cycle count for an input size of 384, with naive MM.
3. Optimize your MM code using blocks.
4. Find the optimal block size for that input size.
5. Report the performance and cycle count for an input size of 384, with BMM.

### Notes
* All three optimizations must have RMSE lower than 0.001.
* All three optimizations must use LUT for DCT coefficients and scaling.
* You have to report the mean of **both** CPU clock cycles and performance (in %, relative to OpenCV DCT) for each optimization in your report.
* You can use the CPU clock cycle divider to avoid overflow as needed.

# Work Submission

**IMPORTANT:** Follow the instructions exactly, and **double check your repository, folder, and file names**. You may lose points if you do not follow the exact instructions. If you have a doubt or a question, please ask the TAs, preferably on Piazza.

* All of your codes related to assignment 2 must be placed in a folder called Assignment_2 in your repository’s source directory
* You have to submit these:
    * A short report named **Report.pdf** with brief report on your optimizations and mean of the CPU cycles and performance for each optimization. This should be at the root of the assignment folder.
    * The modified **dct.cpp** file. This should be at the root of the assignment folder. 
* Do not submit other files than those listed, unless you have a good reason, and explain so in your report.

File structure

```
Assignment_2/
  Report.pdf
  dct.cpp 
```      


