/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "tss.h"
#include "i386.h"

void mapearZombie(int jugador, int coordenadaX, int coordenadaY, void **direcciones);

#define CANTIDAD_DE_JUGADORES 2

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

//define partes del juego
#define CLOCK_MUERTO 0
#define LIMITE_DEL_TABLERO 44

#define FILA_PARA_NUMERO_DE_ZOMBIE 45
#define FILA_PARA_CLOCKS 47

#define COORDENADA_INICIAL_X_A 2
#define COORDENADA_INICIAL_X_B 77

#define JUGADORA 1
#define JUGADORB 0

int coordenadaJugadorY[CANTIDAD_DE_JUGADORES];

short int claseDeProximoZombie[CANTIDAD_DE_JUGADORES];

int coordenadaZombieX[CANTIDAD_DE_JUGADORES][CANT_ZOMBIS];
int coordenadaZombieY[CANTIDAD_DE_JUGADORES][CANT_ZOMBIS];

short int claseDeZombie[CANTIDAD_DE_JUGADORES][CANT_ZOMBIS];

int clockZombie[CANTIDAD_DE_JUGADORES][CANT_ZOMBIS];

char clockZ[] = {'x', '/', '|', '\\', '-', '/', '|', '\\', '-'};

int puntaje[CANTIDAD_DE_JUGADORES];

int cantidadDeZombiesDisponibles[CANTIDAD_DE_JUGADORES];

//graficos del juego

