/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "tss.h"
#include "i386.h"

void mapearDireccionesParaA(int coordenadaJugadorAY,void **direccionReal);

//define colores
#define ROJO 0x3000
#define AZUL 0x4000
#define VERDE 0x2000
#define NEGRO 0x0000
#define GRIS 0x0700


//define clases de zombies
#define CLERIGO 0x0743
#define MAGO 0x074D
#define GUERRERO 0x0747


#define CLOCK_MUERTO 0

#define LIMITE_DEL_TABLERO 44

#define FILA_PARA_NUMERO_DE_ZOMBIE 45
#define FILA_PARA_CLOCKS 47

int coordenadaJugadorAY;
int coordenadaJugadorBY;

short int claseDeProximoZombieA;
short int claseDeProximoZombieB;

int coordenadaZombieAX[CANT_ZOMBIS];
int coordenadaZombieAY[CANT_ZOMBIS];


int coordenadaZombieBX[CANT_ZOMBIS];
int coordenadaZombieBY[CANT_ZOMBIS];

short int claseDeZombieA[CANT_ZOMBIS];
short int claseDeZombieB[CANT_ZOMBIS];

int clockZombieA[CANT_ZOMBIS];
int clockZombieB[CANT_ZOMBIS];

char clockZ[] = {'x', '/', '|', '\\', '-', '/', '|', '\\', '-'};

int puntajeA;
int puntajeB;

int cantidadDeZombiesDisponiblesA;
int cantidadDeZombiesDisponiblesB;

//graficos del juego

void game_inicializarMapa()
{
    int i;
    coordenadaJugadorAY = 25;
    coordenadaJugadorBY = 25;    

    claseDeProximoZombieA = GUERRERO;
    claseDeProximoZombieB = GUERRERO;

    puntajeA=0;
    puntajeB=0;

    cantidadDeZombiesDisponiblesA = CANT_ZOMBIS;
	cantidadDeZombiesDisponiblesB = CANT_ZOMBIS;

    //inicializo los zombies fuera del mapa y en estados invalidos
    for(i = 0; i < CANT_ZOMBIS; i++)
    {
        coordenadaZombieAX[i] = -1; 
        coordenadaZombieAY[i] = -1;
        claseDeZombieA[i] = 0;
        clockZombieA[i] = CLOCK_MUERTO;


        coordenadaZombieBX[i] = -1;
        coordenadaZombieBY[i] = -1;
        claseDeZombieB[i] = 0;
        clockZombieB[i] = CLOCK_MUERTO;
    }
    game_actualizarFrame();
    return;
}


