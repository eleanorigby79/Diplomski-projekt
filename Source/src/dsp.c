 
 // local includes
#include <dsp.h>

// arm cmsis library includes
#define ARM_MATH_CM4
#define MAX_BUF_SIZE 64000
#include "stm32f4xx.h"
#include <arm_math.h>

// arm c library includes
#include <stdbool.h>

// the user button switch
extern volatile int user_mode;
int old_user_mode;
int length = 0;
int dp = 0;
#define NUM_FIR_TAPS 56
#define BLOCKSIZE    512

// definicija filtarskih koeficjenata
arm_fir_instance_q15 FIR;
q15_t outSignal[BLOCKSIZE];
int16_t buf[BLOCKSIZE];
q15_t fir_coeffs_lp[NUM_FIR_TAPS] = {-30,-27,-21,-9,11,39,74,110,136,142,116,49,-59,-200,-356,
																		-496,-585,-584,-461,-192,228,784,1439,2140,2819,3407,3840,
																		4069,4069,3840,3407,2819,2140,1439,784,228,-192,-461,-584,
																		-585,-496,-356,-200,-59,49,116,142,136,110,74,39,11	-9,-21,-27,-30,};  // low pass at 1KHz with 40dB at 1.5KHz for SR=16KHz
q15_t fir_coeffs_hp[NUM_FIR_TAPS] = { -654,  483,  393,  321,  222,   76, -108, -299, -447,
                                      -501, -422, -200,  136,  520,  855, 1032,  953,  558,
                                      -160,-1148,-2290,-3432,-4406,-5060,27477,-5060,-4406,
                                     -3432,-2290,-1148, -160,  558,  953, 1032,  855,  520,
                                       136, -200, -422, -501, -447, -299, -108,   76,  222,
                                       321,  393,  483, -654,    0,    0,    0,    0,    0,
                                         0,    0,}; // high pass at 1.5KHz with 40dB at 1KHz for SR=16KHz



q15_t fir_state[NUM_FIR_TAPS + BLOCKSIZE];
bool firstStart = false;
																				 

// DSP funkcija
void filter(int16_t* buffer, int length)
{

	// ispitujemo da li je tipka bila stisnuta
	if (user_mode & 1)
	{
		if (firstStart == false || old_user_mode != user_mode)
		{
			initFilter();
			old_user_mode = user_mode;
			firstStart = true;
		}
		
 
	  arm_fir_fast_q15(&FIR, buffer, outSignal, BLOCKSIZE);
		
  	// copy the result
	  arm_copy_q15(outSignal, buffer, length);
  }
}

void delay(int16_t* in,int16_t * out, int Ns){
//"w"contains the delay samples obtained by FIFO read // After computation, the updated "w" will be transferred to circular buffer // by FIFO write.
	static double g = 0.5;
	static double fb = 0.7;


		for(int i = 0; i < Ns; i++) { 
			out[i+ci]=in[i]; //spremaj signal u circ buffer	
				if(del > 0)
						del--;
				
		} 
		ci = (ci + Ns)%32000;	

		
		
	
	if(del == 0) {
		for(int i = 0; i < Ns; i++) {
			buf[i] = out[cd+i];
				
		}
		cd = (cd + Ns)%32000;
		memcpy(in, buf, Ns*sizeof(int16_t));
	}
				//*in = *buf;
}

// inicijalizacija 
void initFilter()
{
  // apply the low pass filter
  if (user_mode & 1)
    arm_fir_init_q15(&FIR, NUM_FIR_TAPS, fir_coeffs_lp, fir_state, BLOCKSIZE);
  // or applay the high pass filter depending on the user button switch mode
  if (user_mode & 2)
    arm_fir_init_q15(&FIR, NUM_FIR_TAPS, fir_coeffs_hp, fir_state, BLOCKSIZE);
}

