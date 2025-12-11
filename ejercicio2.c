#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int fd_p2h[2]; // Pipe Padre hacia Hijo (Parent to Child)
    int fd_h2p[2]; // Pipe Hijo hacia Padre (Child to Parent)
    pid_t pid;
    char buffer[100];
    char mensajePadre[] = "Saludos del Padre..";
    char mensajeHijo[] = "Saludos del Hijo...";

    // Crear los dos pipes antes del fork
    if (pipe(fd_p2h) == -1 || pipe(fd_h2p) == -1) { //
        perror("Error creando pipes");
        exit(EXIT_FAILURE);
    }

    pid = fork(); //

    switch (pid) {
        case -1: // Error
            perror("Error en fork");
            exit(EXIT_FAILURE);
            break;

        case 0: // CÃ³digo del Proceso Hijo
            // 1. Cerrar extremos no utilizados en el hijo:
            // No escribe en p2h, no lee de h2p
            close(fd_p2h[1]); //
            close(fd_h2p[0]); //

            // 2. Leer mensaje del padre
            read(fd_p2h[0], buffer, sizeof(buffer)); //
            printf("HIJO RECIBE MENSAJE DE PADRE: %s\n", buffer);
            close(fd_p2h[0]); // Cerrar extremo de lectura tras usarlo

            // 3. Enviar respuesta al padre
            printf("HIJO ENVIA MENSAJE a su PADRE.\n");
            write(fd_h2p[1], mensajeHijo, strlen(mensajeHijo) + 1); //
            close(fd_h2p[1]); // Cerrar extremo de escritura tras usarlo

            exit(EXIT_SUCCESS);
            break;

        default: // CÃ³digo del Proceso Padre
            // 1. Cerrar extremos no utilizados en el padre:
            // No lee de p2h, no escribe en h2p
            close(fd_p2h[0]); //
            close(fd_h2p[1]); //

            // 2. Enviar mensaje al hijo
            printf("PADRE ENVIA MENSAJE.\n");
            write(fd_p2h[1], mensajePadre, strlen(mensajePadre) + 1); //
            close(fd_p2h[1]); // Cerrar extremo de escritura tras usarlo

            // 3. Esperar y leer respuesta del hijo
            wait(NULL); // Esperar a que el hijo termine su tarea
            read(fd_h2p[0], buffer, sizeof(buffer)); //
            printf("PADRE RECIBE MENSAJE del HIJO: %s\n", buffer);
            close(fd_h2p[0]); // Cerrar extremo de lectura tras usarlo

            break;
    }

    return 0;
}
