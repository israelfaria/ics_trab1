#ifndef __WAV_H
#define __WAV_H
	#include <stdint.h>
	
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
		int32_t subchunk2size	
	}
#endif
