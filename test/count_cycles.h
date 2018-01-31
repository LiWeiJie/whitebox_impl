/*
 * @Author: Weijie Li 
 * @Date: 2018-01-31 15:25:42 
 * @Last Modified by: Weijie Li
 * @Last Modified time: 2018-01-31 15:43:33
 */
#include<stdio.h>

#define get_current_cycles rdtsc

static __inline__ unsigned long long rdtsc(void)
{
    unsigned long long int x;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
}

static unsigned long long _start_cycles = 0;
static unsigned long long _end_cycles = 0;

static void set_cycles_start() {
    _start_cycles = get_current_cycles();
}

static unsigned long long set_cycles_ends() {
    _end_cycles = get_current_cycles();
}

/**
 * @brief return the elapsed cycles from last call of from set_cycles_start() to set_cycles_ends()
 * 
 * @return unsigned long long 
 */
static unsigned long long get_cycles_elapsed() {
    return _end_cycles -_start_cycles;
}