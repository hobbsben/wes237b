 //Pat Hanrahan
 //Ben Hobbs
 //assignment 4
 //Aug 15 2018
 
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

#define thread_num 4 
#define max_meals 20 
 

int min_eat = 1;   // initialize default min time in ms
int max_eat = 100;       // initialize default max time in ms
int min_think = 1;   // initialize default min time in ms
int max_think = 100;       // initialize default max time in ms


// /*
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

/* initial status of fork */ 
/* 1 - the fork is available */ 
/* 0 - the fork is not available */ 
int forks[thread_num]={1,1,1,1}; 
 
/* mutex locks for each fork */ 
pthread_mutex_t m[thread_num];  
 
/* philosopher ID */ 
int p[thread_num]={0,1,2,3}; 
 
/* number of meals consumed by each philosopher */ 
int numMeals[thread_num]={0,0,0,0}; 
 
/* counter for the number of meals eaten */ 
int mealCount = 0; 
 
 
void *philosopher(void *); /* prototype of philosopher routine */ 
 
 
int main(int argc, const char * argv[]) 
{ 
   min_eat =  atoi(argv[1]);   
   max_eat = atoi(argv[2]);
   min_think =  atoi(argv[3]);   
   max_think = atoi(argv[4]);
  // cout << min_eat << endl;
  // cout << max_eat << endl;	
  // cout << min_think << endl;
  // cout << max_think << endl;	

    pthread_t tid[thread_num];
    void *status;
    int i,j; 

    srand( (long)time(NULL) );
 
    /* create 4 threads representing 4 dining philosopher */ 
    for (i = 0; i < thread_num; i++) 
    { 
        if(pthread_create( tid + i, 0, philosopher, p + i ) != 0) 
	    { 
	        perror("pthread_create() failure."); 
	        exit(1); 
	    } 
    } 
 
    /* wait for the join of 4 threads */ 
    for (i = 0; i < thread_num; i++) 
    { 
        if(!pthread_join(tid[i], &status )==0) 
        { 
            perror("thr_join() failure."); 
            exit(1); 
        } 
    }  
    
    return (0); 
} 
 
 
void *philosopher(void  *arg) 
{  
    int  forknum = *(int*)arg; 
 
    while( 1 ) 
    {
 
        // lock the left fork 
        pthread_mutex_lock( m + forknum ); 
         
        if( forks[forknum] == 1 ) 
        { // the left is available  
            printf("philosopher %d: left=%d\n",forknum,forks[forknum]);  
            printf("Philosopher %d: I have the left fork\n", forknum); 
 
            forks[forknum]=0; // set the left fork unavailable 
             
            pthread_mutex_unlock( m + forknum ); // unlock the left fork
 
            /* lock the right fork */ 
            pthread_mutex_lock( m + ((forknum+1)%thread_num) ); 
             
            if( forks[(forknum+1)%thread_num]==1 ) 
            { /* the right is available */ 
                printf("philosopher %d: right=%d\n", forknum,
                                    forks[(forknum+1)%thread_num]);
                
                /* set the right fork unavailable */ 
                forks[(forknum+1)%thread_num]=0; 
                 
                /* unlock the right fork */ 
                pthread_mutex_unlock( m + ( (forknum+1) % thread_num) ); 
		 
		        printf("Philosopher %d: I have two forks\n", forknum); 
                printf("philosopher %d: I am eating!\n\n", forknum); 
		
                numMeals[forknum]++; 
		        mealCount++; 
		int j=0;
                for(int i=0; i<rand() % max_eat + min_eat;i++) /* eating time */ 
 		{
		led_state(forknum+4,j%2);
		usleep(5000);
		j+=i;
		}
		
		for(int i=0; i<rand() % max_think + min_think;i++) /* thinking time */ 
		{
		led_state(forknum+4,1);		
		}  
		
		led_state(forknum+4,0);             
		/* lock the left & right forks */ 
                pthread_mutex_lock( m + forknum );  
                pthread_mutex_lock( m + ( (forknum+1) % thread_num) ); 
 
                forks[forknum]=1;   /*set the left fork available */ 
                
                /* set the right fork available */
                forks[(forknum+1)%thread_num]=1;  
 
                /* unlock the left & right fork */ 
                pthread_mutex_unlock ( &m[forknum]); 
                pthread_mutex_unlock (&m[(forknum+1)%thread_num]); 
		
                usleep(rand() % 3000000); /* food settling time */ 
            } 
            else 
            { /* the right is unavailable */ 
                printf("philosopher %d: right=%d\n", 
                       forknum, forks[(forknum+1)%thread_num]);	 
                printf("Philosopher %d: I can't grab the right fork\n\n", forknum); 
 
                /* unlock the right fork */ 
                pthread_mutex_unlock( &m[(forknum+1)%thread_num] ); 
 
                /* lock the left fork */ 
                pthread_mutex_lock( &m[forknum] ); 
                 
                /* set the left fork available (put down the left one) */ 
                forks[forknum]=1; 
 
                /* unlock the left fork */ 
                pthread_mutex_unlock( &m[forknum] ); 

                /* wait for a while and try again later */ 
                usleep(rand() % 3000000); 
            } 
        }     
        else 
        { /* the left fork is unavailable */ 
            printf("philosopher %d: left=%d\n",forknum,forks[forknum]);  
            printf("Philosopher %d: I can't grab the left fork\n\n", 
                   forknum); 
 
            /* unlock the left fork */ 
            pthread_mutex_unlock( &m[forknum] ); 
             
           /* wait for a while and try again later */ 

            usleep(rand() % 3000000);
        } 
	
       sched_yield();  /* for LINUX you may also use pthread_yield(); */
    } 
    printf("Philosopher %d has finished eating\n", forknum); 
    pthread_exit(0); 
} 
