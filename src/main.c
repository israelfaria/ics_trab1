#include <stdio.h>
#include <stdlib.h>

#include "wav.h"
#include "oscil.h"

const uint32_t TABLE_LENGTH = 2048;
const uint32_t SAMPLE_RATE = 44100;
const uint32_t AMPLITUDE = 30000;

int main(int argc, char *argv[]){
	uint32_t freq;
	uint32_t sec;
	oscil * osc;
	int16_t * samples;
	
	if (argc != 4){
		printf("O programa recebe três argumentos: \n");
		printf("frequência duração nome_do_arquivo_de_saida\n");
		exit(EXIT_SUCCESS);
	}

	freq = atoi(argv[1]);
	sec = atoi(argv[2]);

	osc = start_oscil(TABLE_LENGTH,SAMPLE_RATE,AMPLITUDE);
	samples = generate_sample(osc,freq,sec);

	write_wave(argv[3],SAMPLE_RATE,samples,sec);

	return 0;	
}
