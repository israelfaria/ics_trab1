/** Arquivo que implementa  um oscilador digital
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "oscil.h"
#include "utils.h"

const double pi = 3.14159265358979323846;

oscil * start_oscil(uint32_t table_length, uint32_t sample_rate,
					int16_t amplitude){
	oscil * new_oscil = NULL;
	int32_t i;

	new_oscil = xcalloc(1,sizeof(oscil));

	new_oscil->wavetable = xcalloc(table_length, sizeof(double));
	new_oscil->table_length = table_length;
	new_oscil->sample_rate = sample_rate;
	new_oscil->frequency = 440;
	new_oscil->amplitude = amplitude;

	for (i = 0; i < table_length; i++){
		new_oscil->wavetable[i] = sin( (2.0*pi*i)/(table_length - 1) );
	}

	return new_oscil;
}

int16_t * generate_sample(oscil * oscillator, uint32_t frequency, uint32_t seconds){
	int16_t * samples = NULL;
	int64_t i;
	uint32_t num_of_samples;
	double increment;
	double phase_index = 0.0, previous_phase = 0.0;

	num_of_samples = oscillator->sample_rate*seconds;
	samples = xcalloc(num_of_samples,sizeof(int16_t));
	increment = (double) oscillator->table_length*frequency/oscillator->sample_rate;

	for (i = 0; i < num_of_samples; i++){
		phase_index = oscillator->table_length % ((int) ( previous_phase + increment));
		samples[i] = oscillator->amplitude * oscillator->wavetable[(int) phase_index];
		previous_phase = phase_index;
	}

	return samples;
}
