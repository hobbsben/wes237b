//
//  main.cpp
//  Lab2
//
//  Created by Alireza on 7/6/17.
//  Copyright © 2017 Alireza. All rights reserved.
//

#include "main.h"
#include "cycletime.h"

using namespace std;

int main(int argc, const char * argv[])
{
        unsigned int len = atoi(argv[1]); 
	unsigned int c_start, i_start;

	//configure register 0x0 to read PMUEVENT[48] (table A.8.1 of ARM Cortex-A9 rev. r4p1, available on piazza)
	pmn_config(0x0, /*event_number*/);

	init_counters(1, 0);

	cout << "WES237B lab 3_1" << endl;
	
        i_start = pmn_read(0x0);
        c_start = get_cyclecount();
	for(int i = 0; i< len; i++)
	{
	    asm volatile ("ISB");
	}
        cout << "CPU Cycles: " << get_cyclecount() - c_start << endl;
        cout << "ISB count: " << pmn_read(0x0) - i_start << endl;

	return 0;
}

