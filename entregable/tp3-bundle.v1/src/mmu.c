/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/
#include "mmu.h"
#include "paging.h"

void* dirGlobal;
void copiarPagina(void **src, void **dst);

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

typedef void (*func_ptr)(void);

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


	//copio la tarea en el mapa
	void *paginaAUX= pedirPagina();
	mapearPagina(direccionReal, (void*) 0xDC4000 , (void*)0x27000, paginaAUX);
	//0xDC4000
	copiarPagina( (void**)codigo, (void**)0xDC4000);

	void * p = (void*) 0x16000;

	func_ptr fun;
	fun = p;
	fun();
	
	unmapearPagina( (void*)0xDC4000, (void*)0x27000,paginaAUX);

	return direccionDelDirectorio;
}

void copiarPagina(void **src, void **dst)
{
	int i;
	for(i = 0; i < 1024; i++)
		dst[i] = src[i];
	return;
}


/*para mapear las paginas del zombie, vamos a tener que tomar el ancho de fila (78 x 2^10 (1 kb) * 4 )*/