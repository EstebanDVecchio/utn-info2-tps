#include <stdio.h>    /* Biblioteca para entrada/salida (printf, scanf) */
#include <time.h>     /* Biblioteca para la funcion time() */
#include <stdlib.h>   /* Biblioteca para malloc, free, srand, rand */
#include <string.h>   /* Biblioteca para strcpy */

struct record {                   /* Estructura para guardar un registro del ranking */
    char nombre[50];              /* Nombre del jugador (hasta 49 caracteres) */
    int disparos;                 /* Cantidad de disparos que hizo */
};

struct disparo {                  /* Estructura para guardar un disparo en el historial */
    int fila;                     /* Fila donde se disparo */
    int columna;                  /* Columna donde se disparo */
};

typedef struct {                  /* Definicion del tipo Juego (estructura que agrupa todo) */
    char **celdas;                /* Matriz dinamica que representa el tablero */
    int tam;                      /* Tamanio del tablero (tam x tam) */
    int disparos;                 /* Total de disparos realizados */
    int hundido;                  /* Indica si el barco fue hundido (1 = si, 0 = no) */
    char nombre[50];              /* Nombre del jugador actual */
    struct record ranking[10];    /* Arreglo con los mejores 10 puntajes */
    int ranking_count;            /* Cuantos registros hay en el ranking */
    struct disparo *historial;    /* Puntero a arreglo dinamico del historial de disparos */
    int num_disparos;             /* Contador de disparos para el historial */
} Juego;

void agregar_ranking(Juego *juego) {              /* Agrega el puntaje actual al ranking */
    if (juego->ranking_count < 10) {              /* Si hay espacio en el ranking (maximo 10)... */
        strcpy(juego->ranking[juego->ranking_count].nombre, juego->nombre); /* Copia el nombre */
        juego->ranking[juego->ranking_count].disparos = juego->num_disparos; /* Guarda los disparos */
        juego->ranking_count++;                   /* Incrementa el contador del ranking */
    }
}

void mostrar_mejor(Juego *juego) {                /* Muestra el mejor jugador del ranking */
    if (juego->ranking_count == 0) {              /* Si no hay registros... */
        printf("Sin record aun\n");               /* ...muestra mensaje */
        return;
    }
    int mejor = 0;                                /* Asume que el primero es el mejor */
    for (int k = 1; k < juego->ranking_count; k++) {          /* Recorre los demas */
        if (juego->ranking[k].disparos < juego->ranking[mejor].disparos) /* Si tiene menos disparos... */
            mejor = k;                            /* ...actualiza el mejor */
    }
    printf("[ Mejor: %s con %d disparos ]\n",     /* Muestra el nombre y disparos del mejor */
           juego->ranking[mejor].nombre, juego->ranking[mejor].disparos);
}

void inicializar_tab(Juego *juego) {              /* Inicializa el tablero con un barco aleatorio */
    srand(time(NULL));                            /* Semilla para numeros aleatorios basada en la hora actual */
    int b_fila = rand() % juego->tam;             /* Fila aleatoria del barco */
    int b_col = rand() % juego->tam;              /* Columna aleatoria del barco */
    for (int i = 0; i < juego->tam; i++)          /* Recorre todas las filas... */
        for (int j = 0; j < juego->tam; j++)      /* ...y todas las columnas */
            juego->celdas[i][j] = '~';            /* Asigna agua ('~') a cada celda */
    juego->celdas[b_fila][b_col] = 'B';           /* Coloca el barco ('B') en la posicion aleatoria */
}

void mostrar_tab(Juego *juego) {                  /* Muestra el tablero ocultando la posicion del barco */
    for (int i = 0; i < juego->tam; i++) {        /* Recorre filas */
        for (int j = 0; j < juego->tam; j++) {    /* Recorre columnas */
            if (juego->celdas[i][j] == 'B')       /* Si hay un barco ('B')... */
                printf("~");                       /* ...lo muestra como agua para ocultarlo */
            else                                   /* Sino... */
                printf("%c", juego->celdas[i][j]); /* ...muestra el caracter real */
        }
        printf("\n");                              /* Salto de linea al terminar la fila */
    }
}

