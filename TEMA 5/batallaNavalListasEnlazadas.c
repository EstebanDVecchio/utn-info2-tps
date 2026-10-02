#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_RANKING 5

typedef struct record{
    char nombre[50];
    int disparos;
    char fechaRecord[50];
}record;

typedef struct disparo {
    int fila;
    int columna;
    char resultado[20];
}disparo;

typedef struct juego{
    char **tablero;
    int tamano;
    int barcoFila;
    int barcoCol;
}juego;

typedef struct nodo_ranking {
    record jugador;
    struct nodo_ranking *siguiente;
}nodo_ranking;

// ========== LISTA ENLAZADA DEL RANKING ==========

//Inserta un nodo nuevo manteniendo el orden ascendente por disparos
//(el que tiene menos disparos queda al principio de la lista)
nodo_ranking *insertarOrdenado(nodo_ranking *primero, record jugador){
    nodo_ranking *nuevo = malloc(sizeof(nodo_ranking));
    if(nuevo == NULL){
        printf("Error al asignar memoria\n");
        return primero;
    }
    nuevo->jugador = jugador;
    nuevo->siguiente = NULL;

    //si la lista esta vacia o el nuevo record es el mejor, va al principio
    if(primero == NULL || jugador.disparos < primero->jugador.disparos){
        nuevo->siguiente = primero;
        return nuevo;
    }

    //sino, busco el lugar que le corresponde
    nodo_ranking *actual = primero;
    while(actual->siguiente != NULL && actual->siguiente->jugador.disparos <= jugador.disparos){
        actual = actual->siguiente;
    }

    nuevo->siguiente = actual->siguiente;
    actual->siguiente = nuevo;
    return primero;
}

//Deja en la lista solamente los MAX_RANKING mejores (los primeros de la lista)
//y libera la memoria de los nodos que quedan afuera
nodo_ranking *recortarTop5(nodo_ranking *primero){
    if(primero == NULL){
        return NULL;
    }

    int cantidad = 0;
    nodo_ranking *actual = primero;
    while(actual != NULL){
        cantidad++;
        actual = actual->siguiente;
    }

    if(cantidad <= MAX_RANKING){
        return primero;
    }

    //me posiciono en el ultimo nodo que queda en el ranking
    actual = primero;
    for(int i = 1; i < MAX_RANKING; i++){
        actual = actual->siguiente;
    }

    //y libero todos los que siguen
    nodo_ranking *aux = actual->siguiente;
    actual->siguiente = NULL;

    while(aux != NULL){
        nodo_ranking *temp = aux;
        aux = aux->siguiente;
        free(temp);
    }

    return primero;
}

// ========== ARCHIVO ranking.txt ==========

//Lee ranking.txt y reconstruye la lista enlazada conservando el orden de los records
nodo_ranking *cargarRankingDelArchivo(nodo_ranking *primero){
    FILE *archivo = fopen("ranking.txt", "r");
    if(archivo == NULL){
        //si el archivo no existe, el ranking arranca vacio
        return primero;
    }

    char linea[150];
    char nombreTemp[50];
    int disparosTemp;
    char fechaTemp[50];

    while(fgets(linea, sizeof(linea), archivo) != NULL){

        // %[^,] lee hasta chocar con la coma: nombre, disparos, fecha
        if(sscanf(linea, "%49[^,],%d,%49[^\n]", nombreTemp, &disparosTemp, fechaTemp) == 3){

            fechaTemp[strcspn(fechaTemp, "\r")] = '\0'; //borra un eventual \r

            record r;
            strcpy(r.nombre, nombreTemp);
            r.disparos = disparosTemp;
            strcpy(r.fechaRecord, fechaTemp);

            primero = insertarOrdenado(primero, r);
        }
    }
    fclose(archivo);

    //por si el archivo trae mas de MAX_RANKING records
    primero = recortarTop5(primero);

    return primero;
}

//Recorre toda la lista enlazada y guarda cada nodo en ranking.txt
nodo_ranking *guardarRankingArchivo(nodo_ranking *primero){
    FILE *archivo = fopen("ranking.txt", "w");
    if(archivo == NULL){
        printf("Error al guardar el ranking\n");
        return primero;
    }

    nodo_ranking *actual = primero;
    while(actual != NULL){
        fprintf(archivo, "%s,%d,%s\n",
                actual->jugador.nombre,
                actual->jugador.disparos,
                actual->jugador.fechaRecord);
        actual = actual->siguiente;
    }

    fclose(archivo);
    return primero;
}

