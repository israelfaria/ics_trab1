/*! @defgroup oscil Oscilador */
/** @{ */

/** 
 * Arquivo que implementa  um oscilador digital
 *
 * 
 * @brief Oscilador digital com table lookup.
 * @file oscil.c
 * @author Bruno Figueira Lourenço
 * @author Israel Faria
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "oscil.h"
#include "utils.h"

static const double pi = 3.14159265358979323846;
/**Inicializa um oscilador
 * 
 * @param table_length Tamanho da tabela utilizada para table lookup
 * @param sample_rate Taxa de amostragem
 * @param amplitude A amplitude
 * @return Um ponteiro para um novo oscilador
 * 
 */
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
	/*Gera as amostras*/
	for (i = 0; i < table_length; i++){
		new_oscil->wavetable[i] = sin( (2.0*pi*i)/(table_length - 1) );
	}

	return new_oscil;
}
/** Gera as amostras de acordo com as caracterísicas especificadas 
 * pelo oscilador e pelos parâmetros
 * 
 * @param oscillator Oscilador que gerará as amostras
 * @param frequency A frequência desejada
 * @param seconds A duração do som desejado
 * @return As amostras com as características especificadas
 * 
 * Utiliza-se a técnica de table lookup e espera-se que o oscilador 
 * passado à função tenha sido previamente inicializado com a função 
 * start_oscil .
 * 
 */
int16_t * generate_sample(oscil * oscillator, uint32_t frequency, uint32_t seconds){
	int16_t * samples = NULL;
	int64_t i;
	uint32_t num_of_samples;
	double increment;
	double phase = 0.0, previous_phase = 0.0;
	int32_t phase_index;

	num_of_samples = oscillator->sample_rate*seconds;
	samples = xcalloc(num_of_samples,sizeof(int16_t));
	increment = (double) oscillator->table_length*frequency/oscillator->sample_rate;

	for (i = 0; i < num_of_samples; i++){
		phase = previous_phase + increment;
		
		if (phase < oscillator->table_length - 1.0){
			phase_index =(int32_t) phase;			
		}
		else{
			phase_index = (int32_t) oscillator->table_length - phase;
			phase = oscillator->table_length - phase;
		}
	
		samples[i] = oscillator->amplitude * oscillator->wavetable[(int) phase_index];
		previous_phase = phase;
		printf("%d\t%d\n",samples[i],phase_index);
	}

	return samples;
}
/** @} */
