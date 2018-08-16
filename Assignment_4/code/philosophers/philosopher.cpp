/*************************************
Patrick Hanrahan
Ben Hobbs
WES237B
Assignment 4

g++ philosophers_stack.cpp -o lab4 -fpermissive -pthread
**************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sstream>
#include <pthread.h>

using namespace std;

pthread_t philosopher[4];
pthread_mutex_t forks[4];
pthread_mutex_t cntr;
int counter;

 /*
void init_led(int led){

        ostringstream led_s;
        led_s << (led+57);
        string name = "/sys/class/gpio/gpio";
        name.append(led_s.str());
        if(access(name.c_str(), F_OK) == -1){
          string str1 = "echo ";
          str1.append(led_s.str());
          str1.append(" > /sys/class/gpio/export");
          system(str1.c_str());

          string str2 = "echo out > /sys/class/gpio/gpio";
          str2.append(led_s.str());
          str2.append("/direction");
          system(str2.c_str());
        }else{
          cout << "led_" << led_s.str() << " is already initialized." << endl;
        }
}

void led_state(int led, int v){

        ostringstream led_s;
        led_s << (led+57);
        ostringstream v_s;
        v_s << (v);
        string str = "echo ";
        str.append(v_s.str());
        str.append(" > /sys/class/gpio/gpio");
        str.append(led_s.str());
        str.append("/value");
        system(str.c_str());
}


void *func(int n)
   {
   printf ("Philosopher %d is thinking\n",n);

   //when philosopher 4 is eating he takes fork 1 and fork 4
   pthread_mutex_lock(&forks[n]);
   pthread_mutex_lock(&forks[(n+1)%4]);
   printf ("Philosopher %d is eating\n",n);
	int j=0;
   for(int i = 0; i<20; i++)
	{
	led_state(n+4, j+=i%2);
	usleep(100000);
	cout << "blink\n";
	}
   usleep(50000);
   pthread_mutex_unlock(&forks[n]);
   pthread_mutex_unlock(&forks[(n+1)%4]);

   printf ("Philosopher %d finished eating\n",n);
   led_state(n+4, 0);

   return(NULL);
   }
*/

int main(int arg1)
   {
   cout << arg1 << "\n\n" << endl;
   // initialize leds
   /*
   init_led(4);
   init_led(5);
   init_led(6);
   init_led(7);
   // turn all leds off at beginning
   led_state(4, 0);
   led_state(5, 0);
   led_state(6, 0);
   led_state(7, 0);
   int i;
   while(true)
    {
       for(i=0;i<4;i++)
          pthread_mutex_init(&forks[i],NULL);

       for(i=0;i<4;i++)
          pthread_create(&philosopher[i],NULL,(void *)func,(void *)i);

       for(i=0;i<4;i++)
          pthread_join(philosopher[i],NULL);

       for(i=0;i<4;i++)
          pthread_mutex_destroy(&forks[i]);
    }
   */ 
   return 0;
   }
