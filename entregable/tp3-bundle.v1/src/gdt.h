/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#ifndef __GDT_H__
#define __GDT_H__

#include "defines.h"

#define GDT_IDX_CDE_LVL_0           8
#define GDT_IDX_CDE_LVL_3           9
#define GDT_IDX_DATA_LVL_0           10
#define GDT_IDX_DATA_LVL_3           11
#define GDT_IDX_VIDEO_LVL_0        12
#define GDT_IDX_TSS_0           13
#define GDT_IDX_TSS_1           14

typedef struct str_gdt_descriptor {
    unsigned short  gdt_length;
    unsigned int    gdt_addr;
} __attribute__((__packed__)) gdt_descriptor;

typedef struct str_gdt_entry {
    unsigned short  limit_0_15;
    unsigned short  base_0_15;
    unsigned char   base_23_16;
    unsigned char   type:4;
    unsigned char   s:1;
    unsigned char   dpl:2;
    unsigned char   p:1;
    unsigned char   limit_16_19:4;
    unsigned char   avl:1;
    unsigned char   l:1;
    unsigned char   db:1;
    unsigned char   g:1;
    unsigned char   base_31_24;
} __attribute__((__packed__, aligned (8))) gdt_entry;

/* Tabla GDT */
extern gdt_entry gdt[];
extern gdt_descriptor GDT_DESC;


#endif  /* !__GDT_H__ */
