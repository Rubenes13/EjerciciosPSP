#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int fd1[2]; // ABUELO → HIJO
    int fd2[2]; // HIJO → NIETO
    int fd3[2]; // NIETO → HIJO
    int fd4[2]; // HIJO → ABUELO

    char msjabuelo[] = "Saludos del Abuelo.";
    char msjpadre[]  = "Saludos del Padre..";
    char msjhijo[]   = "Saludos del Hijo...";
    char msjnieto[]  = "Saludos del Nieto..";

    char buffer[80];
    pid_t pid_hijo, pid_nieto;

    pipe(fd1);
    pipe(fd2);
    pipe(fd3);
    pipe(fd4);

    pid_hijo = fork();

    if (pid_hijo == -1) {
        perror("Error al crear hijo");
        exit(EXIT_FAILURE);
    }

    if (pid_hijo == 0) {
        // ======== HIJO ========
        pid_nieto = fork();

        if (pid_nieto == -1) {
            perror("Error al crear nieto");
            exit(EXIT_FAILURE);
        }

        if (pid_nieto == 0) {
            // ======== NIETO ========
            close(fd1[0]); close(fd1[1]);
            close(fd2[1]); // lee de su padre
            close(fd3[0]); // escribe a su padre
            close(fd4[0]); close(fd4[1]);

            read(fd2[0], buffer, sizeof(buffer));
            printf("\t\tNIETO RECIBE mensaje de su padre: %s\n", buffer);
            printf("\t\tNIETO ENVIA MENSAJE a su padre...\n");
            write(fd3[1], msjnieto, sizeof(msjnieto));

            close(fd2[0]);
            close(fd3[1]);
            exit(0);
        } else {
            // ======== HIJO ========
            close(fd1[1]); // lee del abuelo
            close(fd2[0]); // escribe al nieto
            close(fd3[1]); // lee del nieto
            close(fd4[0]); // escribe al abuelo

            read(fd1[0], buffer, sizeof(buffer));
            printf("\tHIJO recibe mensaje de ABUELO: %s\n", buffer);

            printf("\t\tNIETO RECIBE mensaje de su padre: %s\n", buffer); // para mantener formato de sangría
            printf("\t\tNIETO ENVIA MENSAJE a su padre...\n");

            printf("\t\tNIETO RECIBE mensaje de su padre: %s\n", msjpadre);
            write(fd2[1], msjpadre, sizeof(msjpadre));

            read(fd3[0], buffer, sizeof(buffer));
            printf("\tHIJO RECIBE mensaje de su hijo: %s\n", buffer);

            printf("\tHIJO ENVIA MENSAJE a su padre...\n");
            write(fd4[1], msjhijo, sizeof(msjhijo));

            close(fd1[0]);
            close(fd2[1]);
            close(fd3[0]);
            close(fd4[1]);
            wait(NULL);
            exit(0);
        }

    } else {
        // ======== ABUELO ========
        close(fd1[0]); // escribe al hijo
        close(fd2[0]); close(fd2[1]);
        close(fd3[0]); close(fd3[1]);
        close(fd4[1]); // lee del hijo

        printf("ABUELO ENVIA MENSAJE AL HIJO...\n");
        write(fd1[1], msjabuelo, sizeof(msjabuelo));

        read(fd4[0], buffer, sizeof(buffer));
        printf("EL ABUELO RECIBE MENSAJE del HIJO: %s\n", buffer);

        close(fd1[1]);
        close(fd4[0]);
        wait(NULL);
    }

    return 0;
}
