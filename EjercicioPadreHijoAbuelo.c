#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int salida = 0;
    int fd1[2]; // ABUELO → HIJO
    int fd2[2]; // HIJO ↔ NIETO y HIJO ↔ ABUELO (reutilizado)

    char msjabuelo[] = "Saludos del Abuelo.";
    char msjpadre[]  = "Saludos del Padre..";
    char msjhijo[]   = "Saludos del Hijo...";
    char msjnieto[]  = "Saludos del Nieto..";

    char buffer[80];
    pid_t pid_hijo, pid_nieto;

    pipe(fd1);
    pipe(fd2);

    pid_hijo = fork();

    if (pid_hijo == -1) {
        perror("Error al crear hijo");
        salida = -1;
    }

    if (pid_hijo == 0) {
        // ======== HIJO ========
        pid_nieto = fork();

        if (pid_nieto == -1) {
            perror("Error al crear nieto");
            salida = -1;
        }

        if (pid_nieto == 0) {
            // ======== NIETO ========
            close(fd1[0]); close(fd1[1]); // No usa el pipe abuelo-hijo
            close(fd2[1]); // Lee del padre
            read(fd2[0], buffer, sizeof(buffer));
            printf("\t\tNIETO RECIBE mensaje de su padre: %s\n", buffer);
            printf("\t\tNIETO ENVIA MENSAJE a su padre...\n");
            close(fd2[0]);

            // Reabrimos el pipe de escritura hacia el padre
            pipe(fd2);
            write(fd2[1], msjnieto, sizeof(msjnieto));
            close(fd2[1]);
            exit(0);
        } else {
            // ======== HIJO ========
            close(fd1[1]); // lee del abuelo
            close(fd2[0]); // escribe al nieto

            read(fd1[0], buffer, sizeof(buffer));
            printf("\tHIJO recibe mensaje de ABUELO: %s\n", buffer);

            printf("\t\tNIETO RECIBE mensaje de su padre: %s\n", msjpadre);
            write(fd2[1], msjpadre, sizeof(msjpadre));

            close(fd1[0]);
            close(fd2[1]);
            wait(NULL);

            // Recibir mensaje del nieto
            pipe(fd2);
            close(fd2[1]);
            read(fd2[0], buffer, sizeof(buffer));
            printf("\tHIJO RECIBE mensaje de su hijo: %s\n", buffer);
            close(fd2[0]);

            printf("\tHIJO ENVIA MENSAJE a su padre...\n");

            // Enviar mensaje al abuelo reutilizando fd1
            pipe(fd1);
            close(fd1[0]);
            write(fd1[1], msjhijo, sizeof(msjhijo));
            close(fd1[1]);
            exit(0);
        }

    } else {
        // ======== ABUELO ========
        close(fd1[0]); // escribe al hijo
        close(fd2[0]); close(fd2[1]);

        printf("ABUELO ENVIA MENSAJE AL HIJO...\n");
        write(fd1[1], msjabuelo, sizeof(msjabuelo));
        close(fd1[1]);

        // Reabrimos fd1 para leer respuesta del hijo
        pipe(fd1);
        close(fd1[1]);
        read(fd1[0], buffer, sizeof(buffer));
        printf("EL ABUELO RECIBE MENSAJE del HIJO: %s\n", buffer);
        close(fd1[0]);

        wait(NULL);
    }

    return 0;
}