void game_inicializarMapa()
{
    int i,j;
    for(j = 0; j < CANTIDAD_DE_JUGADORES; j++)
    {
        coordenadaJugadorY[j] = 25;
        claseDeProximoZombie[j] = GUERRERO;
        puntaje[j]=0;
        cantidadDeZombiesDisponibles[j] = CANT_ZOMBIS;       
        //inicializo los zombies fuera del mapa y en estados invalidos
        for(i = 0; i < CANT_ZOMBIS; i++)
        {
            coordenadaZombieX[j][i] = -1; 
            coordenadaZombieY[j][i] = -1;
            claseDeZombie[j][i] = 0;
            clockZombie[j][i] = CLOCK_MUERTO;
        }
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


    pixel[coordenadaJugadorY[JUGADORA]][0] = claseDeProximoZombie[JUGADORA];
    pixel[coordenadaJugadorY[JUGADORB]][79] = claseDeProximoZombie[JUGADORB];


    //para cada zombie actualizo su posicion
    for(i = 0; i < CANT_ZOMBIS; i++)
    {
        for(j = 0; j < CANTIDAD_DE_JUGADORES; j++)
            if(game_zombieEnEstadoValido(coordenadaZombieX[j][i],coordenadaZombieY[j][i]))
            {
                pixel[coordenadaZombieY[j][i]][coordenadaZombieX[j][i]] = claseDeZombie[j][i];
            }
    }

    for(i = 0; i < CANT_ZOMBIS ; i++)
    {
    	//actualizo los relojes para los zombies A
	    aux = (short int) clockZ[clockZombie[JUGADORA][i]];
	    aux += 0x0700;
	    pixel[FILA_PARA_NUMERO_DE_ZOMBIE][i*3 + 5] = i + 0x0730; //esta constante es para que me quede justo en el ascci apropiado, 0x07 es el color y 0x30 para el caracter numero
	    pixel[FILA_PARA_CLOCKS][i*3 + 5] = aux; 


	    //actializo los relojes para zombies B
	    aux = (short int) clockZ[clockZombie[JUGADORB][i]];
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

    pixel[47][35] = puntaje[JUGADORA] + 0x0730;
    pixel[47][45] = puntaje[JUGADORB] + 0x0730;

    pixel[47][30] = cantidadDeZombiesDisponibles[JUGADORA] + 0x0730;
    pixel[47][51] = cantidadDeZombiesDisponibles[JUGADORB] + 0x0730;

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
        if(coordenadaZombieY <= LIMITE_DEL_TABLERO && coordenadaZombieY >= 0)
        {
            return 1;
        }
    }
    return 0;
}

void game_moverJugador(int numeroDeJugador,int cantidadPosiciones)
{
    coordenadaJugadorY[numeroDeJugador] += cantidadPosiciones;
    //para que no se salga del area de juego
    if(coordenadaJugadorY[numeroDeJugador] < 0)
        coordenadaJugadorY[numeroDeJugador] = LIMITE_DEL_TABLERO -1;
    if(coordenadaJugadorY[numeroDeJugador] > LIMITE_DEL_TABLERO - 1)
        coordenadaJugadorY[numeroDeJugador] = 0;
}

void game_cambiarClase_atras(int numeroDeJugador)
{
    switch(claseDeProximoZombie[numeroDeJugador])
    {
        case GUERRERO:
            claseDeProximoZombie[numeroDeJugador] = MAGO;
            break;
        case MAGO:
            claseDeProximoZombie[numeroDeJugador] = CLERIGO;
            break;
        case CLERIGO:
            claseDeProximoZombie[numeroDeJugador] = GUERRERO;
            break;
    }
}

void game_cambiarClase_adelante(int numeroDeJugador)
{
    switch(claseDeProximoZombie[numeroDeJugador])
    {
        case GUERRERO:
            claseDeProximoZombie[numeroDeJugador] = CLERIGO;
            break;
        case MAGO:
            claseDeProximoZombie[numeroDeJugador] = GUERRERO;
            break;
        case CLERIGO:
            claseDeProximoZombie[numeroDeJugador] = MAGO;
            break;
    }
}

void game_actualizarClockZombie(int numeroDeJugador,int numeroDeZombie)
{
	if(clockZombie[numeroDeJugador][numeroDeZombie] != CLOCK_MUERTO)
		clockZombie[numeroDeJugador][numeroDeZombie]++;
	if(clockZombie[numeroDeJugador][numeroDeZombie] > 8)
		clockZombie[numeroDeJugador][numeroDeZombie] = 1;
}


void game_lanzar_zombi(unsigned int jugador) 
{
    void *direccionReal[9];
    int i;
    void *cr3;
    for(i = 0; i < CANT_ZOMBIS; i++)
    {
    //esto quiere decir que el slot esta vacio, lo uso para el zombie nuevo
        if(clockZombie[jugador][i] == CLOCK_MUERTO)
        {
            cantidadDeZombiesDisponibles[jugador]--;
                //lo saco de su estado muerto
            clockZombie[jugador][i] = 1;

            coordenadaZombieY[jugador][i] = coordenadaJugadorY[jugador];
            
            if(jugador == JUGADORA)
                {
                    coordenadaZombieX[jugador][i] = COORDENADA_INICIAL_X_A;
                }
            if(jugador == JUGADORB)
                {
                    coordenadaZombieX[jugador][i] = COORDENADA_INICIAL_X_B;
                }
            claseDeZombie[jugador][i] = claseDeProximoZombie[jugador];

            //calculo las direcciones para A
            //ahora calculo las paginas realas
            mapearZombie(jugador, coordenadaZombieX[jugador][i], coordenadaZombieY[jugador][i], direccionReal);

            if(jugador == JUGADORA)
                {
                    switch(claseDeZombie[jugador][i])
                    {
                        case GUERRERO:
                            cr3 = mmu_inicializar_zombie(direccionReal, (void*) 0x10000);
                            break;
                        case MAGO:
                            cr3 = mmu_inicializar_zombie(direccionReal, (void*) 0x11000);
                            break;
                        case CLERIGO:
                            cr3 = mmu_inicializar_zombie(direccionReal, (void*) 0x12000);
                            break;

                    }
                    mapearCr3Tss(cr3, &tss_zombisA[i]);
                }
            if(jugador == JUGADORB)
                {
                    switch(claseDeZombie[jugador][i])
                    {
                        case GUERRERO:
                            cr3 = mmu_inicializar_zombie(direccionReal, (void*) 0x13000);
                            break;
                        case MAGO:
                            cr3 = mmu_inicializar_zombie(direccionReal, (void*) 0x14000);
                            break;
                        case CLERIGO:
                            cr3 = mmu_inicializar_zombie(direccionReal, (void*) 0x15000);
                            break;
                    }
                    mapearCr3Tss(cr3, &tss_zombisB[i]);
                }
            return;
        }
    }
}



int coordenadas[2][9] = {
                        { 0x401000 + 0x1000*78 , 0x400000 + 0x1000*78 , 0x400000, 0x400000 + 0x1000*78*3 ,0x401000, 0x401000 + 0x1000*78*2 , 0x403000 + 0x1000*78*2 , 0x403000 + 0x1000*78*3 , 0x402000}, 
                        { 0x401000 + 0x1000*78 , 0x402000 + 0x1000*78 , 0x402000 + 0x1000*78*2 , 0x402000, 0x401000 + 0x1000*78*2 , 0x401000, 0x400000 + 0x1000*78, 0x400000 , 0x400000 + 0x1000*78*2}};

void mapearZombie(int jugador, int coordenadaX, int coordenadaY, void ** direccionReal)
{
    int i;
    for(i = 0; i< 9; i++)
    {
        direccionReal[i] = (void*) coordenadas[jugador][i] + (OFFSET_FILA*(coordenadaY-1)) + (0x1000 * (coordenadaX -2));
        
        //chequear que estoy dentro del area de juego!
    }
}

int algo[2][2] = {{ -1, 1 },{1, -1} };

void game_move_current_zombi(direccion dir) 
{
    unsigned int indice_tss = tss_current_tss();
    unsigned int jugador;
    int numeroDeZombie;


    if(indice_tss < OFFSET_ZOMBIS_B)
    {
        numeroDeZombie = indice_tss-15;   
        jugador = JUGADORA;
    }
    else
    {
        jugador = JUGADORB;
        numeroDeZombie = indice_tss - (15+CANT_ZOMBIS);
    }

    //DEPENDE DEL PUTO ZOMBIE
    //uso mmu_bla_bla para mapear
    switch(dir)
    {
        case IZQ:
             coordenadaZombieY[jugador][numeroDeZombie] += algo[jugador][0];
            break;
        case DER:
             coordenadaZombieY[jugador][numeroDeZombie] += algo[jugador][1];
            break;
        case ADE:
             coordenadaZombieX[jugador][numeroDeZombie] += algo[jugador][0];
            break;
        case ATR:
             coordenadaZombieX[jugador][numeroDeZombie] += algo[jugador][1];
            break;
    }

    if(coordenadaZombieX[jugador][numeroDeZombie] > 77)
    {
        clockZombie[jugador][numeroDeZombie] = CLOCK_MUERTO;
        puntaje[JUGADORA]++;
        coordenadaZombieX[jugador][numeroDeZombie] = -1;
        coordenadaZombieY[jugador][numeroDeZombie] = -1;
        //limpiar tss
        return;
    }
     if(coordenadaZombieX[jugador][numeroDeZombie] < 2)
    {
        clockZombie[jugador][numeroDeZombie] = CLOCK_MUERTO;
        puntaje[JUGADORB]++;
        coordenadaZombieX[jugador][numeroDeZombie] = -1;
        coordenadaZombieY[jugador][numeroDeZombie] = -1;
        //limbiar tss
        return;
    }
    if(coordenadaZombieY[jugador][numeroDeZombie] < 0)
    {
        coordenadaZombieY[jugador][numeroDeZombie] = LIMITE_DEL_TABLERO - 1;   
    }
    if(coordenadaZombieY[jugador][numeroDeZombie] > LIMITE_DEL_TABLERO)
    {
        coordenadaZombieY[jugador][numeroDeZombie] = 0;   
    }

    //acá remapeo las direcciones del zombie

    void *direccionReal[9];
    
    mapearZombie(jugador, coordenadaZombieX[jugador][numeroDeZombie], coordenadaZombieY[jugador][numeroDeZombie], direccionReal);

    //en vez de inicializar zombie, modifico la pagina
    //este codigo no tiene que estar acá, pertenece mas a la mmu
    void **cr3;
    unsigned int pTable;
    unsigned int aux = 0x08000000;
    copiarPagina((void**) 0x08000000,(void**) CACHE_TAREA);
    int i;

    cr3 = (void**) rcr3();    
    aux = aux >> 22;
    pTable = (unsigned int) cr3[aux];
    pTable = pTable >> 12;
    pTable = pTable << 12;

    for(i = 0; i <9 ;i++)
    {
        mapearAPagina01(direccionReal[i], (void*) 0x08000000 + 0x1000*i, (void*) pTable, 0x7);   
    }

    tlbflush();

    copiarPagina((void**) CACHE_TAREA,(void**) 0x08000000);
}





int jugadorAlQueLeToca = JUGADORA;
int anteriorZombie[] = {0,0};

unsigned short game_proximo_zombie()
{
	unsigned short i;

    if(jugadorAlQueLeToca == JUGADORA)
    {
        jugadorAlQueLeToca = JUGADORB;
    }
    else
    {
        jugadorAlQueLeToca = JUGADORA;
    }

    i = anteriorZombie[jugadorAlQueLeToca] +1;
    while(1)
    {

        if(i == CANT_ZOMBIS)
            i = 0;
		if(clockZombie[jugadorAlQueLeToca][i] != CLOCK_MUERTO)
        {
            anteriorZombie[jugadorAlQueLeToca] = i;
            game_actualizarClockZombie(jugadorAlQueLeToca, i);
            if(jugadorAlQueLeToca == JUGADORA)
			     return OFFSET_ZOMBIS_A +i;
            if(jugadorAlQueLeToca == JUGADORB)
                 return OFFSET_ZOMBIS_B + i;
        }

        if(i == anteriorZombie[jugadorAlQueLeToca] && clockZombie[jugadorAlQueLeToca][i] == CLOCK_MUERTO)
        {
            return INDICE_NO_ENCONTRADO;
        }
        i++;
    }
	return INDICE_NO_ENCONTRADO;
}