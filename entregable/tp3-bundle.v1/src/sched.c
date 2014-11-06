/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"


unsigned short sched_proximo_indice() {

	unsigned int indice;
	indice = game_proximo_zombie();
	if(indice != INDICE_NO_ENCONTRADO)
	{
		return 0x70; //corro idle
	}
	indice = indice << 3;
	//aca tengo que acordarme de ponerle prioridad de app, para debug, corro en priv 0
	return indice;
}

