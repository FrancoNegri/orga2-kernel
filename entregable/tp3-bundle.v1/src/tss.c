/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss tss_inicial;
tss tss_idle;

tss tss_zombisA[CANT_ZOMBIS];
tss tss_zombisB[CANT_ZOMBIS];

void tss_inicializar() {
		tss_idle.esp0 = 0x27000 ;
		//tss_idle.ss0;// que va???
		tss_idle.esp1 = 0x27000;
		//tss_idle.ss1; // que va???
		tss_idle.esp2 = 0x27000;
		//tss_idle.ss2;// que va???
		tss_idle.cr3 = 0x27000;
		tss_idle.eip = 0x16000;
		tss_idle.eflags = 0x0002;
		tss_idle.esp = 0x27000;
		tss_idle.ebp = 0x27000;
		tss_idle.es = 0x50;
		tss_idle.cs = 0x40;
		tss_idle.ss = 0x50;
		tss_idle.ds = 0x50;
		tss_idle.fs = 0x50;
		tss_idle.gs = 0x50;
		//borramos... ldt

		tss_inicial.esp0 = 0x27000 ;
		//tss_inicial.ss0;// que va???
		tss_inicial.esp1 = 0x27000;
		//tss_inicial.ss1; // que va???
		tss_inicial.esp2 = 0x27000;
		//tss_inicial.ss2;// que va???
		tss_inicial.cr3 = 0x27000;
		tss_inicial.eip = 0x16000;
		tss_inicial.eflags = 0x0002;
		tss_inicial.esp = 0x27000;
		tss_inicial.ebp = 0x27000;
		tss_inicial.es = 0x50;
		tss_inicial.cs = 0x40;
		tss_inicial.ss = 0x50;
		tss_inicial.ds = 0x50;
		tss_inicial.fs = 0x50;
		tss_inicial.gs = 0x50;
		//ver esto e ir a casa
		//por hay esto no es necesario y bla bla bla

	    editarGDT( &gdt[GDT_IDX_TSS_0].base_0_15,&gdt[GDT_IDX_TSS_0].base_23_16,&gdt[GDT_IDX_TSS_0].base_31_24, &tss_inicial);
	    editarGDT( &gdt[GDT_IDX_TSS_1].base_0_15,&gdt[GDT_IDX_TSS_1].base_23_16,&gdt[GDT_IDX_TSS_1].base_31_24, &tss_idle);
}


void editarGDT(unsigned short *base_0_15, unsigned char *base_23_16,unsigned char *base_31_24, void* tss_inicial)
{
	unsigned long int aux = (unsigned long int) tss_inicial;
	aux = aux << 16;
	aux = aux >> 16;
	*base_0_15 = (unsigned short) aux;

	aux = (unsigned long int) tss_inicial;
	aux = aux << 8;
	aux = aux >> 24;
	*base_23_16 = (unsigned char) aux;

	aux = (unsigned long int) tss_inicial;
	aux = aux >> 24;
	*base_31_24 = (unsigned short) aux;
	return;
}