void guardarRanking(nodo_ranking *primero){
    guardarRankingArchivo(primero);
}

// ========== OPERACIONES SOBRE EL RANKING ==========

//Carga el record del jugador que acaba de ganar y recorta la lista a los 5 mejores
nodo_ranking *ranking(nodo_ranking *primero, record *jugadorActual){
    record nuevoRecord;

    strcpy(nuevoRecord.nombre, jugadorActual->nombre);
    nuevoRecord.disparos = jugadorActual->disparos;

    time_t tiempoActual;
    time(&tiempoActual);
    struct tm *infoTiempo = localtime(&tiempoActual);
    strftime(nuevoRecord.fechaRecord, sizeof(nuevoRecord.fechaRecord), "%d/%m/%Y %H:%M", infoTiempo);

    primero = insertarOrdenado(primero, nuevoRecord);
    primero = recortarTop5(primero);

    //veo si el record quedo dentro del top 5
    int encontrado = 0;
    nodo_ranking *actual = primero;
    while(actual != NULL){
        if(strcmp(actual->jugador.nombre, nuevoRecord.nombre) == 0 &&
           actual->jugador.disparos == nuevoRecord.disparos){
            encontrado = 1;
            break;
        }
        actual = actual->siguiente;
    }

    if(encontrado){
        printf("\nHas entrado al Top 5!\n");
    }else{
        printf("\nNo alcanzaste el Top 5\n");
    }

    return primero;
}

void mostrarRanking(nodo_ranking *primero){
    printf("\n=====Ranking Top %d=====\n", MAX_RANKING);

    if(primero == NULL){
        printf("\nSin campeon\n");
        return;
    }

    time_t ahora;
    time(&ahora);

    nodo_ranking *actual = primero;
    int posicion = 1;

    while(actual != NULL){
        printf("\n%d. %s - %d disparos\n",
               posicion, actual->jugador.nombre, actual->jugador.disparos);

        struct tm tm_record = {0};
        int leidos = sscanf(actual->jugador.fechaRecord, "%d/%d/%d %d:%d",
                            &tm_record.tm_mday, &tm_record.tm_mon, &tm_record.tm_year,
                            &tm_record.tm_hour, &tm_record.tm_min);

        if(leidos == 5){
            tm_record.tm_mon -= 1;
            tm_record.tm_year -= 1900;
            tm_record.tm_isdst = -1;
            time_t tiempoRecord = mktime(&tm_record);
            double segundos = difftime(ahora, tiempoRecord);
            int dias = (int)(segundos / 86400);
            printf("   Record hace %d dias\n", dias);
        }

        actual = actual->siguiente;
        posicion++;
    }
}

//Libera la memoria de todos los nodos de la lista (evita memory leaks)
void liberarLista(nodo_ranking *primero){
    nodo_ranking *actual = primero;

    while(actual != NULL){
        nodo_ranking *temp = actual;
        actual = actual->siguiente;
        free(temp);
    }
}

// ========== HISTORIAL DE DISPAROS ==========

void guardarHistorialDisparo(record *jugadorActual, disparo *disparo){
    char nombreArchivo[100];
    snprintf(nombreArchivo, sizeof(nombreArchivo), "disparos_%s.txt", jugadorActual->nombre);

    FILE *archivo = fopen(nombreArchivo, "a");
    if(archivo == NULL){
        printf("Error al abrir el archivo de historial\n");
        return;
    }

    fprintf(archivo, "%d,%d,%s\n", disparo->fila, disparo->columna, disparo->resultado);
    fclose(archivo);
}

void mostrarHistorialDisparos(record *jugadorActual){
    char nombreArchivo[100];
    snprintf(nombreArchivo, sizeof(nombreArchivo), "disparos_%s.txt", jugadorActual->nombre);

    FILE *archivo = fopen(nombreArchivo, "r");
    if(archivo == NULL){
        printf("No hay historial de disparos para %s\n", jugadorActual->nombre);
        return;
    }

    printf("\nHistorial de disparos de %s:\n", jugadorActual->nombre);

    int fila;
    int columna;
    char resultado[20];

    while(fscanf(archivo, "%d,%d,%19[^\n]\n", &fila, &columna, resultado) == 3){
        printf("Fila %d, Columna %d -> %s\n", fila, columna, resultado);
    }

    fclose(archivo);
}