//esta función imprime tooooooodo el mapa
//puede que haya una manera mas performante
//ver cada cuanto hay que llamarla para obtener un buen desempeño
//30 o 60 frames per second? el gran dilema
void game_actualizarFrame()
{
    int i,j;
    short int aux;
    short int *puntero = (short int *) CACHE_VIDEO;
    short int (*pixel)[80] = (short int (*)[80]) puntero;

    short int *punteroABufferDeVideo = (short int *) VIDEO;
    short int (*bufferDeVideo)[80] = (short int (*)[80]) punteroABufferDeVideo;

    //borro toda la pantalla
    //imprimo la parte de arriba
    for( i = 0 ; i < LIMITE_DEL_TABLERO; i++ ) //para pantalla completa es 50
    {
        pixel[i][0] = AZUL;
        pixel[i][79] = ROJO;
        for(j=1; j< 79;j++)
        {
            pixel[i][j] = VERDE;
        }
    }

    //imprimo la parte de abajo
    for( i = LIMITE_DEL_TABLERO ; i < 50; i++ )
    {
        for(j=0; j< 80;j++)
        {
            pixel[i][j] = GRIS;
        }
    }


    pixel[coordenadaJugadorAY][0] = claseDeProximoZombieA;
    pixel[coordenadaJugadorBY][79] = claseDeProximoZombieB;


    //para cada zombie actualizo su posicion
    for(i = 0; i < CANT_ZOMBIS; i++)
    {
        if(game_zombieEnEstadoValido(coordenadaZombieAX[i],coordenadaZombieAY[i]))
        {
            pixel[coordenadaZombieAY[i]][coordenadaZombieAX[i]] = claseDeZombieA[i];
        }

        if(game_zombieEnEstadoValido(coordenadaZombieBX[i],coordenadaZombieBY[i]))
        {
            pixel[coordenadaZombieBY[i]][coordenadaZombieBX[i]] = claseDeZombieB[i];
        }
    }

    for(i = 0; i < CANT_ZOMBIS ; i++)
    {
    	//actualizo los relojes para los zombies A
	    aux = (short int) clockZ[clockZombieA[i]];
	    aux += 0x0700;
	    pixel[FILA_PARA_NUMERO_DE_ZOMBIE][i*3 + 5] = i + 0x0730; //esta constante es para que me quede justo en el ascci apropiado, 0x07 es el color y 0x30 para el caracter numero
	    pixel[FILA_PARA_CLOCKS][i*3 + 5] = aux; 


	    //actializo los relojes para zombies B
	    aux = (short int) clockZ[clockZombieB[i]];
	    aux += 0x0700;
	    pixel[FILA_PARA_NUMERO_DE_ZOMBIE][i*3 + 55] = i + 0x0730; 
	    pixel[FILA_PARA_CLOCKS][i*3 + 55] = aux;	
    }


    //esto es para el puntaje, imprimo un cuadradito rojo y uno azul
    for(i = LIMITE_DEL_TABLERO; i < 50; i++)
    {
    	for(j = 33; j < 38; j++)
    	{
    		pixel[i][j] = AZUL;
    	}
    }

    for(i = LIMITE_DEL_TABLERO; i < 50 ; i++)
    {
    	for(j = 43; j < 48; j++)
    	{
    		pixel[i][j] = ROJO;
    	}
    }

    pixel[47][35] = puntajeA + 0x0730;
    pixel[47][45] = puntajeB + 0x0730;

    pixel[47][30] = cantidadDeZombiesDisponiblesA + 0x0730;
    pixel[47][51] = cantidadDeZombiesDisponiblesB + 0x0730;


    //ahora que tengo actualizado todo, lo paso a la pantalla
    //(me evito el parpadeo fantasma)
    for( i = 0 ; i < 49; i++ ) //la ultima linea la dejo libre para que los relojes hagan lo que quieran
    {
        for(j=0; j< 80;j++) 
        {
            bufferDeVideo[i][j] = pixel[i][j];
        }
    }


}

int game_zombieEnEstadoValido(short int coordenadaZombieX, short int coordenadaZombieY)
{
    if(coordenadaZombieX < 79 && coordenadaZombieX > 0)
    {
        if(coordenadaZombieY < LIMITE_DEL_TABLERO && coordenadaZombieY > 0)
        {
            return 1;
        }
    }
    return 0;
}

void game_moverJugadorA(int cantidadPosiciones)
{
    coordenadaJugadorAY += cantidadPosiciones;
    //para que no se salga del area de juego
     if(coordenadaJugadorAY < 0)
        coordenadaJugadorAY = LIMITE_DEL_TABLERO -1;
    if(coordenadaJugadorAY > LIMITE_DEL_TABLERO - 1)
        coordenadaJugadorAY = 0;
}

void game_moverJugadorB(int cantidadPosiciones)
{
    coordenadaJugadorBY += cantidadPosiciones;
    //esto es para que no se me salga del area de juego
    if(coordenadaJugadorBY < 0)
        coordenadaJugadorBY = LIMITE_DEL_TABLERO -1;
    if(coordenadaJugadorBY > LIMITE_DEL_TABLERO - 1)
        coordenadaJugadorBY = 0;

}

int game_obtenerCoordenadaYjugadorA()
{
    return coordenadaJugadorAY;
}

int game_obtenerCoordenadaYjugadorB()
{
    return coordenadaJugadorBY;
}


//obvio que estas funciones al tener comportamiento similar se pueden extraer en una función unica.
//supongamos que tuve un aneurisma y me olvidé todo lo que se del arte de programar.
void game_cambiarClaseA_atras()
{
    switch(claseDeProximoZombieA)
    {
        case GUERRERO:
            claseDeProximoZombieA = MAGO;
            break;
        case MAGO:
            claseDeProximoZombieA = CLERIGO;
            break;
        case CLERIGO:
            claseDeProximoZombieA = GUERRERO;
            break;
    }
}

