/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"

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

#define LIMITE_DEL_TABLERO 44

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

//graficos del juego

void game_inicializarMapa()
{
    int i;
    coordenadaJugadorAY = 25;
    coordenadaJugadorBY = 25;    

    claseDeProximoZombieA = GUERRERO;
    claseDeProximoZombieB = GUERRERO;

    //inicializo los zombies fuera del mapa y en estados invalidos
    for(i = 0; i < CANT_ZOMBIS; i++)
    {
        coordenadaZombieAX[i] = -1; 
        coordenadaZombieAY[i] = -1;
        claseDeZombieA[i] = 0;


        coordenadaZombieBX[i] = -1;
        coordenadaZombieBY[i] = -1;
        claseDeZombieB[i] = 0;
    }
    game_actualizarFrame();
    return;
}


void game_actualizarFrame()
{
    int i,j;
    short int *puntero = (short int *) 0xB8000;
    short int (*pixel)[80] = (short int (*)[80]) puntero;

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


    //para cada zombie
    for(i = 0; i < CANT_ZOMBIS; i++)
    {
        if(game_zombieEnEstadoValido(coordenadaZombieAX[i],coordenadaZombieAY[i]))
        {
            pixel[coordenadaZombieAX[i]][coordenadaZombieAY[i]] = claseDeZombieA[i];
        }

        if(game_zombieEnEstadoValido(coordenadaZombieBX[i],coordenadaZombieBY[i]))
        {
            pixel[coordenadaZombieBX[i]][coordenadaZombieBY[i]] = claseDeZombieB[i];
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

void game_lanzar_zombi(unsigned int jugador) {
}
void game_move_current_zombi(direccion dir) {
}
