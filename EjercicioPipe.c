#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int fd[2];
    char buffer[30];
    pid_t pid;
    int salida;

    pipe(fd);
    pid=fork();

    switch (pid)
    {
        case -1 : //Error
            printf("Error al crear el proceso hijo\n");
            salida = -1;
            break;
        case 0 : //Proceso hijo
            printf("El hijo escribe en la pipe\n");
            write(fd[1], "Hola papa", 10);
            close(fd[0]); //Cerramos el extremo de lectura
            salida=1;
            break;
        default:
            wait(NULL); //Esperamos a que termine el hijo
            printf("El padre lee de la pipe\n");
            read(fd[0], buffer, sizeof(buffer));
            printf("Mensaje leido: %s\n", buffer);
            salida=1;
            break;
    }
    return salida;
}
