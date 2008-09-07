/*!@brief Módulo de Utilidades
 *@defgroup ut Utils
 *@{
 *	
 *@file utils.h.h
 *@brief Declaração de estruturas de arquivo e de funções auxiliares
 *@author Bruno Figueira "Jedi" Lourenço 
 *@date 30 de maio
 *@version 0.1 
 * 
 */
#ifndef UTILS_H
#define UTILS_H

	#include <stdio.h>
	#include <stdint.h>

	
	typedef struct file_type file_t;
	/*!@brief Um struct que representa o conteúdo 
	 * de um arquivo em memória
	 */
	struct file_type{
		/*!@brief Conteúdo do arquivo*/
		uint8_t * contents;
		/*!@brief Tamanho do arquivo*/
		uint32_t size;
	};
	
	extern void * xcalloc(size_t nitems, size_t size);
	extern file_t * read_file(char * file_name);
	

#endif
/** @} */
