#include <stdio.h>

typedef struct escuela {
    int año;
    int curso;
    char asistencia;
    int sanciones;
    
} escuela;

// 1. La función recibe el "mapa" (puntero) directo a un número entero
void corregirCurso(int *cursoEquivocado, int cursoCorrecto) {
    // 2. Usamos el asterisco (*) para viajar a la variable original y reemplazarla
    *cursoEquivocado = cursoCorrecto;
}

int main() {
    escuela alumno1;
    alumno1.curso = 5; // Supongamos que lo anotaron en 5to por error

    printf("Curso original: %d\n", alumno1.curso);

    // 3. Le pasamos a la función la dirección (&) del compartimento exacto que queremos cambiar
    corregirCurso(&alumno1.curso, 6);

    // Al imprimir de nuevo, verás que la variable original en el main sí cambió
    printf("Curso corregido: %d\n", alumno1.curso);

    return 0;
}