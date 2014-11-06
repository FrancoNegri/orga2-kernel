/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"
int zombieEnEstadoValido(short int coordenadaZombieX, short int coordenadaZombieY);

void print(const char * text, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    int i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (unsigned char) text[i];
        p[y][x].a = (unsigned char) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    int i;
    char hexa[8];
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}


//graficos del juego


//define colores
#define ROJO 0x3000
#define AZUL 0x4000
#define VERDE 0x2000
#define NEGRO 0x0000

//define clases de zombies
#define CLERIGO 0x0043
#define MAGO 0x004D
#define GUERRERO 0x0747




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

void inicializarMapa()
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
    return;
}


void actualizarFrame()
{
    int i,j;
    short int *puntero = (short int *) 0xB8000;
    short int (*pixel)[80] = (short int (*)[80]) puntero;

    //borro toda la pantalla
    for(i=0;i< 50; i++ )
    {
        pixel[i][0] = AZUL;
        pixel[i][79] = ROJO;
        for(j=1; j< 79;j++)
        {
            pixel[i][j] = VERDE;
        }
    }

    pixel[coordenadaJugadorAY][0] = claseDeProximoZombieA;
    pixel[coordenadaJugadorBY][79] = claseDeProximoZombieB;


    //para cada zombie
    for(i = 0; i < CANT_ZOMBIS; i++)
    {
        if(zombieEnEstadoValido(coordenadaZombieAX[i],coordenadaZombieAY[i]))
        {
            pixel[coordenadaZombieAX[i]][coordenadaZombieAY[i]] = claseDeZombieA[i];
        }

        if(zombieEnEstadoValido(coordenadaZombieBX[i],coordenadaZombieBY[i]))
        {
            pixel[coordenadaZombieBX[i]][coordenadaZombieBY[i]] = claseDeZombieB[i];
        }
    }
}

int zombieEnEstadoValido(short int coordenadaZombieX, short int coordenadaZombieY)
{
    if(coordenadaZombieX < 79 && coordenadaZombieX > 0)
    {
        if(coordenadaZombieY < 50 && coordenadaZombieY > 0)
        {
            return 1;
        }
    }
    return 0;
}

void moverJugadorA(int cantidadPosiciones)
{
    coordenadaJugadorAY += cantidadPosiciones;
}

void moverJugadorB(int cantidadPosiciones)
{
    coordenadaJugadorBY += cantidadPosiciones;
}

int obtenerCoordenadaYjugadorA()
{
    return coordenadaJugadorAY;
}

int obtenerCoordenadaYjugadorB()
{
    return coordenadaJugadorBY;
}