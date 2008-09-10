/*! @defgroup wave Wave */
/** @{ */

/** 
 * Contém a função write wave para escrever uma amostra como um 
 * arquivo wave
 *
 * 
 * @brief Módulo para escrita de arquivo wave.
 * @file wav.c
 * @author Bruno Figueira Lourenço
 * @author Israel Faria
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "wav.h"
/** Escreve no file_name as amostras com as características adequadas
 * 
 * @param file_name Nome do arquivo a ser criado
 * @param sample_rate Taxa de amostragem
 * @param sample O vetor com as amostras
 * @param sample_duration A duração das amostras em segundos
 * @return @c void
 * 
 */
void write_wave(char * file_name,
				int32_t sample_rate,
				int16_t * sample,
				int16_t sample_duration){
	wav_header file_header = {0x46464952, /*chunkID = RIFF */
							  0x00000000, /*chunksize, a preencher*/ 
							  0x45564157, /*format = WAVE*/
							  0x20746d66, /*subchunk1ID = fmt*/
							  0x00000010, /*subchunk1size = 16*/
							  0x00000001, /*AudioFormat = 1*/
							  0x00000001, /*NumChannel = 1 (Mono)*/  
							  };
	FILE * fp;
	
	fp = fopen(file_name,"wb");
	if (fp == NULL){
		perror("Can't open file!");
		exit(EXIT_FAILURE);
	}
	
	file_header.sample_rate = sample_rate;
	file_header.bits_per_sample = 16;
	/*
	 * byte_rate = sampleRate * numChannels * bitsPerSample/8
	 * Como numChanneles = 1 e bitsPerSample = 16, então
	 * byte_rate = sample_rate*2
	 */
	file_header.byte_rate = sample_rate*2;
	file_header.block_align = 2;
	file_header.subchunk2ID = 0x61746164;/*"data" em ASCII*/
	file_header.subchunk2size = 2*sample_duration*sample_rate;
	file_header.chunk_size = 36 + file_header.subchunk2size;
	
	fwrite(&file_header,sizeof(wav_header),1,fp);
	fwrite(sample,sizeof(int16_t),sample_duration*sample_rate,fp);
	fclose(fp);
					
}
/** @} */
