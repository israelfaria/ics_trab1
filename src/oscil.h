/*! @addtogroup oscil*/
/** @{ */
/** 
 * Arquivo que contém as definições das structs e os protótipos das 
 * funções do oscilador
 *
 * 
 * @brief Oscilador digital com table lookup - Header.
 * @file oscil.h
 * @author Bruno Figueira Lourenço
 * @author Israel Faria
 */


#ifndef __OSCIL_H
#define __OSCIL_H
	#include <stdint.h>

	typedef struct oscil_t oscil;
	/**
	 * Um oscilador digital feito com a técnica de table lookup
	 */
	struct oscil_t{
		uint32_t table_length;
		uint32_t sample_rate;
		uint32_t frequency;
		int16_t amplitude;
		double * wavetable;
	};

	/**
	 * Enum que contém os tipos de interpolação disponíveis. Por enquanto, 
	 * a interpolação cúbica ainda não está implementada.
	 */
	enum interpolation_t{
		NONE = 0,
		LINEAR,
		CUBIC
	};
	
	typedef enum interpolation_t interpolation_t;
	
	extern oscil * start_oscil(uint32_t table_length, uint32_t sample_rate,
					int16_t amplitude);
	extern int16_t * generate_sample(oscil * oscillator,
									 uint32_t frequency,
									 uint32_t seconds,
									 interpolation_t inter_type);
#endif

/** @} */
