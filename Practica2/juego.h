#ifndef JUEGO_H
#define JUEGO_H

#include <iostream>
#include <stdio.h>

#define FILAS 6
#define COLUMNAS 7
#define MATRIZ sizeof(char)*FILAS*COLUMNAS

#define JUGADOR_1 'o'
#define JUGADOR_2 'x'
#define VACIO ' '

#define NO_VALID -1
#define CORRECT 3
#define COLUMNA_LLENA 2
#define COLUMNA_INVALIDA 4

#define CONECTA_VERTICAL 1
#define CONECTA_HORIZONTAL 2
#define CONECTA_DIAGONAL_ASCENDENTE 3
#define CONECTA_DIAGONAL_DESCENDENTE 4
#define NO_CONECTA 0

#define COLUMNA_GANADORA_NO_ENCONTRADA -1

using namespace std;

int conectaVertical(int fila, int columna, char jugador, char tablero[FILAS][COLUMNAS]);
int conectaHorizontal(int fila, int columna, char jugador, char tablero[FILAS][COLUMNAS]);
int conectaDiagonalAscendente(int fila, int columna, char jugador, char tablero[FILAS][COLUMNAS]);
int conectaDiagonalDescendente(int fila, int columna, char jugador, char tablero[FILAS][COLUMNAS]);

int ganador(char jugador, char tablero[FILAS][COLUMNAS]);
int empate(char tablero[FILAS][COLUMNAS]);
int obtenerFilaDesocupada(int columna, char tablero[FILAS][COLUMNAS]);

void limpiarTablero(char tablero[FILAS][COLUMNAS]);
int dibujarTablero(char tablero[FILAS][COLUMNAS]);
int colocarFicha(char jugador, int columna, char tablero[FILAS][COLUMNAS]);

char oponente(char jugador);
int pedirColumna();

void jugar();

#endif
