/*!@addtogroup ut */
/** @{ */

 /*!@file utils.c
 *@brief Contém funções auxiliares
 *@author Bruno Figueira "Jedi" Lourenço 
 *@date 4 de Setembro
 *@version 0.5 
 */
#include <stdio.h>
#include <stdlib.h>
/*!@brief Aloca memória para <i>nitens</i>, cada um de tamanho 
 * <i>size</i> com a função calloc.
 * 
 * @param nitems
 * @param size
 * @return Ponteiro com o endereço do pedaço de memória alocada 
 * com calloc
 * 
 */
void * 
xcalloc(size_t nitems, size_t size){
	void * aux;
	
	aux =  calloc(nitems,size);
	if (aux == NULL){
		perror("Not enough memory!");
		exit(EXIT_FAILURE);	
	}
	
	return aux;
}
/** @} */
