# Assignment 4 - Due date: 08/18/2017
In this assignment, you will learn how to use threads, practice scheduling, and fixed point optimization on ARM.

## Part 1: Dining Philosophers Problem
In this part you have to use your lab experience for controlling LEDs via threads and implement the dining philosopher problem on an ARM core.

* As discussed in the lab session, you have four philosophers and four forks.
* Each philosopher is a thread, and each fork is shared resource between two adjacent philosophers.
* Each philosopher needs two forks for eating. When a philosopher has both forks, he eats for a while, then releases its forks, thinks for a while, and waits to have its both forks to repeat his eating-thinking-waiting sequence.
* Each Philosopher uses one of the four LEDs to indicate its state:
    * Blinking LED (eating): in this state your thread must blink its LED for a period of time.
    * Solid LED ON (thinking): in this state your thread must keep its LED ON for a period of time.
    * LED OFF (waiting): in this state your thread is waiting for its resources (forks) and must keep its LED OFF until both resources are available.
    * Store the LED states and time in each state (in CPU cycles) anyway you'd like. Use that to create a clear, easy to read graph depicting the amount of time each philosopher was in each state.
* The eating time and thinking time must be a random value between a minimum and maximum number, in milliseconds. You can use the rand() function to generate a random integer between two values. **The min/max values should be passed as a command line argument.** Your code must work for values going from 1ms to a few thousands of ms.
* You have to use mutex to control access to the shared resources (forks).
* Your shared resources (forks) cannot be mutex themselves.
* You must implement your code in a way that each philosopher periodically get a chance to eat and think.
* Note that a shared resource (fork) can only be used by one thread at a time, in another word two adjacent philosophers can not eat at the same time.
We do not provide a template for this part and you have to create your own project. Your project must be organized similar to lab examples and must follow this structure and naming convention:

```
philosophers/
    Makefile    // used to compile your project
    obj/       
    include/    // all header files must be placed here
    src/        // all .cpp files must be placed here
```      
Your Makefile should generate a "lab4" executable taking 4 arguments:

```
./lab4 <min_eat> <max_eat> <min_think> <max_think>
```

## Part 2: 1D convolution

In this part you have to implement 1D convolution using float variables and fixed point variables using the provided template. This template code asks for two exponents q1 and q2, one for converting the original float signal into fixed point and one for converting back the results of convolution to float.

When you run this template, it first shows the original float signal and its fixed point equivalent, waits for user to press enter and then it calculates the convolution and shows the results.

* You have to complete the code and perform 1D convolution in the provided template.
* Try different values for **q1** and **q2** and explain your observations (in drawing, CPU cycles, and RMSE) in your report.
* Similar to the lab session, utilize your CPU by running a GNURadio application and monitor your code’s performance with and without CPU shielding.

# Work Submission
IMPORTANT: Follow the instructions exactly, and double check your repository, folder, and file names. You may lose points if you do not follow the exact instructions. If you have a doubt or a question, please ask the TAs, preferably on Piazza.

* All of your codes related to assignment 4 must be placed in a folder called Assignment_4 in your repository’s source directory
* You have to submit these:
    * A short report named **Report.pdf** with brief report on your method for part 1 and your observations from part 2. Additionally for part1, comment on the graph of resource sharing. This should be at the root of the assignment folder.
    * For part 1: provide a 2-5 minutes video in which you have to explain your code, and the demo of your the dinning philosophers problem running on Zedboard. Your code must be clean, easy to read, and commented.
    * For part 2: the modified **fp.cpp** including your float and fixed point 1D convolution implementation. This should be at the root of the assignment folder. Your code must be clean, easy to read, and commented.
Do not submit other files than those listed, unless you have a good reason, and explain so in your report.

```
    Assignment_4/
        Report.pdf
        fp.cpp
        philosophers/
            Makefile
            obj/
            include/
            src/
        conv/
            Makefile/
            include/
                cycletime.h
                main.h
            obj/
            src/
                fp.cpp
```