void game_cambiarClaseA_adelante()
{
    switch(claseDeProximoZombieA)
    {
        case GUERRERO:
            claseDeProximoZombieA = CLERIGO;
            break;
        case MAGO:
            claseDeProximoZombieA = GUERRERO;
            break;
        case CLERIGO:
            claseDeProximoZombieA = MAGO;
            break;
    }
}

void game_cambiarClaseB_atras()
{
    switch(claseDeProximoZombieB)
    {
        case GUERRERO:
            claseDeProximoZombieB = MAGO;
            break;
        case MAGO:
            claseDeProximoZombieB = CLERIGO;
            break;
        case CLERIGO:
            claseDeProximoZombieB = GUERRERO;
            break;
    }
}

void game_cambiarClaseB_adelante()
{
    switch(claseDeProximoZombieB)
    {
        case GUERRERO:
            claseDeProximoZombieB = CLERIGO;
            break;
        case MAGO:
            claseDeProximoZombieB = GUERRERO;
            break;
        case CLERIGO:
            claseDeProximoZombieB = MAGO;
            break;
    }
}

void game_actualizarClockZombieA(int numeroDeZombie)
{
	if(clockZombieA[numeroDeZombie] != CLOCK_MUERTO)
		clockZombieA[numeroDeZombie]++;
	if(clockZombieA[numeroDeZombie] > 8)
		clockZombieA[numeroDeZombie] = 1;
}

void game_actualizarClockZombieB(int numeroDeZombie)
{
	if(clockZombieB[numeroDeZombie] != CLOCK_MUERTO)
		clockZombieB[numeroDeZombie]++;
	if(clockZombieB[numeroDeZombie] > 8)
		clockZombieB[numeroDeZombie] = 0;
}

#define JUGADORA 1
#define JUGADORB 0

void *direccionReal[9];

void game_lanzar_zombi(unsigned int jugador) 
{
	int i;
	void *cr3;
	if(jugador == JUGADORA)
	{
		for(i=0; i < CANT_ZOMBIS; i++)
		{
			//esto quiere decir que el slot esta vacio, lo uso para el zombie nuevo
			if(clockZombieA[i] == CLOCK_MUERTO)
			{
				cantidadDeZombiesDisponiblesA--;
				//lo saco de su estado muerto
				clockZombieA[i] = 1;
				//calculo las direcciones para A
                
				mapearDireccionesParaA(coordenadaJugadorAY,direccionReal);
				//ahora calculo las paginas realas
				cr3 = mmu_inicializar_zombie(direccionReal, (void*) 0x12000);
				mapearCr3Tss(cr3, &tss_zombisA[i]);
				coordenadaZombieAY[i] = coordenadaJugadorAY;
				coordenadaZombieAX[i] = 1;
				claseDeZombieA[i] = claseDeProximoZombieA;
				break;
			}
		}
	}
}



void mapearDireccionesParaA(int coordenadaJugadorAY,void **direccionReal)
{
	int i;
	for(i = 0; i< 9; i++)
	{
		direccionReal[i] = (void*) 0x400000;
	}
}

void game_move_current_zombi(direccion dir) 
{
}

int proximoTurno = JUGADORA;

unsigned short game_proximo_zombie()
{
	unsigned short i;
	if(proximoTurno == JUGADORA)
	{
		proximoTurno = JUGADORB;
		for(i = 0; i < CANT_ZOMBIS; i++)
		{
			if(clockZombieA[i] != CLOCK_MUERTO)
				return i+15;
		}
	}

	 if(proximoTurno == JUGADORB)
	 {
	 	proximoTurno = JUGADORA;
	 	for(i = 0; i < CANT_ZOMBIS; i++)
	 	{
	 		if(clockZombieB[i] != CLOCK_MUERTO)
	 			return i+23;
	 	}
	 }
	return INDICE_NO_ENCONTRADO;
}