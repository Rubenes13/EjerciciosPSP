#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    int valor = 6;


    printf("Valor inicial= %d\n", valor);
    fflush(stdout); // Asegurarse de que se imprima antes del fork

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Proceso hijo
        valor -= 5;
        printf("Hijo (pid=%d): valor = %d\n", getpid(), valor);
        exit(EXIT_SUCCESS);
    } else {

        // Proceso padre
        valor += 5;
        wait(NULL); // esperar al hijo (opcional, para sincronizar la salida)
        printf("Padre (pid=%d): valor = %d\n", getpid(), valor);
    }

    return 0;
}