#include "juego.h"

//FUNCIONES PARA LA COMPROBACIÓN DE ALINEACIÓN GANADORA

int conectaVertical(int columna, int fila, char jugador, char tablero[FILAS][COLUMNAS]){
    int filaInicio; //Fila comienzo de la comprobación
    if(fila-3>0){
        filaInicio=fila-3;
    }
    else{
        filaInicio=0;
    }

    int contador=0;
    for(;filaInicio<=fila;filaInicio++){
        if(tablero[filaInicio][columna]==jugador){
            contador++;
        }
        else{
            contador=0;
        }
    }
    return contador;
}

int conectaHorizontal(int columna, int fila, char jugador, char tablero[FILAS][COLUMNAS]){
    int columnaFinal; //Columna final de la comprobación
    if(columna+4<COLUMNAS){
        columnaFinal=columna+3;
    }
    else{
        columnaFinal=COLUMNAS-1;
    }

    int contador=0;
    for(;columna<=columnaFinal;columna++){
        if(tablero[fila][columna]==jugador){
            contador++;
        }
        else{
            contador=0;
        }
    }
    return contador;
}

int conectaDiagonalAscendente(int columna, int fila, char jugador, char tablero[FILAS][COLUMNAS]){
    int filaInicio;
    if(fila-3>0){
        filaInicio=fila-3;
    }
    else{
        filaInicio=0;
    }

    int columnaFinal;
    if(columna+4<COLUMNAS){
        columnaFinal=columna+3;
    }
    else{
        columnaFinal=COLUMNAS-1;
    }

    int contador=0;
    while(columna<=columnaFinal && filaInicio<=fila){
        if(tablero[fila][columna]==jugador){
            contador++;
        }
        else{
            contador=0;
        }
        fila--;
        columna++;
    }
    return contador;
}

int conectaDiagonalDescendente(int columna, int fila, char jugador, char tablero[FILAS][COLUMNAS]){
    int columnaFinal;
    if(columna+4<COLUMNAS){
        columnaFinal=columna+3;
    }
    else{
        columnaFinal=COLUMNAS-1;
    }

    int filaFinal;
    if(fila+4<FILAS){
        filaFinal=fila+3;
    }
    else{
        filaFinal=FILAS-1;
    }

    int contador=0;
    while(columna<=columnaFinal && fila<=filaFinal){
        if(tablero[fila][columna]==jugador){
            contador++;
        }
        else{
            contador=0;
        }
        fila++;
        columna++;
    }
    return contador;
}

//FUNCIÓN PARA LA DETERMINACIÓN DE GANADOR O EMPATE

int ganador(char jugador, char tablero[FILAS][COLUMNAS]){
    for(int fila=0;fila<FILAS;fila++){
        for(int columna=0;columna<COLUMNAS;columna++){
            if(conectaVertical(columna,fila,jugador,tablero)>=4){
                return CONECTA_VERTICAL;
            }
            if(conectaHorizontal(columna,fila,jugador,tablero)>=4){
                return CONECTA_HORIZONTAL;
            }
            if(conectaDiagonalAscendente(columna,fila,jugador,tablero)>=4){
                return CONECTA_DIAGONAL_ASCENDENTE;
            }
            if(conectaDiagonalDescendente(columna,fila,jugador,tablero)>=4){
                return CONECTA_DIAGONAL_DESCENDENTE;
            }
        }
    }
    return NO_CONECTA;
}

int empate(char tablero[FILAS][COLUMNAS]){
    for(int i=0;i<COLUMNAS;i++){
        int resultado=obtenerFilaDesocupada(i,tablero);
        if(resultado!=NO_VALID){
            return 0;
        }
    }
    return 1;
}

int obtenerFilaDesocupada(int columna, char tablero[FILAS][COLUMNAS]){
    for(int i=FILAS-1;i>=0;i--){
        if(tablero[i][columna]==VACIO){
            return i;
        }
    }
    return NO_VALID;
}

//FUNCIONES PARA EL TABLERO

void limpiarTablero(char tablero[FILAS][COLUMNAS]){
    for(int i=0;i<FILAS;i++){
        for(int j=0;j<COLUMNAS;j++){
            tablero[i][j]=VACIO;
        }
    }
}

int dibujarTablero(char tablero[FILAS][COLUMNAS]){
    cout<<endl;
    for(int i=0;i<COLUMNAS;i++){
        cout<<"|"<<i+1;
        if(i+1>=COLUMNAS){
            cout<<"|";
        }
    }
    cout<<endl;
    for(int i=0;i<FILAS;i++){
        for(int j=0;j<COLUMNAS;j++){
            cout<<"|"<<tablero[i][j];
            if(j+1>=COLUMNAS){
                cout<<"|";
            }
        }
        cout<<endl;
    }
    return 0;
}

int colocarFicha(char jugador, int columna, char tablero[FILAS][COLUMNAS]){
    if(columna<0 || columna>=COLUMNAS){
        return COLUMNA_INVALIDA;
    }
    int fila=obtenerFilaDesocupada(columna,tablero);
    if(fila==NO_VALID){
        return COLUMNA_LLENA;
    }
    tablero[fila][columna]=jugador;
    return CORRECT;
}

//FUNCIONES PARA LOS JUGADORES

char oponente(char jugador){
    if(jugador==JUGADOR_1){
        return JUGADOR_2;
    }
    else{
        return JUGADOR_1;
    }
}

int pedirColumna(){
    int columna;
    cout<<endl<<"Indique la columna en la que desea colocar su ficha: ";
    cin>>columna;
    columna--;
    return columna;
}

//FUNCIÓN PARA JUGAR

void jugar(){
    char tablero[FILAS][COLUMNAS];
    limpiarTablero(tablero);
    char jugadorActual=JUGADOR_1;

    while(1){
        int columna=0;
        //system("clear");
        cout<<endl<<"Turno del jugador "<<jugadorActual<<endl;
        dibujarTablero(tablero);
        columna=pedirColumna();
        int estado=colocarFicha(jugadorActual,columna,tablero);
        while(estado!=CORRECT){
            if(estado==COLUMNA_LLENA){
                cout<<"-Err. Columna llena"<<endl;
                columna=pedirColumna();
                estado=colocarFicha(jugadorActual,columna,tablero);
            }
            else if(estado==COLUMNA_INVALIDA){
                cout<<"-Err. Columna inválida"<<endl;
                columna=pedirColumna();
                estado=colocarFicha(jugadorActual,columna,tablero);
            }
        }
        if(estado==CORRECT){
            int win=ganador(jugadorActual,tablero);
            if(win!=NO_CONECTA){
                dibujarTablero(tablero);
                cout<<endl<<"Victoria del jugador "<<jugadorActual<<endl<<endl;
                break;
            }
            else if(empate(tablero)){
                dibujarTablero(tablero);
                cout<<endl<<"Empate"<<endl<<endl;
                break;
            }
        }
        jugadorActual=oponente(jugadorActual);
    }
}