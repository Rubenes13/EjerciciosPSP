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
            salida = 0;
        } else {
            // ==== HIJO ====
            strcpy(quienSoy, "Hijo");

            // El hijo recibe del abuelo (fd1[0]), envía al nieto (fd2[1]),
            // luego recibe del nieto (fd2[0]) y envía al abuelo (fd1[1])

            close(fd1[1]); // Cierro escritura de abuelo->hijo (solo leo primero)
            close(fd2[0]); // Cierro lectura de hijo->nieto (solo escribo primero)

            printf("Hijo: leo del abuelo\n");
            read(fd1[0], buffer, sizeof(buffer));
            printf("Hijo: mensaje leido: %s\n", buffer);

            printf("Hijo: escribo al nieto\n");
            write(fd2[1], saludoPadre, sizeof(saludoPadre));

            // Ahora el hijo recibe del nieto y escribe al abuelo
            close(fd1[0]); // Ya leí del abuelo
            close(fd2[1]); // Ya escribí al nieto

            printf("Hijo: leo del nieto\n");
            read(fd2[0], buffer, sizeof(buffer));
            printf("Hijo: mensaje leido: %s\n", buffer);

            // Reabrir escritura hacia el abuelo
            int fd3[2];
            pipe(fd3);
            close(fd3[0]);
            printf("Hijo: escribo al abuelo\n");
            write(fd3[1], saludoHijo, sizeof(saludoHijo));
            close(fd3[1]);

            wait(NULL);
            salida = 0;
        }
    } else {
        // ==== ABUELO ====
        strcpy(quienSoy, "Abuelo");

        // El abuelo envía al hijo (fd1[1]) y recibe del hijo (fd1[0])
        close(fd1[0]); // Cierro lectura del pipe abuelo-hijo para escribir
        close(fd2[0]);
        close(fd2[1]); // No usa el pipe del nieto directamente

        printf("Abuelo: escribo al hijo\n");
        write(fd1[1], saludoAbuelo, sizeof(saludoAbuelo));

        close(fd1[1]); // Ya escribió

        // Simulamos que el hijo responde por el mismo canal
        pipe(fd1);
        close(fd1[1]);
        printf("Abuelo: leo del hijo\n");
        read(fd1[0], buffer, sizeof(buffer));
        printf("Abuelo: mensaje leido: %s\n", buffer);
        close(fd1[0]);

        wait(NULL);
        salida = 0;
    }

    printf("Fin del %s\n", quienSoy);
    exit(salida);
}

