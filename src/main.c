#include <stdio.h>
#include <stdlib.h>

#include "wav.h"
#include "oscil.h"

const uint32_t TABLE_LENGTH = 1024;
const uint32_t SAMPLE_RATE = 44100;
const uint32_t AMPLITUDE = 30000;

int main(int argc, char *argv[]){
	uint32_t freq;
	uint32_t sec;
	int16_t * samples;
	oscil * osc;
	interpolation_t inter_type;
	
	if (argc != 5){
		printf("O programa recebe 4 argumentos: \n");
		printf("frequência duração tipo_de_interpolação arquivo_de_saida\n");
		printf("O tipo da interpolação é 0 para nenhum, 1 para linear e 2 para cúbica");
		exit(EXIT_SUCCESS);
	}

	freq = atoi(argv[1]);
	sec = atoi(argv[2]);
	inter_type = atoi(argv[3]);
	
	osc = start_oscil(TABLE_LENGTH,SAMPLE_RATE,AMPLITUDE);
	samples = generate_sample(osc,freq,sec,inter_type);

	write_wave(argv[4],SAMPLE_RATE,samples,sec);

	return 0;	
}
