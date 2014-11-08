/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/
#include "mmu.h"

//para testear si salta bien
//typedef void (*func_ptr)(void);

void* dirGlobal;
int contadorDePaginas;

void mmu_inicializar() 
{
		dirGlobal = (void*) PAGINAS_LIBRES_MMU; //defines.h
		contadorDePaginas = 0;
		return;
}

void *pedirPagina()
{
	dirGlobal = dirGlobal +0x01000;
	if( contadorDePaginas < LIMITE_PAGINAS_LIBRES_MMU)
	{
		contadorDePaginas++;
		pedirPag((void**)dirGlobal);
		return dirGlobal;		
	}
	else
	{
		int d = 1;
		__asm __volatile(
        "mov %0, %%eax \n"
        "int $99     \n"
        : /* no output*/
        : "m" (d)
        : "eax"
    );
	}
	return 0x0;
}

void *mmu_inicializar_zombie(void** direccionReal, void* codigo)
{
	//no puedo usar el cr3 harcodeado, porque al ser llamada esta función de diferentes tareas,
	//el cr3 va a ir cambiando, y no voy a estar mapeandolo bien

	void * cr3Actual = (void*) rcr3();

	int i;
	void *direccionDelDirectorio = pedirPagina();
	void *direccionDePagina= pedirPagina();



	mapearADirectorio01((void*)0x0,(void*)0x28000, direccionDelDirectorio, 0x3);

	//mapearAPagina01((void*)0x0, (void*)0x0, (void*)0x28000, 0x7);

	for(i = 0; i < 9; i++)
	{
		void *direccionVirtual = (void*) 0x08000000 + 0x1000*i;
		mapearAPagina01(direccionReal[i], direccionVirtual, direccionDePagina, 0x7 ); //ponr bien las drecciones reales
	}

	mapearADirectorio01((void*)0x08000000,direccionDePagina, direccionDelDirectorio, 0x7);	

	//copio la tarea en el mapa

	void *paginaAUX= pedirPagina();

	mapearAPagina01(direccionReal[0], (void*) 0xDC4000, paginaAUX, 0x3);

	mapearADirectorio01((void*) 0xDC4000, paginaAUX, cr3Actual, 0x3);

	//0xDC4000
	copiarPagina( (void**)codigo, (void**)0xDC4000);

	//void * p = (void*) codigo;

	//func_ptr fun;
	//fun = p;
	//fun();

	unmapearAPagina((void*)0xDC4000, paginaAUX);
	unmapearADirectorio((void*)0xDC4000, cr3Actual);

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

void mapearAPagina01(void *direccionReal, void* dirVirtual,void** paginaAUX, unsigned char flags)
{
	long int aux1 = (long int) dirVirtual;
	long int aux2 = (long int) direccionReal;
	aux1 = aux1 << 10;
	aux1 = aux1 >> 22;

	aux2 = aux2 >> 12;
	aux2 = aux2 << 12;
	aux2 += flags; //pagina de usuario
	paginaAUX[aux1]= (void*) aux2;
	return;
}

void mapearADirectorio01(void *dir,void* pt, void** pd , unsigned char flags)
{
	long int aux1 = (long int) dir;
	long int aux2 = (long int) pt;
	aux1 = aux1 >> 22;
	aux2 = aux2 >> 12;
	aux2 = aux2 << 12;
	aux2 += flags;
	pd[aux1]= (void*) aux2;
	return;
}
void unmapearAPagina(void* direccionVirtual,void** pagina)
{
	long int aux1 = (long int) direccionVirtual;
	aux1 = aux1 << 10;
	aux1 = aux1 >> 22;

	pagina[aux1]= (void*) 0x2;
	return;
}

void unmapearADirectorio(void* direccionVirtual,void** direcorio)
{
	long int aux1 = (long int) direccionVirtual;
	aux1 = aux1 >> 22;
	direcorio[aux1]= (void*) 0x2;
	return;
}

/*para mapear las paginas del zombie, vamos a tener que tomar el ancho de fila (78 x 2^10 (1 kb) * 4 )*/