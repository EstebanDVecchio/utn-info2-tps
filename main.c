#include <stdio.h>
#include <stdlib.h>

typedef struct escuela{
    int año;
    int curso;
    char asistencia;
    int sanciones;
}escuela;

int main(){

    int cantidad;

    printf("Cuantos alumnos vas a registrar hoy: ");
    scanf("%d", &cantidad);
    //Memoria
    escuela *alumno = malloc (cantidad *sizeof(escuela));
    
    if(alumno == NULL){
        printf("ERROR: Memoria");
        free(alumno);
        return 1;
    }

    for(int i = 0; i < cantidad; i++){
        printf("\n------DATOS ALUMNO %d------\n", i + 1);
        printf("En que año cursa el alumno: ");
        scanf("%d",&alumno[i].año);

        printf("En que curso esta?: ");
        scanf("%d",&alumno[i].curso);

        printf("Esta presente s/n?: ");
        scanf(" %c",&alumno[i].asistencia);

        printf("Cuantas sanciones?: ");
        scanf("%d",&alumno[i].sanciones);
    }
    //Abre el archivo para el reporte de los alumnos
    FILE *archivo = fopen("reporte_alumno.txt","w");

    if (archivo==NULL){
        printf("Error\n");
        return 1;
    }

    escuela *explorador = alumno;

    fprintf(archivo, "\n-----Alumnos con sanciones-----\n");
    for (int i = 0; i < cantidad; i++){
        if(explorador->sanciones >=1){
            fprintf(archivo, "El alumno %d (Año %d, Curso %d) tiene %d sancion(es).\n", i + 1, explorador->año, explorador->curso, explorador->sanciones);
        }
        explorador++;
    }

    fprintf(archivo, "\n------Asistencias-----\n");
    //Reinicia el puntero
    explorador = alumno;

    for (int i = 0; i < cantidad; i++){
        if(explorador->asistencia == 'S' || explorador->asistencia == 's'){
            fprintf(archivo, "El alumno %d esta presente\n", i + 1);
        }else{
            fprintf(archivo, "El alumno %d esta ausente\n", i + 1);
        }
        explorador++;
    }
    //Cierra el archivo
    fclose(archivo);
    free(alumno);
}

