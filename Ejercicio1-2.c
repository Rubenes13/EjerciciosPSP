#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2];
    char buffer[30];
    pid_t pid;
    int salida;
    char quienSoy[15];

    pipe(fd);
    pid=fork();

    switch (pid)
    {
        case -1 : //Error
            printf("Error al crear el proceso hijo\n");
            salida = -1;
            break;
        case 0 : //Proceso hijo
            close(fd[0]); //Cerramos el extremo de lectura
            strcpy(quienSoy, "Proceso Hijo");
            char mensaje[] = "Buenos dias padre";
            printf("El hijo escribe algo en la pipe\n");
            write(fd[1], mensaje, sizeof(mensaje));
            close(fd[1]); //Cerramos el extremo de escritura
            salida=1;
            printf("El hijo ha terminado de escribir\n");
            break;
        default:
            wait(NULL); //Esperamos a que termine el hijo
            close(fd[1]); //Cerramos el extremo de escritura
            strcpy(quienSoy, "Proceso Padre");
            printf("El padre lee de la pipe: ");
            read(fd[0], buffer, sizeof(buffer));
            printf("Mensaje leido: %s\n", buffer);
            salida=1;
            printf("El padre ha terminado de leer\n");
            break;
    }
    printf("Fin del %s\n", quienSoy);
    return salida;
}
