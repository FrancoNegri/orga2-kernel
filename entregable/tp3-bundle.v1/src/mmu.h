/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "defines.h"
#include "i386.h"
#include "tss.h"
#include "game.h"

void copiarPagina(void **src, void **dst);

void pedirPag(void** dir);

void mapearAPagina01(void *direccionReal, void* dirVirtual,void** paginaAUX);

void mapearADirectorio01(void *dir,void* pt, void** pd);

void mmu_inicializar();

void *pedirPagina();

void unmapearAPagina(void* direccionVirtual,void** pagina);

void unmapearADirectorio(void* direccionVirtual,void** direcorio);

#endif	/* !__MMU_H__ */




