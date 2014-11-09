/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "mmu.h"


unsigned int pedirPaginaVacia();

tss tss_inicial;
tss tss_idle;

unsigned int proximaPaginaVacia;
unsigned int contadorPaginasVacias;

void tss_inicializar() {
		tss_idle.esp0 = 0x27000 ;
		tss_idle.ss0 = 0x50;// que va???
		tss_idle.esp1 = 0x27000;
		//tss_idle.ss1; // que va???
		tss_idle.esp2 = 0x27000;
		//tss_idle.ss2;// que va???
		tss_idle.cr3 = 0x27000;
		tss_idle.eip = 0x16000;
		tss_idle.eflags = 0x0202;
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
		tss_inicial.eip = 0x17000;
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
		//por hay esto no es necesario porque solo se va a usar para guardar el estado 
		//de la cpu al saltar a la primera tarea
		proximaPaginaVacia = PAGINAS_LIBRES_TSS;
		contadorPaginasVacias = 0;


	    editarGDT( &gdt[GDT_IDX_TSS_0].base_0_15,&gdt[GDT_IDX_TSS_0].base_23_16,&gdt[GDT_IDX_TSS_0].base_31_24, &tss_inicial);
	    editarGDT( &gdt[GDT_IDX_TSS_1].base_0_15,&gdt[GDT_IDX_TSS_1].base_23_16,&gdt[GDT_IDX_TSS_1].base_31_24, &tss_idle);
}


void cargarTSS_zombie()
{
	int i;
	for(i = 15; i < GDT_COUNT; i++)
	{
		gdt[i].limit_0_15 = 0x68;
		gdt[i].base_0_15 = 0x0;
		gdt[i].base_23_16 = 0x0;
		gdt[i].type = 0x09;
		gdt[i].s = 0x0;
		gdt[i].dpl = 0x0; //esto no tendría que ser 3? -> NO
		gdt[i].p = 0x1;
		gdt[i].limit_16_19 = 0x00;
		gdt[i].avl = 0x0;
		gdt[i].l = 0x0;
		gdt[i].db = 0x1;
		gdt[i].g = 0x0;
		gdt[i].base_31_24 = 0x0;
	}

	for(i = 0; i < CANT_ZOMBIS; i++)
	{
		
		tss_zombisA[i].esp0 = pedirPaginaVacia();
		tss_zombisA[i].ss0 = 0x50;// que va???
		//tss_zombisA[i].esp1 = 0x27000;
		//tss_idle.ss1 = 0x50;
		//tss_zombisA[i].esp2 = 0x27000;
		//tss_idle.ss2 = 0x50;
		tss_zombisA[i].cr3 = 0x00000; //se completa cuando creamos un zombie nuevo
		tss_zombisA[i].eip = 0x08000000;
		tss_zombisA[i].eflags = 0x202;
		tss_zombisA[i].esp = 0x08001000;
		tss_zombisA[i].ebp = 0x08001000;
		tss_zombisA[i].es = 0x5B;
		tss_zombisA[i].cs = 0x4B;
		tss_zombisA[i].ss = 0x5B;
		tss_zombisA[i].ds = 0x5B;
		tss_zombisA[i].fs = 0x5B;
		tss_zombisA[i].gs = 0x5B;
		editarGDT(&gdt[i+15].base_0_15, &gdt[i+15].base_23_16, &gdt[i+15].base_31_24, &tss_zombisA[i]);
	}

	//01011011
	for(i = 0; i < CANT_ZOMBIS; i++)
    {
        tss_zombisB[i].esp0 = pedirPaginaVacia();
		tss_zombisB[i].ss0 = 0x50;// que va???
		//tss_zombisB[i].esp1 = 0x27000;
		//tss_idle.ss1 = 0x50;
		//tss_zombisB[i].esp2 = 0x27000;
		//tss_idle.ss2 = 0x50;
		tss_zombisB[i].cr3 = 0x00000; //se completa cuando creamos un zombie nuevo
		tss_zombisB[i].eip = 0x08000000;
		tss_zombisB[i].eflags = 0x202;
		tss_zombisB[i].esp = 0x08001000;
		tss_zombisB[i].ebp = 0x08001000;
		tss_zombisB[i].es = 0x5B;
		tss_zombisB[i].cs = 0x4B;
		tss_zombisB[i].ss = 0x5B;
		tss_zombisB[i].ds = 0x5B;
		tss_zombisB[i].fs = 0x5B;
		tss_zombisB[i].gs = 0x5B;
        editarGDT(&gdt[i+23].base_0_15, &gdt[i+23].base_23_16, &gdt[i+23].base_31_24, &tss_zombisB[i]);
    }
}

//1011000

void editarGDT(unsigned short *base_0_15, unsigned char *base_23_16,unsigned char *base_31_24, void* tss)
{
	unsigned  int aux = (unsigned  int) tss;
	aux = aux << 16;
	aux = aux >> 16;
	*base_0_15 = (unsigned short) aux;

	aux = (unsigned  int) tss;
	aux = aux << 8;
	aux = aux >> 24;
	*base_23_16 = (unsigned char) aux;

	aux = (unsigned int) tss;
	aux = aux >> 24;
	*base_31_24 = (unsigned char) aux;
	return;
}

unsigned int tss_current_tss()
{
	unsigned int i;
	for(i = 15; i< GDT_COUNT; i++)
	{
		//1011 -> la tarea esta busy
		if(gdt[i].type == 11)
		{
			return i;
		}
	}
	return 0;
}



void mapearCr3Tss(void *cr3, tss *someTss)
{
	(*someTss).cr3 = (unsigned int) cr3;
}

unsigned int pedirPaginaVacia()
{
	if(contadorPaginasVacias < LIMITE_PAGINAS_LIBRES_TSS)
	{
		contadorPaginasVacias++;
		proximaPaginaVacia -= 0x1000;
		return proximaPaginaVacia;		
	}
	else
	{
		int d = 2;
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
//la tarea tiene rpl = 3
//el nivel con el que quiero pedir para axceder es nivel 3?
//tss de las tareas -> tiene un nivel de privilegio
//dpl = 0, sinó las tareas podrían hacer task switch
//tres cosas con dpl = 3 sd y cs entrada a la idt para mover.
//esp0 = nueva pagina libre (pero al final o sea +1000)
//ss0 es el selector de segmento de nivel 0 -> rpl 0