
#ifndef __DSP_H
#define __DSP_H

#include <stdint.h>
#include "timer.h"
void filter(int16_t* buffer, int length);
void delay(int16_t* buffer,int16_t* out, int Ns);
void initFilter(void);

extern int del, ci, cd;



#endif
