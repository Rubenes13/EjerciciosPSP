#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int i;

    // El padre crearÃ¡ 3 hijos en un bucle
    for (i = 1; i <= 3; i++) {
        pid = fork(); //

        if (pid < 0) {
            perror("Error en fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // CÃ³digo del Proceso Hijo
            // Imprime su nÃºmero, el PID de su padre y su propio PID
            printf("Soy el hijo= %d, Mi padre es= %d, Mi PID= %d\n", i, getppid(), getpid());
            // Es importante que el hijo termine aquÃ­ para no continuar el bucle y crear mÃ¡s procesos
            exit(EXIT_SUCCESS);
        }
        // El proceso padre continÃºa el bucle para crear el siguiente hijo
    }

    // El padre espera a que terminen todos sus hijos para evitar procesos zombies
    // y para que su mensaje final salga despuÃ©s de los hijos (como en el ejemplo).
    for (i = 1; i <= 3; i++) {
        wait(NULL); //
    }

    // Visualiza el PID del padre de todos al final
    printf("Proceso PADRE = %d\n", getpid()); //

    return 0;
}
