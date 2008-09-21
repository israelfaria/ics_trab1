/*! @addtogroup wave */
/** @{ */

/** 
 * Módulo para escrita de arquivo wav - Interface
 *
 * 
 * @brief Cabeçalho para as funções de escrita de arquivo wav.
 * @file wav.h
 * @author Bruno Figueira Lourenço
 * @author Israel Faria
 */

#ifndef __WAV_H
#define __WAV_H
	#include <stdint.h>
	
	typedef struct wav_header_t wav_header;
	
	/**
	 * Header do arquivo wav
	 */
	struct wav_header_t{
		int32_t chunkID;
		int32_t chunk_size;
		int32_t format;
		int32_t subchunk1ID;
		int32_t subchunk1size;
		int16_t audio_format;
		int16_t num_channels;
		int32_t sample_rate;
		int32_t byte_rate;
		int16_t block_align;
		int16_t bits_per_sample;
		int32_t subchunk2ID;
		int32_t subchunk2size;
	};
	
	extern	void write_wave(char * file_name,
				int32_t sample_rate,
				int16_t * sample,
				int16_t sample_duration);
#endif
/** @} */
