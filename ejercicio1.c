#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int i, salida = 0;

    // El padre creara¡ 3 hijos en un bucle
    for (i = 1; i <= 3; i++) {
        pid = fork(); //

        if (pid < 0) {
            perror("Error en fork");
            salida = -1;
        } else if (pid == 0) {
            // Codigo del Proceso Hijo
            // Imprime su número, el PID de su padre y su propio PID
            printf("Soy el hijo= %d, Mi padre es= %d, Mi PID= %d\n", i, getppid(), getpid());
            // Es importante que el hijo termine aquí para no continuar el bucle y crear más procesos
            exit(EXIT_SUCCESS);
        }
    }

    // El padre espera a que terminen todos sus hijos para evitar procesos zombies
    // y para que su mensaje final salga después de los hijos (como en el ejemplo).
    for (i = 1; i <= 3; i++) {
        wait(NULL); //
    }

    // Visualiza el PID del padre de todos al final
    printf("Proceso PADRE = %d\n", getpid()); //

    return salida;
}