// ========== GUARDAR / CARGAR PARTIDA ==========

void guardarJuego(juego *juego, record *jugadorActual){
    char nombreArchivo[100];
    snprintf(nombreArchivo, sizeof(nombreArchivo), "partida_%s.txt", jugadorActual->nombre);

    FILE *archivo = fopen(nombreArchivo, "w");
    if(archivo == NULL){
        printf("Error al guardar la partida\n");
        return;
    }

    fprintf(archivo, "%s\n", jugadorActual->nombre);
    fprintf(archivo, "%d\n", juego->tamano);
    fprintf(archivo, "%d\n", jugadorActual->disparos);
    fprintf(archivo, "%d,%d\n", juego->barcoFila, juego->barcoCol);

    for (int i = 0; i < juego->tamano; i++){
        for (int j = 0; j < juego->tamano; j++){
            fputc(juego->tablero[i][j], archivo);
        }
        fputc('\n', archivo);
    }

    fclose(archivo);
    printf("Partida guardada\n");
}

void cargarJuego(juego *juego, record *jugadorActual){
    char nombreArchivo[100];
    snprintf(nombreArchivo, sizeof(nombreArchivo), "partida_%s.txt", jugadorActual->nombre);

    FILE *archivo = fopen(nombreArchivo, "r");
    if(archivo == NULL){
        printf("No hay partida guardada para %s\n", jugadorActual->nombre);
        return;
    }

    for(int i = 0; i < juego->tamano; i++){
        free(juego->tablero[i]);
    }
    free(juego->tablero);

    char linea[100];
    fgets(linea, sizeof(linea), archivo);
    linea[strcspn(linea, "\n")] = '\0';
    strcpy(jugadorActual->nombre, linea);

    fscanf(archivo, "%d\n", &juego->tamano);
    fscanf(archivo, "%d\n", &jugadorActual->disparos);
    fscanf(archivo, "%d,%d\n", &juego->barcoFila, &juego->barcoCol);

    juego->tablero = malloc(juego->tamano * sizeof(char*));
    for(int i = 0; i < juego->tamano; i++){
        juego->tablero[i] = malloc(juego->tamano * sizeof(char));
    }

    for (int i = 0; i < juego->tamano; i++){
        for (int j = 0; j < juego->tamano; j++){
            juego->tablero[i][j] = fgetc(archivo);
        }
        fgetc(archivo);
    }

    fclose(archivo);
    printf("Partida cargada para %s\n", jugadorActual->nombre);
}

