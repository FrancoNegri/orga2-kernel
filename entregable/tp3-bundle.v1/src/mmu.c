/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/
#include "mmu.h"

void* dirGlobal;
void copiarPagina(void **src, void **dst);
typedef void (*func_ptr)(void);
void pedirPag(void** dir);
void mapearAPagina01(void *direccionReal, void* dirVirtual,void** paginaAUX);
void mapearADirectorio01(void *dir,void* pt, void** pd);
void mmu_inicializar();

void mmu_inicializar() 
{
		dirGlobal = (void*) 0x100000;
		return;
}

void *pedirPagina()
{
	dirGlobal = dirGlobal +0x01000;
	pedirPag((void**)dirGlobal);
	return dirGlobal;
}

void *mmu_inicializar_zombie(void* direccionReal, void* codigo)
{
	int i;
	void *direccionDelDirectorio = pedirPagina();
	void *direccionDePagina= pedirPagina();

	mapearADirectorio01((void*)0x0,(void*)0x28000, direccionDelDirectorio);

	mapearAPagina01((void*)0x0, (void*)0x0, (void*)0x28000);

	for(i = 0; i < 9; i++)
	{
		void *direccionVirtual = (void*) 0x400000 + 0x1000*i;
		mapearAPagina01(direccionReal, direccionVirtual, direccionDePagina); //ponr bien las drecciones reales
	}

	//copio la tarea en el mapa

	void *paginaAUX= pedirPagina();

	mapearAPagina01(direccionReal, (void*) 0xDC4000, paginaAUX);

	mapearADirectorio01((void*) 0xDC4000, paginaAUX, (void*)0x27000);

	//0xDC4000
	//copiarPagina( (void**)codigo, (void**)0xDC4000);

	//void * p = (void*) codigo;

	//func_ptr fun;
	//fun = p;
	//fun();

	unmapearAPagina();	

	return direccionDelDirectorio;
}

void copiarPagina(void **src, void **dst)
{
	int i;
	for(i = 0; i < 1024; i++)
		dst[i] = src[i];
	return;
}


void pedirPag(void** dir)
{
	int i;
	void *aux =(void*) 0x00000002;
	for(i = 0; i < 1024; i++)
		dir[i] = aux;
	return;
}

void mapearAPagina01(void *direccionReal, void* dirVirtual,void** paginaAUX)
{
	long int aux1 = (long int) dirVirtual;
	long int aux2 = (long int) direccionReal;
	aux1 = aux1 << 10;
	aux1 = aux1 >> 22;

	aux2 = aux2 >> 12;
	aux2 = aux2 << 12;
	aux2 += 3;
	paginaAUX[aux1]= (void*) aux2;
	return;
}

void mapearADirectorio01(void *dir,void* pt, void** pd)
{
	long int aux1 = (long int) dir;
	long int aux2 = (long int) pt;
	aux1 = aux1 >> 22;
	aux2 = aux2 >> 12;
	aux2 = aux2 << 12;
	aux2 += 3;
	pd[aux1]= (void*) aux2;
	return;
}
void unmapearAPagina(void* direccionVirtual,void* pagina);

void unmapearADirectorio(void* direccionVirtual,void* direcorio);

/*para mapear las paginas del zombie, vamos a tener que tomar el ancho de fila (78 x 2^10 (1 kb) * 4 )*/