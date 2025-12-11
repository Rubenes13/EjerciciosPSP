#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int fd1[2]; // Pipe Padre hacia Hijo 
    int fd2[2]; // Pipe Hijo hacia Padre 
    pid_t pid;
    char buffer[100];
    char mensajePadre[] = "Saludos del Padre..";
    char mensajeHijo[] = "Saludos del Hijo...";
    int salida=0;

    // Crear los dos pipes antes del fork
    if (pipe(fd1) == -1 || pipe(fd2) == -1) { 
        perror("Error creando pipes");
        salida = -1;
    }

    pid = fork(); //

    switch (pid) {
        case -1: // Error
            perror("Error en fork");
            salida = -1;
            break;

        case 0: // Código del Proceso Hijo
            // 1. Cerrar extremos no utilizados en el hijo:
            // No escribe en fd1, no lee de fd2
            close(fd1[1]); 
            close(fd2[0]); 

            // 2. Leer mensaje del padre
            read(fd1[0], buffer, sizeof(buffer)); //
            printf("HIJO RECIBE MENSAJE DE PADRE: %s\n", buffer);
            close(fd1[0]); // Cerrar extremo de lectura tras usarlo

            // 3. Enviar respuesta al padre
            printf("HIJO ENVIA MENSAJE a su PADRE.\n");
            write(fd2[1], mensajeHijo, strlen(mensajeHijo) + 1); //
            close(fd2[1]); // Cerrar extremo de escritura tras usarlo

            break;

        default: // Código del Proceso Padre
            // 1. Cerrar extremos no utilizados en el padre:
            // No lee de fd1, no escribe en fd2
            close(fd1[0]); 
            close(fd2[1]); 

            // 2. Enviar mensaje al hijo
            printf("PADRE ENVIA MENSAJE.\n");
            write(fd1[1], mensajePadre, strlen(mensajePadre) + 1); //
            close(fd1[1]); // Cerrar extremo de escritura tras usarlo

            // 3. Esperar y leer respuesta del hijo
            wait(NULL); // Esperar a que el hijo termine su tarea
            read(fd2[0], buffer, sizeof(buffer)); //
            printf("PADRE RECIBE MENSAJE del HIJO: %s\n", buffer);
            close(fd2[0]); // Cerrar extremo de lectura tras usarlo

            break;
    }

    return salida;
}
