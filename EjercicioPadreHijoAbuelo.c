#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// ABUELO - HIJO - NIETO
int main() {
    pid_t pid, pid2;
    int fd1[2], fd2[2];
    int salida;
    char quienSoy[15];

    char saludoAbuelo[] = "Saludos del abuelo";
    char saludoPadre[]  = "Saludos del padre";
    char saludoHijo[]   = "Saludos del hijo";
    char saludoNieto[]  = "Saludos del nieto";

    char buffer[80] = "";

    pipe(fd1); // pipe para comunicacion de abuelo <-> hijo
    pipe(fd2); // pipe para comunicacion de hijo <-> nieto

    pid = fork(); // Soy el Abuelo, creo al hijo

    if (pid == -1) {
        perror("Error en fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // ==== HIJO ====
        pid2 = fork(); // Soy el Hijo, creo al nieto

        if (pid2 == -1) {
            perror("Error en fork");
            exit(EXIT_FAILURE);
        }

        if (pid2 == 0) {
            // ==== NIETO ====
            strcpy(quienSoy, "Nieto");

            // El nieto recibe del padre (fd2[0]) y envía al padre (fd2[1])
            close(fd1[0]);
            close(fd1[1]); // No usa el pipe abuelo-hijo

            printf("Nieto: leo del padre\n");
            read(fd2[0], buffer, sizeof(buffer));
            printf("Nieto: mensaje leido: %s\n", buffer);

            printf("Nieto: escribo al padre\n");
            write(fd2[1], saludoNieto, sizeof(saludoNieto));

            close(fd2[0]);
            close(fd2[1]);
