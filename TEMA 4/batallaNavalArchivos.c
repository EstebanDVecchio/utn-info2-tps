#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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


//PARTIDAS GUARDADAS
void guardarPartida(record *jugadorActual){

    FILE *archivo = fopen("ranking.txt","a");

    if(archivo == NULL){
        printf("Error al abrir o crear el archivo\n");
        return;
    }
    time_t tiempoActual;
    time(&tiempoActual);
    struct tm *infoTiempo = localtime(&tiempoActual);

    char fechaHora[50];
    strftime(fechaHora, sizeof(fechaHora), "%d/%m/%Y %H:%M", infoTiempo);

    fprintf(archivo, "%s,%d,%s\n", jugadorActual->nombre, jugadorActual->disparos, fechaHora);
    fclose(archivo);
}
void cargarMejorJugador(record *mejorJugador){
    
    FILE *archivo = fopen("ranking.txt", "r");
    if(archivo == NULL){
        return;
    }
    char linea[150];
    char nombreTemp[50];
    int disparosTemp;
    char fechaTemp[50];

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        
        // %[^,] Lee el texto hasta chocar con una coma
        if (sscanf(linea, "%[^,],%d,%[^\n]", nombreTemp, &disparosTemp, fechaTemp) == 3) {
            
            if (disparosTemp < mejorJugador->disparos) {
                mejorJugador->disparos = disparosTemp;
                strcpy(mejorJugador->nombre, nombreTemp);
                strcpy(mejorJugador->fechaRecord, fechaTemp);
            }
        }
    }
    fclose(archivo);
}
//RANKING
void ranking(record *mejorJugador, record *jugadorActual ){

    if (jugadorActual->disparos < mejorJugador->disparos) {
    
        mejorJugador->disparos = jugadorActual->disparos;
        strcpy(mejorJugador->nombre, jugadorActual->nombre);

        time_t tiempoActual;
        time(&tiempoActual);
        struct tm *infoTiempo = localtime(&tiempoActual);
        strftime(mejorJugador->fechaRecord, sizeof(mejorJugador->fechaRecord), "%d/%m/%Y %H:%M", infoTiempo);
    
        printf("\nHas superado el record\n");
    }
}
void mostrarRanking(record *mejorJugador){
    printf("\n=====Ranking=====\n");
    
    if(mejorJugador->disparos == 9999){
        printf("\nSin campeon\n");
    }else{
        printf("\nMejor jugador: %s\n", mejorJugador->nombre);
        printf("Disparos: %d\n", mejorJugador->disparos);
        time_t ahora;
        time(&ahora);
        struct tm tm_record = {0};
        sscanf(mejorJugador->fechaRecord, "%d/%d/%d %d:%d",
               &tm_record.tm_mday, &tm_record.tm_mon, &tm_record.tm_year,
               &tm_record.tm_hour, &tm_record.tm_min);
        tm_record.tm_mon -= 1;
        tm_record.tm_year -= 1900;
        tm_record.tm_isdst = -1;
        time_t tiempoRecord = mktime(&tm_record);
        double segundos = difftime(ahora, tiempoRecord);
        int dias = (int)(segundos / 86400);
        printf("Record establecido hace %d dias\n", dias);
    }
}

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
//MENU
void menu(juego *juego, disparo *disparo, record *mejorJugador, record *jugadorActual){
    int opcion;
    
    do{ 
        printf("\n=====MENU=====\n");
        mostrarRanking(mejorJugador);
        printf("1. Inicializar tablero\n");
        printf("2. Mostrar tablero\n");
        printf("3. Realizar disparo\n");
        printf("4. Mostrar ranking\n");
        printf("5. Mostrar historial de disparos\n");
        printf("6. Guardar partida\n");
        printf("7. Cargar partida\n");
        printf("8. Cerrrar programa\n");
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
                    ranking(mejorJugador, jugadorActual);
                    guardarPartida(jugadorActual);
                    break;
                }
            }if(jugadorActual->disparos >= 10){
                printf("\n========= PERDISTE =========\n");
                printf("Te quedaste sin disparos\n");
                ranking(mejorJugador, jugadorActual);
                mostrarTablero(juego);
                guardarPartida(jugadorActual);
            }
            printf("Partida finalizada. Elija otra opcion o salga.\n");
            break;
        case 4:
            mostrarRanking(mejorJugador);
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
        
    } while (opcion !=8);
    
}
//PRINCIPAL
int main(){
    record jugadorActual;
    record mejorJugador;
    disparo disparo;
    juego juego;

    mejorJugador.disparos = 9999;
    strcpy(mejorJugador.nombre, "Nadie");
    cargarMejorJugador(&mejorJugador);

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
    menu(&juego, &disparo, &mejorJugador, &jugadorActual);
    getchar();

    //libera la memoria de las columnas
    for(int i = 0; i < juego.tamano; i++){
        free(juego.tablero[i]);
    }
    //libera la memoria de las filas
    free(juego.tablero);

    return 0;
}