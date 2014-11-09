/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1


/* Misc */
/* -------------------------------------------------------------------------- */
#define CANT_ZOMBIS             8

#define SIZE_W                  78
#define SIZE_H                  44


/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 32

#define GDT_IDX_NULL_DESC           0

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC      << 3)

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define VIDEO                   0x000B8000 /* direccion fisica del buffer de video */
#define PAGINAS_LIBRES_MMU		0x100000 //500 paginas disponibles (hacia arriba)
#define CACHE_VIDEO				0x300000 // lo uso para posicionar todo adecuadamente antes de ponerlo en pantalla
#define CACHE_TAREA				0x301000
#define PAGINAS_LIBRES_TSS		0x400000 // 254 paginas disponibles (hacia abajo)


#define LIMITE_PAGINAS_LIBRES_MMU 256*2
#define LIMITE_PAGINAS_LIBRES_TSS 255 //la pagina 0x300000 la uso para video

#define COMIENZO_DEL_MAPA 0x400000
#define OFFSET_FILA 0x1000*SIZE_W 
#define TAMANIO_DEL_MAPA OFFSET_FILA*SIZE_H
#define FIN_DEL_MAPA COMIENZO_DEL_MAPA+TAMANIO_DEL_MAPA

#define OFFSET_ZOMBIS_A 15
#define OFFSET_ZOMBIS_B 15+CANT_ZOMBIS

#endif  /* !__DEFINES_H__ */
