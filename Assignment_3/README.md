# Assignment 3 - Due date: 08/04/2017 
In this assignment, you will learn how to measure cache miss, implement Sobel filter, and optimize convolution on an ARM processor.

## Part 1) Measuring Cache Miss on ARM
This task is based on your Block Matrix Multiplication (BMM) in dct/. Similar to what you did in lab 3 part 1 (lab3_1), modify cycletime.h for BMM code to add necessary functions for counting cache misses. You have to modify main.cpp to call appropriate functions and print cache misses for different block sizes.

ARM has different cache layers and by changing block size of BMM you should get different cache miss rates. You have to try different block sizes and report your observations using a graph. Based on your observations, you have to provide block size versus cache miss/cache hit graph in your final submission.

## Part 2) Basic Sobel algorithm
You have to implement a Sobel filter **without using OpenCV functions**. We don’t provide an error function for your baseline Sobel implementation, however your Sobel filter should produce similar visual result compared to the OpenCV Sobel function.

## Part 3) Optimizations
In lab 3 part 2 (lab3_2), you learned how to optimize a FIR filter by loop unrolling and using NEON instructions. For this part, you have to optimize your Sobel implementation using similar techniques. You have to do the following optimizations for your Sobel function:

1. Loop unrolling: Your Sobel function, unlike the FIR from the lab is a convolution on 2D signals. The principle of loop unrolling is similar for both functions, and you can use your lab experiences to complete this task.
2. ARM NEON instructions: use appropriate ARM NEON instructions to speedup your Sobel code. You are free to use any NEON instruction for this task.
Use your non-optimized basic Sobel algorithm as your baseline. In your brief report you have to provide a comparison between your optimizations and your baseline.

# Work Submission
**IMPORTANT:** Follow the instructions exactly, and **double check your repository, folder, and file names**. You may lose points if you do not follow the exact instructions. If you have a doubt or a question, please ask the TAs, preferably on Piazza.

* All of your codes related to assignment 3 must be placed in a folder called Assignment_3 in your repository’s source directory
* You have to submit these:
	* A short report named **Report.pdf** with brief report on your optimizations and cache sizes. This should be at the root of the assignment folder.
	* For part 1: the modified **main.cpp**, **cycletime.h**, and **student_dct.cpp**. These should be at the root of the assignment folder.
	* For part 2 and 3: the modified **student_sobel.cpp** including your basic Sobel implementation, loop unrolling optimization, and NEON optimization. This should be at the root of the assignment folder.

Do not submit other files than those listed, unless you have a good reason, and explain so in your report.

```
Assignment_2/
    Report.pdf
    main.cpp
    cycletime.h
    student_dct.cpp
    student_sobel.cpp
    code/
        dct/
            input.raw
            Makefile
            include/
                cycletime.h
                timer.h
                main.h
            src/
                main.cpp
                student_dct.cpp
            kernel_module/
                <should-not-change>
        sobel/
            input.raw
            Makefile
            include/
                main.h
                cycletime.h
            src/
                main.cpp
                student_sobel.cpp
```