void realizar_disparo(Juego *juego) {             /* Permite al jugador realizar un disparo */
    int fila, col;                                /* Variables para fila y columna ingresadas */
    printf("Ingrese fila (0-%d): ", juego->tam - 1);    /* Pide la fila */
    scanf("%d", &fila);                           /* Lee la fila */
    printf("Ingrese columna (0-%d): ", juego->tam - 1); /* Pide la columna */
    scanf("%d", &col);                            /* Lee la columna */

    juego->num_disparos++;                        /* Incrementa el contador de disparos del historial */
    juego->historial = realloc(juego->historial, juego->num_disparos * sizeof(struct disparo)); /* Agranda el historial */
    juego->historial[juego->num_disparos - 1].fila = fila;   /* Guarda la fila del disparo */
    juego->historial[juego->num_disparos - 1].columna = col; /* Guarda la columna del disparo */

    if (juego->celdas[fila][col] == 'A') {        /* Si ya se disparo ahi (agua)... */
        printf("Ya disparaste aca\n");            /* ...avisa */
    } else if (juego->celdas[fila][col] == 'B') { /* Si habia un barco... */
        printf("Barco hundido!\n");               /* ...avisa que lo hundio */
        juego->celdas[fila][col] = 'X';           /* Marca la celda como hundido ('X') */
        juego->hundido = 1;                       /* Cambia el estado a hundido */
        juego->disparos++;                        /* Incrementa el contador de disparos */
    } else {                                       /* Sino (agua virgen)... */
        juego->celdas[fila][col] = 'A';           /* ...marca la celda como agua ('A') */
        printf("Agua\n");                         /* ...avisa que cayo al agua */
        juego->disparos++;                        /* Incrementa el contador de disparos */
    }
    printf("Disparos realizados: %d\n", juego->disparos); /* Muestra cuantos disparos lleva */
}

void menu(Juego *juego) {                         /* Muestra el menu de opciones */
    int opcion;                                   /* Variable para la opcion elegida */
    printf("\n");                                 /* Salto de linea */
    mostrar_mejor(juego);                         /* Muestra el mejor jugador del ranking */

    printf("1. Inicializar el tablero\n");        /* Opcion 1: reiniciar el tablero */
    printf("2. Mostrar tablero\n");               /* Opcion 2: ver el tablero */
    printf("3. Realizar disparo\n");              /* Opcion 3: disparar */

    scanf("%d", &opcion);                         /* Lee la opcion */

    switch (opcion) {                             /* Segun la opcion elegida... */
        case 1:
            inicializar_tab(juego);               /* ...inicializa el tablero */
        break;
        case 2:
            mostrar_tab(juego);                   /* ...muestra el tablero */
        break;
        case 3:
            realizar_disparo(juego);              /* ...realiza un disparo */
        break;
        default:
        break;                                    /* Opcion invalida: no hace nada */
    }
}

int main() {                                      /* Funcion principal */
    Juego juego;                                  /* Declara una variable de tipo Juego */
    juego.historial = NULL;                       /* Inicializa el historial como NULL (sin disparos) */
    juego.num_disparos = 0;                       /* Inicializa contador del historial en 0 */
    juego.disparos = 0;                           /* Inicializa los disparos en 0 */
    juego.hundido = 0;                            /* Inicializa hundido como falso */
    juego.ranking_count = 0;                      /* Inicializa el contador del ranking en 0 */

    printf("Ingrese su nombre: ");                /* Pide el nombre del jugador */
    scanf(" %49[^\n]", juego.nombre);             /* Lee el nombre (maximo 49 caracteres, hasta salto de linea) */

    printf("Ingrese el tamanio del tablero: ");   /* Pide el tamanio del tablero */
    scanf("%d", &juego.tam);                      /* Lee el tamanio */

    juego.celdas = malloc(juego.tam * sizeof(char*));           /* Reserva memoria para las filas */
    for (int i = 0; i < juego.tam; i++)                         /* Para cada fila... */
        juego.celdas[i] = malloc(juego.tam * sizeof(char));     /* ...reserva memoria para las columnas */

    inicializar_tab(&juego);                      /* Inicializa el tablero con un barco aleatorio */
    do {                                          /* Bucle que se repite al menos una vez... */
        menu(&juego);                             /* ...muestra el menu y ejecuta la opcion */
    } while (juego.disparos != 10 && juego.hundido != 1); /* ...mientras queden disparos y no se haya hundido */

    if (juego.hundido == 1) {                     /* Si el barco fue hundido... */
        printf("Destruiste el barco!\n");         /* ...el jugador gano */
        agregar_ranking(&juego);                  /* Agrega su puntaje al ranking */
    } else {                                       /* Sino... */
        printf("Perdiste\n");                     /* ...el jugador perdio */
    }

    mostrar_tab(&juego);                          /* Muestra el tablero final con la posicion del barco */

    for (int i = 0; i < juego.tam; i++)           /* Recorre cada fila... */
        free(juego.celdas[i]);                    /* ...libera la memoria de esa fila */
    free(juego.celdas);                           /* Libera el arreglo de punteros a filas */
    free(juego.historial);                        /* Libera el historial de disparos */
    return 0;                                     /* Termina el programa */
}
