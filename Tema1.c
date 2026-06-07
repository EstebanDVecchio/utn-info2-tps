/* Cree un programa en C que simule un juego de "Batalla Naval" simplificado. El juego tendrá un
tablero de 5x5 donde el jugador intentará adivinar la ubicación de un barco enemigo de tamaño 1x1.
El jugador tendrá 10 disparos para intentar hundirlo. El juego terminará cuando se hunda el barco, o se
hayan realizado los 10 disparos. Al finalizar debe verse el tablero con la ubicación de los disparos y del
Barco (indicando si está hundido o no).

Requisitos:
 Utilice un arreglo de caracteres para representar el tablero. Marque la posicion del Barco con una
letra ‘B’, y los disparos realizados por el jugador, cuando no acierte al barco, con una ‘A’ (Agua).
 Genere la ubicación del barco de manera aleatoria en el tablero.
 Utilice funciones para separar las distintas partes de su programa.
 No debe poder disparar 2 veces en el mismo lugar.
 Lleve la cuenta de cuantos disparos se han hecho y muéstrela en la pantalla.
 Implemente un menú que permita al menos las siguientes operaciones:
 Inicializar el tablero e iniciar el juego.
 Mostrar el tablero.
 Realizar un disparo.

2 – Agregue la opción de almacenar un ranking de jugadores con la cantidad de disparos que realizó para
ganar. Le permitirá al jugador ingresar un nombre para poner en el ranking. Deberá mostrar el mejor
puntaje en algun lugar de la pantalla principal (con el nombre del mejor jugador) */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void rellenarTablero(char tablero[5][5]){
    int i, j;
    for(i = 0; i < 5; i++){
        for(j = 0; j < 5; j++){
            tablero[i][j] = '~';
        }
    }
}

void ubicacionBarco(char tablero[5][5], char barco){
    int x, y;
    x = rand()%5;
    y = rand()%5;
    tablero[x][y] = barco;
}
void mostrarTablero(char tablero [5][5]){
    int i, j;
    for(i = 0; i < 5; i++ ){
        for(j=0; j < 5; j++){
            if(tablero[i][j] == 'B'){
                printf("~");
            }else{
                printf("%c", tablero[i][j]);
            }
        }
        printf("\n");
    }
}
void disparo(int disparo){

}

int main(){

    srand(time(NULL));
    
    char tablero[5][5];
    char barco = 'B';
    int disparo = 0;
    rellenarTablero(tablero);
    ubicacionBarco(tablero, barco);
    mostrarTablero(tablero);
    return 0;
}