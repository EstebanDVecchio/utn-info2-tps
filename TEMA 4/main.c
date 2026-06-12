#include <stdio.h>


struct escuela{
    int año;
    int curso;
    char asistencia;
    int sanciones;
};

int main(){
    
    struct escuela alumno[3];
    
    for(int i = 0; i < 3; i++){
        printf("\n------DATOS------\n");
        printf("En que año cursa el alumno %d\n", i + 1);
        scanf("%d",&alumno[i].año);

        printf("En que curso está?\n");
        scanf("%d",&alumno[i].curso);

        printf("Esta presente s/n?\n");
        scanf(" %c",&alumno[i].asistencia);

        printf("Cuantas sanciones?\n");
        scanf("%d",&alumno[i].sanciones);
    }

    struct escuela *explorador = alumno;

    printf("\n-----Alumnos con sanciones-----\n");
    for (int i = 0; i < 3; i++){
        if(explorador->sanciones >=1){
            printf("El alumno %d (Año %d, Curso %d) tiene %d sancion(es).\n", i + 1, explorador->año, explorador->curso, explorador->sanciones);
        }
        explorador++;
    }
}