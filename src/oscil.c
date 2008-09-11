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
 * 
 * @todo Implementar a interpolação cúbica.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "oscil.h"
#include "utils.h"

static int16_t linear_interpolation(oscil * oscillator,double phase);
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

	new_oscil->wavetable = xcalloc(table_length + 2, sizeof(double));
	new_oscil->table_length = table_length;
	new_oscil->sample_rate = sample_rate;
	new_oscil->frequency = 440;
	new_oscil->amplitude = amplitude;
	/*Gera as amostras*/
	for (i = 0; i < table_length + 2; i++){
		new_oscil->wavetable[i] = sin( (2.0*pi*i)/(table_length + 1) );
	}

	return new_oscil;
}
/** Efetua uma interpolação linear com a fase atual do oscilador
 * 
 * @param oscillator 
 * @param phase
 * @return Uma amostra que corresponde à interpolação entre dois pontos 
 * vizinhos da tabela de lookup do oscilador.
 */
static int16_t linear_interpolation(oscil * oscillator,double phase){
	int32_t x1,x2;
	double y1,y2,y;
	
	x1 = (int32_t) phase;
	x2 = (int32_t) phase + 1;

	y1 = oscillator->wavetable[x1];
	y2 = oscillator->wavetable[x2];
	
	y = y1 + (phase - x1)*(y2-y1);
	
	return y*oscillator->amplitude;
}

static int16_t cubic_interpolation(oscil * oscillator, double phase){
	int32_t x1,x2,x3;
	double y1,y2,y3,y;
	/*double a,b,c,d;*/
	
	x1 = (int32_t) phase;
	x2 = (int32_t) phase + 1;
	x3 = (int32_t) phase + 2;
	
	y1 = oscillator->wavetable[x1];
	y2 = oscillator->wavetable[x2];
	y3 = oscillator->wavetable[x3];
	
	return 0;
}


/** Gera as amostras de acordo com as caracterísicas especificadas 
 * pelo oscilador e pelos parâmetros
 * 
 * @param oscillator Oscilador que gerará as amostras
 * @param frequency A frequência desejada
 * @param seconds A duração do som desejado
 * @param inter_type O tipo de interpolação desejada. Pode ser NONE,
 * LINEAR ou CUBIC
 * @return As amostras com as características especificadas
 * 
 * Utiliza-se a técnica de table lookup e espera-se que o oscilador 
 * passado à função tenha sido previamente inicializado com a função 
 * start_oscil .
 * 
 */
int16_t * generate_sample(oscil * oscillator, uint32_t frequency, uint32_t seconds, interpolation_t inter_type){
	int16_t * samples = NULL;
	int64_t i;
	uint32_t num_of_samples;
	double increment;
	double phase = 0.0, previous_phase = 0.0;
	double aux;

	num_of_samples = oscillator->sample_rate*seconds;
	samples = xcalloc(num_of_samples,sizeof(int16_t));
	increment = (double) oscillator->table_length*frequency/oscillator->sample_rate;

	for (i = 0; i < num_of_samples; i++){
		phase = previous_phase + increment;
		/*Pega a parte fracionária da fase*/
		aux = phase - (int32_t) phase;
		
		phase = (int32_t)phase % oscillator->table_length;
		/*
		 * Soma a parte fracionária à fase. Isso só é útil no 
		 * caso de utilizar interpolação
		 */
		phase += aux;
		
		/*if (phase > oscillator->table_length - 1.0){
			phase = phase - oscillator->table_length;	
		}*/
		
		switch(inter_type){
			case NONE:
				samples[i] = oscillator->amplitude * oscillator->wavetable[(int32_t) phase];
				break;
			case LINEAR:	
				samples[i] = linear_interpolation(oscillator,phase);
				break;
			case CUBIC:
				break;
			default:
				printf("Erro! Tipo de interpolação inválida!\n");
				exit(1);
		}
		
		previous_phase = phase;
	}

	return samples;
}
/** @} */
