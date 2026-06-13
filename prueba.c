/*1 – Utilizando el programa de la “Batalla Naval” del Trabajo Práctico Nº1, modifique el programa para
poder cumplir con los siguientes requisitos:

-Permita que el usuario pueda elegir el tamaño del tablero con el que quiere jugar. Para ello reserve
de manera dinámica la memoria para el mismo utilizado malloc.

-Envíe el tablero como un argumento por referencia a las funciones para que no sea una variable
global.

-Modifique la función que muestra el tablero para utilizar aritmética de punteros para recorrer el
arreglo.

2 – Agregue una opción de almacenar la secuencia de disparos que realiza el jugador (y poder mostrarla al
finalizar el juego). Como es una cantidad variable de disparos, utilice la funcion realloc para ir ampliando
la memoria usada a medida que el jugador va progresando en su juego.*/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

char tam_tabl[5][5];
int i, j;
int disparos = 0;
int hundido = 0;
char nombre[50];
char ranking_nombres[10][50];
int ranking_disparos[10];
int ranking = 0;

void agregar_ranking() {
    if (ranking<10) {
        strcpy(ranking_nombres[ranking], nombre);
        ranking_disparos[ranking] = disparos;
        ranking++;
    }
}

void mostrar_mejor() {
    if (ranking == 0) {
        printf("Sin record aun\n");
        return;
    }
    int mejor = 0;
    for (int k = 1; k < ranking; k++) {
        if (ranking_disparos[k] < ranking_disparos[mejor])
            mejor = k;
    }
    printf("[ Mejor: %s con %d disparos ]\n", ranking_nombres[mejor], ranking_disparos[mejor]);
}

void nombres() {
    printf("Ingrese su nombre: ");
    scanf(" %49[^\n]", nombre);
}

void inicializar_tab() {
    srand(time(NULL));
    int b_fila = rand() % 5;
    int b_col = rand() % 5;
    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
            tam_tabl[i][j] = '~';
    tam_tabl[b_fila][b_col] = 'B';
}

void mostrar_tab() {
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            if (tam_tabl[i][j] == 'B')
                printf("~");
            else
                printf("%c", tam_tabl[i][j]);
        }
        printf("\n");
    }
}

void realizar_disparo() {
    int fila, col;
    
    printf("Ingrese fila (0-4): ");
    scanf("%d", &fila);
    printf("Ingrese columna (0-4): ");
    scanf("%d", &col);

    if (tam_tabl[fila][col] == 'A') {
        printf("Ya disparaste aca\n");
    } else if (tam_tabl[fila][col] == 'B') {
        printf("Barco hundido!\n");
        tam_tabl[fila][col] = 'X';
        hundido = 1;
        disparos++;
    } else {
        tam_tabl[fila][col] = 'A';
        printf("Agua\n");
        disparos++;
    }
    printf("Disparos realizados: %d\n", disparos);
}

void menu() {
    int opcion;
    printf("\n");
    mostrar_mejor();

    printf("1. Inicializar el tablero\n");
    printf("2. Mostrar tablero\n");
    printf("3. Realizar disparo\n");

    scanf("%d", &opcion);

    switch (opcion) {
        case 1: 
            inicializar_tab(); 
        break;
        case 2: 
            mostrar_tab();    
        break;
        case 3:   
            realizar_disparo(); 
        break;
        default: 
        break;
    }
}

int main() {
    nombres();

    inicializar_tab();
    do {
        menu();
    } while (disparos != 10 && hundido != 1);

    if (hundido == 1) {
        printf("Destruiste el barco!\n");
        agregar_ranking();
    } else {
        printf("Perdiste\n");
    }

    mostrar_tab();
    return 0;
}