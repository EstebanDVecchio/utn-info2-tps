#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definici�n de la estructura para el jugador
typedef struct {
	char nombre[51]; 
	int puntaje;
} Jugador;

// Prototipos de las funciones
Jugador* agregarJugador(Jugador *lista, int *cantidad);
void mostrarJugadores(Jugador *lista, int cantidad);
void buscarPorNombre(Jugador *lista, int cantidad);
void buscarPorPuntaje(Jugador *lista, int cantidad);
void limpiarBuffer();
void mostrarMenu();

int main() {
	// Iniciamos con el array completamente vac�o (NULL) y 0 elementos
	Jugador *ranking = NULL;
	int cantidad = 0; 
	int opcion;
	
	do {
		mostrarMenu();
		
		scanf("%d", &opcion);
		limpiarBuffer(); 
		
		switch (opcion) {
		case 1:
			// Asignamos el resultado del return para actualizar el puntero (por si cambio de lugar)
			ranking = agregarJugador(ranking, &cantidad);
			break;
		case 2:
			mostrarJugadores(ranking, cantidad);
			break;
		case 3:
			buscarPorNombre(ranking, cantidad);
			break;
		case 4:
			buscarPorPuntaje(ranking, cantidad);
			break;
		case 0:
			printf("Saliendo del programa.\n");
			break;
		default:
			printf("Opcion no valida. Intente de nuevo.\n");
		}
	} while (opcion != 0);
	
	// Ya termino el programa, y debo liberar la memoria
	if (ranking != NULL) {
		free(ranking);
	}
	return 0;
}

void mostrarMenu() {
	printf("\n--- MENU DE RANKING DE JUGADORES ---\n");
	printf("1. Agregar jugador y puntaje\n");
	printf("2. Mostrar ranking\n");
	printf("3. Buscar jugador por nombre\n");
	printf("4. Buscar jugador por puntaje\n");
	printf("\n");
	printf("0. Salir\n");
	printf("\n");
	printf("Seleccione una opcion: ");
	return;
}

// Funci�n que agrega 1 jugador (como es asignacion dinamica devuelve el puntero para actualizar
//  la direcci�n de memoria donde est� el nuevo arreglo)
Jugador* agregarJugador(Jugador *lista, int *cantidad) {
	int nuevaCantidad = 0;
	char otroNombre[51];
	int nuevoPuntaje = 0;
	nuevaCantidad = *cantidad + 1;
	
	// realloc funciona como malloc la primera vez si 'lista' es NULL
	Jugador *temporal = (Jugador *) realloc(lista, nuevaCantidad * sizeof(Jugador));
	
	if (temporal == NULL) {
		printf("Error: No se pudo asignar memoria para el nuevo jugador.\n");
		return lista; 	// Si falla, retornamos el valor viejo sin cambiar nada
	}
	
	// Si todo sali� bien, actualizamos nuestro puntero de trabajo (ahora hay una nueva zona de
	// memoria que contiene todos los anteriores, y espacio para uno mas).
	lista = temporal;
	
	// Ingreso de datos en la �ltima posici�n creada (lo que dice cantidad, que todavia no cambia)
	printf("\nIngrese el nombre del jugador (hasta 50 caracteres): ");
	fgets(otroNombre, 51, stdin);
	//limpiarBuffer();
	otroNombre[strlen(otroNombre)-1]='\0';
	strcpy(lista[*cantidad].nombre, otroNombre);
	
	printf("Ingrese el puntaje de %s: ", lista[*cantidad].nombre);
	scanf("%d", &nuevoPuntaje);
	limpiarBuffer();
	lista[*cantidad].puntaje = nuevoPuntaje;
	
	// Incrementamos la cantidad total de jugadores
	(*cantidad)++;
	printf("Jugador agregado\n");
	
	// Devolvemos el nuevo puntero (podria ser el mismo si habia espacio a continuacion)
	return lista; 
}

// Listado de jugadores
void mostrarJugadores(Jugador *lista, int cantidad) {
	if (cantidad == 0 || lista == NULL) {
		printf("\nEl ranking esta vacio.\n");
		return;
	}
	
	printf("\n=== RANKING ACTUAL ===\n");
	printf("Pos  | Nombre                    | Puntaje\n");
	printf("-----------------------------------------\n");
	for (int i = 0; i < cantidad; i++) {
		printf("%-4d | %-25s | %d\n", i + 1, lista[i].nombre, lista[i].puntaje);
	}
}

void buscarPorNombre(Jugador *lista, int cantidad) {
	int i = 0;
	char buscado[51];		// aca guardo el jugador a buscar
	int encontrado = 0;		// marca para saber si lo encontre
	
	memset(buscado, '\0', sizeof(buscado));
	
	if (cantidad == 0 || lista == NULL) {
		printf("\nEl ranking esta vacio.\n");
		return;
	}
	
	printf("\nIngrese el nombre del jugador a buscar: ");
	fgets(buscado, 51, stdin);
	//limpiarBuffer();
	buscado[strlen(buscado)-1] = '\0';
	printf("Buscando: %s", buscado);
	for (i = 0; i < cantidad; i++) {
		printf(".");
		if (strcmp(lista[i].nombre, buscado) == 0) {
			printf("\nEncontrado: \n");
				   printf("%s - Puntaje: %d  en la posicion %d\n", lista[i].nombre, lista[i].puntaje, i + 1);
						  encontrado = 1;	// lo encontre
		}
	}
	
	if (!encontrado) {
		printf("\nNo se encontro ningun jugador llamado: %s\n", buscado);
	}
}

void buscarPorPuntaje(Jugador *lista, int cantidad) {
	int i = 0;
	int puntajeABuscar;	// aca guardo el puntaje a buscar
	int encontrado = 0;		// marca para saber si lo encontre
	
	if (cantidad == 0 || lista == NULL) {
		printf("\nEl ranking esta vacio.\n");
		return;
	}
	
	printf("\nIngrese el puntaje a buscar: ");
	scanf("%d", &puntajeABuscar);
	limpiarBuffer();
	
	for (i = 0; i < cantidad; i++) {
		if (lista[i].puntaje == puntajeABuscar) {
			printf("Encontrado:\n");
				   printf("%s - Puntaje: %d en la posicion %d\n", lista[i].nombre, lista[i].puntaje, i + 1);
						  encontrado = 1;	// lo encontre
		}
	}
	
	if (!encontrado) {
		printf("No se encontraron jugadores con el puntaje: %d\n", puntajeABuscar);
	}
}

void limpiarBuffer() {
	int c;
	while ((c = getchar()) != EOF && c != '\n');
	//fseek(stdin, 0, SEEK_END);
	//rewind(stdin);
	//fflush(stdin);
}