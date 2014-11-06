/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "defines.h"
#include "screen.h"
#include "mmu.h"

#define INDICE_NO_ENCONTRADO 0



typedef enum direccion_e { IZQ = 0xAAA, DER = 0x441, ADE = 0x83D, ATR = 0x732 } direccion;

unsigned short game_proximo_zombie();

int game_zombieEnEstadoValido(short int coordenadaZombieX, short int coordenadaZombieY);

void game_moverJugadorA(int cantidadPosiciones);

void game_moverJugadorB(int cantidadPosiciones);

int game_obtenerCoordenadaYjugadorA();

int game_obtenerCoordenadaYjugadorB();

void game_actualizarFrame();

void game_cambiarClaseA_atras();

void game_cambiarClaseA_adelante();

void game_lanzar_zombi(unsigned int jugador);

void game_move_current_zombi(direccion dir);


#endif  /* !__GAME_H__ */
