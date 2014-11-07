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
	
	//return 0x7B;
	indice = indice *8;
	indice += 3;
	//aca tengo que acordarme de ponerle prioridad de app, para debug, corro en priv 0
	return indice;
}