//INICIALIZAR TABLERO
void inicializarTablero(juego *juego, record *jugadorActual){
    srand(time(NULL));
    juego->barcoFila = rand() % juego->tamano;
    juego->barcoCol = rand() % juego->tamano;
    jugadorActual->disparos = 0;

    for (int i = 0; i < juego->tamano; i++){
        for(int j = 0; j < juego->tamano; j++){
            juego->tablero[i][j] = '~';
        }
    }
    juego->tablero[juego->barcoFila][juego->barcoCol] = 'B';
}
//MOSTRAR TABLERO
void mostrarTablero(juego *juego){
    for(int i = 0; i < juego->tamano; i++){
        for(int j = 0; j < juego->tamano; j++){
            if(juego->tablero[i][j] == 'B'){
                printf(" ~");
            }else{
                printf(" %c", juego->tablero[i][j]);
            }
        }
        printf("\n");
    }
}
//REALIZAR DISPARO
void realizarDisparo(juego *juego, disparo *disparo, record *jugadorActual){

    printf("\nEn que fila: ");
    scanf("%d", &disparo->fila);
    printf("En que columna: ");
    scanf("%d", &disparo->columna);

    if(disparo->fila < 0 || disparo->fila >= juego->tamano ||
       disparo->columna < 0 || disparo->columna >= juego->tamano){
        printf("Coordenadas invalidas\n");
        return;
    }

    if(juego->tablero[disparo->fila][disparo->columna] == 'B'){
        strcpy(disparo->resultado, "hundido");
        printf("Destruiste el barco\n");

        juego->tablero[disparo->fila][disparo->columna] = 'X';
        jugadorActual->disparos++;
    }else if(juego->tablero[disparo->fila][disparo->columna] == 'A'){
        strcpy(disparo->resultado, "repetido");
        printf("\nYa disparaste aca\n");

    }else{
        strcpy(disparo->resultado, "agua");
        printf("Agua\n");

        juego->tablero[disparo->fila][disparo->columna] = 'A';
        jugadorActual->disparos++;
    }

    if(strcmp(disparo->resultado, "repetido") != 0){
        guardarHistorialDisparo(jugadorActual, disparo);
    }
    printf("Disparo en (%d,%d) -> %s\n", disparo->fila, disparo->columna, disparo->resultado);
    printf("Disparos: %d\n", jugadorActual->disparos);

}
//MENU (devuelve la lista para no perder los cambios al salir)
nodo_ranking *menu(juego *juego, disparo *disparo, nodo_ranking *primero, record *jugadorActual){
    int opcion;

    do{
        printf("\n=====MENU=====\n");
        mostrarRanking(primero);
        printf("1. Inicializar tablero\n");
        printf("2. Mostrar tablero\n");
        printf("3. Realizar disparo\n");
        printf("4. Mostrar ranking\n");
        printf("5. Mostrar historial de disparos\n");
        printf("6. Guardar partida\n");
        printf("7. Cargar partida\n");
        printf("8. Cerrar programa\n");
        printf("Seleccione una opcion: ");

        scanf("%d",&opcion);

        switch (opcion){
        case 1:
            inicializarTablero(juego, jugadorActual);
            break;
        case 2:
            printf("\n");
            mostrarTablero(juego);
            break;
        case 3:
            while(jugadorActual->disparos < 10){
                realizarDisparo(juego, disparo, jugadorActual);

                if(strcmp(disparo->resultado, "hundido") == 0){
                    printf("\n========= GANASTE! =========\n");
                    //el ganador entra a la lista (se recorta a los 5 mejores)
                    primero = ranking(primero, jugadorActual);
                    //al finalizar la partida se guarda el ranking en el archivo
                    guardarRanking(primero);
                    break;
                }
            }
            if(jugadorActual->disparos >= 10){
                printf("\n========= PERDISTE =========\n");
                printf("Te quedaste sin disparos\n");
                mostrarTablero(juego);
                //al finalizar la partida se guarda el ranking en el archivo
                guardarRanking(primero);
            }
            printf("Partida finalizada. Elija otra opcion o salga.\n");
            break;
        case 4:
            mostrarRanking(primero);
            break;
        case 5:
            mostrarHistorialDisparos(jugadorActual);
            break;
        case 6:
            guardarJuego(juego, jugadorActual);
            break;
        case 7:
            cargarJuego(juego, jugadorActual);
            mostrarTablero(juego);
            break;
        case 8:
            printf("Cerrando...\n");
            break;
        default:
            break;
        }

    } while (opcion != 8);

    return primero;
}
//PRINCIPAL
int main(){
    record jugadorActual;
    nodo_ranking *listaRanking = NULL;
    disparo disparo;
    juego juego;

    //al iniciar el juego se reconstruye la lista desde ranking.txt
    listaRanking = cargarRankingDelArchivo(listaRanking);

    printf("Ingrese su nombre: ");
    fgets(jugadorActual.nombre, sizeof(jugadorActual.nombre), stdin);
    jugadorActual.nombre[strcspn(jugadorActual.nombre, "\n")] = '\0'; //limpia el buffer del enter

    printf("Ingrese el tamano del tablero: ");
    scanf("%d", &juego.tamano);

    juego.tablero = malloc(juego.tamano * sizeof(char*));
    for(int i = 0; i < juego.tamano; i++){
        juego.tablero[i] = malloc(juego.tamano * sizeof(char));
    }

    inicializarTablero(&juego, &jugadorActual);
    listaRanking = menu(&juego, &disparo, listaRanking, &jugadorActual);

    //al salir: se guarda el ranking en el archivo y se libera la memoria
    guardarRanking(listaRanking);
    liberarLista(listaRanking);
    listaRanking = NULL;

    //libera la memoria de las columnas
    for(int i = 0; i < juego.tamano; i++){
        free(juego.tablero[i]);
    }
    //libera la memoria de las filas
    free(juego.tablero);

    return 0;
}
