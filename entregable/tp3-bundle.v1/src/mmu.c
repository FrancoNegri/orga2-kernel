/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/
#include "mmu.h"
#include "paging.h"

int contadorDePaginas;
int contadorDeDirectorios;

void mmu_inicializar() 
	{
		contadorDePaginas = 0;
		contadorDeDirectorios = 0;
		return;
	}

void *mmu_inicializar_zombie()
{
	int i;
	int aux = 0x12000 + contadorDeDirectorios * 0x1000;
	void *direccionDelDirectorio = (void*) aux;

	aux = 0x13000 + 0x1000 * contadorDePaginas;
	void *direccionDePagina = (void*) aux;

	contadorDeDirectorios++;

	crearPageDirectory(direccionDelDirectorio);

	for(i = 0; i < 9; i++)
	{
		aux = aux + 0x1000;
		direccionDePagina = (void*) aux;
		crearPageTable(direccionDePagina);

		//direccion real??
		void *direccionReal = (void*) 0x1; //no se!
		void *direccionVirtual = (void*) 0x0;
		mapearPagina(direccionReal, direccionVirtual, direccionDelDirectorio, direccionDePagina);
	}
	return direccionDelDirectorio;
}




