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
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "oscil.h"
#include "utils.h"

/*Membros públicos deste módulo*/
oscil * start_oscil(uint32_t table_length, uint32_t sample_rate,
					int16_t amplitude, interpolation_t inter_type);
int16_t * generate_sample(oscil * oscillator, uint32_t frequency, uint32_t seconds);

/* Membros privados deste módulo*/
static int16_t linear_interpolation(oscil * oscillator,double phase);
static int16_t cubic_interpolation(oscil * oscillator, double phase);
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
					int16_t amplitude, interpolation_t inter_type){
	oscil * new_oscil = NULL;
	int32_t i;

	new_oscil = xcalloc(1,sizeof(oscil));

	new_oscil->table_length = table_length;
	new_oscil->sample_rate = sample_rate;
	new_oscil->frequency = 440;
	new_oscil->amplitude = amplitude;
	new_oscil->inter_type = inter_type;
	
	/*Gera as amostras*/
	switch(inter_type){
			case NONE:
				new_oscil->wavetable = xcalloc(table_length, sizeof(double));
				for (i = 0; i < table_length; i++){
					new_oscil->wavetable[i] = sin( (2.0*pi*i)/(table_length) );
				}
				break;
			/* A interpolação linear precisa de 1 ponto no final da tabela*/
			case LINEAR:
				new_oscil->wavetable = xcalloc(table_length + 1, sizeof(double));
				for (i = 0; i < table_length + 1; i++){
					new_oscil->wavetable[i] = sin( (2.0*pi*i)/(table_length) );
				}	
				break;
			/*
			 * A interpolação cúbica precisa de 2 pontos no final da tabela
			 * Além disso, precisa de 1 ponto adicional no começo da tabela, 
			 * mas isso será tratado no código de gerar as amostras.
			 */
			case CUBIC:
				new_oscil->wavetable = xcalloc(table_length + 2, sizeof(double));
				for (i = 0; i < table_length + 2; i++){
					new_oscil->wavetable[i] = sin( (2.0*pi*i)/(table_length) );
				}	
				break;
			default:
				printf("Erro! Tipo de interpolação inválida!\n");
				exit(1);
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
/** Efetua a interpolação cúbica com a fase atual do oscilador
 * 
 * @param oscillator
 * @param phase
 * @return Uma amostra que corresponde à interpolação entre 4 pontos 
 * na tabela de lookup do oscilador
 * 
 * Essa função espera que existam 2 pontos extras ao final da tabela.
 * 
 */
static int16_t cubic_interpolation(oscil * oscillator, double phase){
	int32_t x0,x1,x2,x3;
	double y,y0,y1,y2,y3;
	double frac;
	
	x1= (int32_t) phase;
	x0 = x1 - 1;
	x2 = x1 + 1;
	x3 = x1 + 2;
	frac = phase - x1;
	
	y1 = oscillator->wavetable[x1];
	y2 = oscillator->wavetable[x2];
	y3 = oscillator->wavetable[x3];
	
	if (x1 < 0 ){
		y0 = oscillator->wavetable[oscillator->table_length - 1];	
	}
	else{
		y0 = oscillator->wavetable[x1 - 1];	
	}

	/*Apenas divide a fórmula em duas partes para facilitar a leitura*/
	y = -frac*(frac-1)*(frac-2)*y0/6.0 + (frac+1)*(frac-1)*(frac-2)*y1/2.0;
	y += -(frac+1)*(frac)*(frac-2)*y2/2.0 + (frac+1)*(frac)*(frac-1)*y3/6.0;
	
	
	return y*oscillator->amplitude;
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
int16_t * generate_sample(oscil * oscillator, uint32_t frequency, uint32_t seconds){
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
		
		switch(oscillator->inter_type){
			case NONE:
				samples[i] = oscillator->amplitude * oscillator->wavetable[(int32_t) phase];
				break;
			case LINEAR:	
				samples[i] = linear_interpolation(oscillator,phase);
				break;
			case CUBIC:
				samples[i] = cubic_interpolation(oscillator,phase);
				break;
			default:
				printf("Erro! Tipo de interpolação inválida!\n");
				exit(1);
		}
		
		previous_phase = phase;
	}

	return samples;
}
/** Desaloca a memória alocada por um oscilador
 * 
 * @param oscillator
 * @return @c void
 * 
 */
void free_oscillator(oscil ** oscillator){
	free((*oscillator)->wavetable);
	free(*oscillator);	
	*oscillator = NULL;
}
/** @} */
