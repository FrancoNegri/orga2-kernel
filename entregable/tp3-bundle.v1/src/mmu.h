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

void mapearAPagina01(void *direccionReal, void* dirVirtual,void** paginaAUX, unsigned char flags);

void mmu_inicializar();

void *pedirPagina();

void unmapearAPagina(void* direccionVirtual,void** pagina);

void mapearADirectorio01(void *dir,void* pt, void** pd, unsigned char flags);

void unmapearADirectorio(void* direccionVirtual,void** direcorio);

void *mmu_inicializar_zombie(void** direccionReal, void* codigo);

void mmu_remapearPaginasZombie(void** direccionReal);

#endif	/* !__MMU_H__ */




