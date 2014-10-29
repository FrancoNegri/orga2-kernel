/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/
#include "mmu.h"
#include "paging.h"

void* dirGlobal;
void mmu_inicializar() 
{
		dirGlobal = (void*) 0x100000;
		return;
}

void *pedirPagina()
{
	dirGlobal = dirGlobal +0x01000;
	pedirPagina_0(dirGlobal);
	return dirGlobal;
}


void *mmu_inicializar_zombie(void* direccionReal, void* codigo)
{
	int i;
	void *direccionDelDirectorio = pedirPagina();
	void *direccionDePagina= pedirPagina();

	mapearPagina((void*)0x0, (void*)0x0, direccionDelDirectorio, (void*)0x28000);

	for(i = 0; i < 9; i++)
	{
		void *direccionVirtual = (void*) 0x400000 + 0x1000*i;
		mapearPagina(direccionReal, direccionVirtual, direccionDelDirectorio, direccionDePagina); //ponr bien las drecciones reales
	}
	return direccionDelDirectorio;

}




/*para mapear las paginas del zombie, vamos a tener que tomar el ancho de fila (78 x 2^10 (1 kb) * 4 )*/