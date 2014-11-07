/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"


unsigned short sched_proximo_indice() {

	unsigned short idle = 0x70;
	unsigned short indice = 0;
	indice = game_proximo_zombie();
	if(indice == INDICE_NO_ENCONTRADO)
	{
		return idle; //corro idle
	}

	indice = indice *8;	
	//para saltar necesito priv 0!!!
	//indice += 3;
	return indice;
}

