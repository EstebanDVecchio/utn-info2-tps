#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

struct record {
    char nombre[50];
    int disparos;
};

struct disparo {
    int fila;
    int columna;
};

typedef struct {
    char **celdas;
    int tam;
    int disparos;
    int hundido;
    char nombre[50];
    struct record ranking[10];
    int ranking_count;
    struct disparo *historial;
    int num_disparos;
} Juego;

void agregar_ranking(Juego *juego) {
    if (juego->ranking_count < 10) {
        strcpy(juego->ranking[juego->ranking_count].nombre, juego->nombre);
        juego->ranking[juego->ranking_count].disparos = juego->num_disparos;
        juego->ranking_count++;
    }
}

void mostrar_mejor(Juego *juego) {
    if (juego->ranking_count == 0) {
        printf("Sin record aun\n");
        return;
    }
    
    int mejor = 0;
    for (int k = 1; k < juego->ranking_count; k++) {
        if (juego->ranking[k].disparos < juego->ranking[mejor].disparos)
            mejor = k;
    }
    printf("[ Mejor: %s con %d disparos ]\n",
           juego->ranking[mejor].nombre, juego->ranking[mejor].disparos);
}

void inicializar_tab(Juego *juego) {
    srand(time(NULL));
    int b_fila = rand() % juego->tam;
    int b_col = rand() % juego->tam;
    for (int i = 0; i < juego->tam; i++)
        for (int j = 0; j < juego->tam; j++)
            juego->celdas[i][j] = '~';
    juego->celdas[b_fila][b_col] = 'B';
}

void mostrar_tab(Juego *juego) {
    for (int i = 0; i < juego->tam; i++) {
        for (int j = 0; j < juego->tam; j++) {
            if (juego->celdas[i][j] == 'B')
                printf("~");
            else
                printf("%c", juego->celdas[i][j]);
        }
        printf("\n");
    }
}

void realizar_disparo(Juego *juego) {
    int fila, col;

    printf("Ingrese fila (0-%d): ", juego->tam - 1);
    scanf("%d", &fila);
    printf("Ingrese columna (0-%d): ", juego->tam - 1);
    scanf("%d", &col);

    juego->num_disparos++;
    juego->historial = realloc(juego->historial, juego->num_disparos * sizeof(struct disparo));
    juego->historial[juego->num_disparos - 1].fila = fila;
    juego->historial[juego->num_disparos - 1].columna = col;

    if (juego->celdas[fila][col] == 'A') {
        printf("Ya disparaste aca\n");
    } else if (juego->celdas[fila][col] == 'B') {
        printf("Barco hundido!\n");
        juego->celdas[fila][col] = 'X';
        juego->hundido = 1;
        juego->disparos++;
    } else {
        juego->celdas[fila][col] = 'A';
        printf("Agua\n");
        juego->disparos++;
    }
    printf("Disparos realizados: %d\n", juego->disparos);
}

void menu(Juego *juego) {
    int opcion;
    printf("\n");
    mostrar_mejor(juego);

    printf("1. Inicializar el tablero\n");
    printf("2. Mostrar tablero\n");
    printf("3. Realizar disparo\n");

    scanf("%d", &opcion);

    switch (opcion) {
        case 1:
            inicializar_tab(juego);
        break;
        case 2:
            mostrar_tab(juego);
        break;
        case 3:
            realizar_disparo(juego);
        break;
        default:
        break;
    }
}

int main() {
    Juego juego;
    juego.historial = NULL;
    juego.num_disparos = 0;
    juego.disparos = 0;
    juego.hundido = 0;
    juego.ranking_count = 0;

    printf("Ingrese su nombre: ");
    scanf(" %49[^\n]", juego.nombre);

    printf("Ingrese el tamanio del tablero: ");
    scanf("%d", &juego.tam);

    juego.celdas = malloc(juego.tam * sizeof(char*));
    for (int i = 0; i < juego.tam; i++)
        juego.celdas[i] = malloc(juego.tam * sizeof(char));

    inicializar_tab(&juego);
    do {
        menu(&juego);
    } while (juego.disparos != 10 && juego.hundido != 1);

    if (juego.hundido == 1) {
        printf("Destruiste el barco!\n");
        agregar_ranking(&juego);
    } else {
        printf("Perdiste\n");
    }

    mostrar_tab(&juego);

    for (int i = 0; i < juego.tam; i++)
        free(juego.celdas[i]);
    free(juego.celdas);
    free(juego.historial);
    return 0;
}
