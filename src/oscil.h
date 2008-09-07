#ifndef __OSCIL_H
#define __OSCIL_H
	#include <stdint.h>

	typedef struct oscil_t oscil;
	struct oscil_t{
		uint32_t table_length;
		uint32_t sample_rate;
		uint32_t frequency;
		int16_t amplitude;
		double * wavetable;
	};
	
	extern oscil * start_oscil(uint32_t table_length, uint32_t sample_rate,
					int16_t amplitude);
	extern int16_t * generate_sample(oscil * oscillator, uint32_t frequency,
					uint32_t seconds);
#endif
