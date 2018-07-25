/*
 1-Before running this code, make sure to insert CPUcntr.ko module in kernel folder.
 2-You must run this code on CPU_1: #taskset -c 0 ./your_ELF arg
Author = "Alireza Khodamoradi"
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

static inline unsigned int get_cyclecount(void){
  unsigned int value;
  // Read CCNT register
  asm volatile ("MRC p15, 0, %0, c9, c13, 0\n\t" : "=r"(value));
  return value;
}

static inline void init_counters(int32_t do_reset, int32_t enable_divider){
  int32_t value = 1;
  if(do_reset)
    value |= 6; // reset all counters to zero.
  if(enable_divider)
    value |= 8;
  value |= 16;
  // Program the performance-counter control-register
  asm volatile ("MCR p15, 0, %0, c9, c12, 0\n\t" :: "r"(value));
  // Enable all counters
  asm volatile ("MCR p15, 0, %0, c9, c12, 1\n\t" :: "r"(0x8000000f));
  // Clear overflow 
  asm volatile ("MCR p15, 0, %0, c9, c12, 3\n\t" :: "r"(0x8000000f));
}

static inline void pmn_config(unsigned int cntr, unsigned evnt){
  // configure PMSELR register for counter = cntr
  asm volatile ("MCR p15, 0, %0, CRn, CRm, Op2\n\t" :: "r"(cntr));
  // configure PMXEVTYPER for event type = event
  asm volatile ("MCR p15, 0, %0, cRn, cRm, Op2\n\t" :: "r"(evnt));
}

static inline unsigned int pmn_read(unsigned int cntr){
  unsigned int value;
  // configure PMSELR register for counter = cntr
  asm volatile ("MCR p15, 0, %0, cRn, CRm, Op2\n\t" :: "r"(cntr));
  // Read from PMXEVCNTR register and store it to value 
  asm volatile ("MRC p15, 0, %0, cRn, CRm, Op2\n\t" : "=r"(value));
  return value;
}

