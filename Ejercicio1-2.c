#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

int main(void) {
    int fd[2]; /* fd[0] = lectura, fd[1] = escritura */
    int status = EXIT_SUCCESS;

    /* Crear el pipe */
    if (pipe(fd) == -1) {
        perror("pipe");
        status = EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid == -1) {
        /* Error al crear el proceso hijo */
        perror("fork");
        close(fd[0]);
        close(fd[1]);
        status = EXIT_FAILURE;
    }

    if (pid == 0) {
        /* Proceso hijo: enviará un mensaje al padre */
        const char *msg = "Buenos días, padre!";
        /* El hijo no necesita el extremo de lectura */
        close(fd[0]);

        /* Escribir el mensaje en el extremo de escritura del pipe */
        
        ssize_t nw = write(fd[1], msg, strlen(msg) + 1); /* +1 para enviar el '\0' */
        if (nw == -1) {
             perror("write (hijo)");
             close(fd[1]);
        }
        printf("Hijo ha escrito algo en la pipe.\n");
        /* Cerrar el extremo de escritura después de enviar */
        close(fd[1]);
    } else {
        /* Proceso padre: recibirá el mensaje del hijo */
        char buffer[256];
        ssize_t nr;

        /* El padre no necesita el extremo de escritura */
        close(fd[1]);

        /* Leer desde el pipe. read devuelve 0 cuando el hijo cierra el extremo de escritura (EOF). */
        nr = read(fd[0], buffer, sizeof(buffer) - 1);
        if (nr == -1) {
             perror("read (padre)");
             close(fd[0]);
             wait(NULL);
             status = EXIT_FAILURE;
        }

        /* Asegurar terminación nula para tratar buffer como cadena */
        if (nr >= 0) buffer[nr] = '\0';

        /* Mostrar el mensaje recibido */
        printf("Padre recibió algo de la pipe: %s\n", buffer);

        /* Cerrar el extremo de lectura */
        close(fd[0]);

        /* Esperar a que el hijo termine para evitar zombis */
        wait(NULL);
    }
    return status;
}