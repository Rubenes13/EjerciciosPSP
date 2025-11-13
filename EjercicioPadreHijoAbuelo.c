#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    pid_t pid, pid2;
    int fd1[2], fd2[2];
    int salida = 0;
    char quienSoy[15];

    char saludoAbuelo[] = "Saludos del Abuelo.";
    char saludoPadre[]  = "Saludos del Padre..";
    char saludoHijo[]   = "Saludos del Hijo...";
    char saludoNieto[]  = "Saludos del Nieto..";
    char buffer[80] = "";

    pipe(fd1); // Pipe abuelo ↔ hijo
    pipe(fd2); // Pipe hijo ↔ nieto

    pid = fork(); // Crea al hijo

    switch (pid) {
        case -1:
            perror("Error en fork del hijo");
            salida = -1;
            break;

        case 0: // ======== HIJO ========
            pid2 = fork(); // El hijo crea al nieto

            switch (pid2) {
                case -1:
                    perror("Error en fork del nieto");
                    salida = -1;
                    break;

                case 0: // ======== NIETO ========
                    strcpy(quienSoy, "Nieto");
                    // El nieto lee del padre (fd2[0]) y responde (fd2[1])
                    close(fd1[0]); close(fd1[1]);
                    close(fd2[1]); // solo lee primero

                    read(fd2[0], buffer, sizeof(buffer));
                    printf("\t\tNIETO RECIBE mensaje de su padre: %s\n", buffer);
                    printf("\t\tNIETO ENVIA MENSAJE a su padre...\n");

                    close(fd2[0]);
                    pipe(fd2); // reutiliza el mismo pipe para escribir
                    close(fd2[0]);
                    write(fd2[1], saludoNieto, sizeof(saludoNieto));
                    close(fd2[1]);
                    salida = 0;
                    break;

                default: // ======== HIJO ========
                    strcpy(quienSoy, "Hijo");
                    // El hijo recibe del abuelo (fd1), escribe al nieto (fd2),
                    // recibe del nieto (fd2), y escribe al abuelo (fd1)

                    close(fd1[1]); // lee del abuelo
                    close(fd2[0]); // escribe al nieto

                    read(fd1[0], buffer, sizeof(buffer));
                    printf("\tHIJO recibe mensaje de ABUELO: %s\n", buffer);

                    printf("\t\tNIETO RECIBE mensaje de su padre: %s\n", saludoPadre);
                    write(fd2[1], saludoPadre, sizeof(saludoPadre));

                    close(fd2[1]);
                    wait(NULL); // Espera al nieto

                    pipe(fd2); // reutiliza el pipe para leer la respuesta del nieto
                    close(fd2[1]);
                    read(fd2[0], buffer, sizeof(buffer));
                    printf("\tHIJO RECIBE mensaje de su hijo: %s\n", buffer);
                    close(fd2[0]);

                    printf("\tHIJO ENVIA MENSAJE a su padre...\n");

                    pipe(fd1); // reutiliza pipe con el abuelo
                    close(fd1[0]);
                    write(fd1[1], saludoHijo, sizeof(saludoHijo));
                    close(fd1[1]);
                    salida = 0;
                    break;
            }
            exit(salida);

        default: // ======== ABUELO ========
            strcpy(quienSoy, "Abuelo");
            // El abuelo escribe al hijo (fd1[1]) y luego lee del hijo (fd1[0])
            close(fd1[0]); // solo escribe al inicio
            close(fd2[0]); close(fd2[1]);

            printf("ABUELO ENVIA MENSAJE AL HIJO...\n");
            write(fd1[1], saludoAbuelo, sizeof(saludoAbuelo));
            close(fd1[1]);

            wait(NULL); // Espera a hijo y nieto para sincronizar la salida

            pipe(fd1); // reutiliza pipe para leer respuesta del hijo
            close(fd1[1]);
            read(fd1[0], buffer, sizeof(buffer));
            printf("EL ABUELO RECIBE MENSAJE del HIJO: %s\n", buffer);
            close(fd1[0]);
            salida = 0;
            break;
    }

    exit(salida);
}
