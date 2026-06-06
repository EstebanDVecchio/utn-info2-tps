#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

int tam;
int i, j;
int disparos = 0;
int hundido = 0;
char nombre[50];
char ranking_nombres[10][50];
int ranking_disparos[10];
int ranking = 0;

char **tablero;

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

void inicializar_tab(char **tablero, int tam) {
    srand(time(NULL));
    int b_fila = rand() % tam;
    int b_col = rand() % tam;
    for (i = 0; i < tam; i++)
        for (j = 0; j < tam; j++)
            tablero[i][j] = '~';
    tablero[b_fila][b_col] = 'B';
}

void mostrar_tab(char **tablero, int tam) {
    for (i = 0; i < tam; i++) {
        for (j = 0; j < tam; j++) {
            if (tablero[i][j] == 'B')
                printf("~");
            else
                printf("%c", tablero[i][j]);
        }
        printf("\n");
    }
}

void realizar_disparo() {
    int fila, col;
    
    printf("Ingrese fila (0-%d): ", tam - 1);
    scanf("%d", &fila);
    printf("Ingrese columna (0-%d): ", tam - 1);
    scanf("%d", &col);

    if (tablero[fila][col] == 'A') {
        printf("Ya disparaste aca\n");
    } else if (tablero[fila][col] == 'B') {
        printf("Barco hundido!\n");
        tablero[fila][col] = 'X';
        hundido = 1;
        disparos++;
    } else {
        tablero[fila][col] = 'A';
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
            inicializar_tab(tablero, tam); 
        break;
        case 2: 
            mostrar_tab(tablero, tam);    
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

    printf("Ingrese el tamanio del tablero: ");
    scanf("%d", &tam);

    tablero = malloc(tam * sizeof(char*));
    for (int i = 0; i < tam; i++)
        tablero[i] = malloc(tam * sizeof(char));

    inicializar_tab(tablero, tam);
    do {
        menu();
    } while (disparos != 10 && hundido != 1);

    if (hundido == 1) {
        printf("Destruiste el barco!\n");
        agregar_ranking();
    } else {
        printf("Perdiste\n");
    }

    mostrar_tab(tablero, tam);

    for (int i = 0; i < tam; i++)
        free(tablero[i]);
    free(tablero);
    return 0;
}