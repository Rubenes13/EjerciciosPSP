#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Manejador para la señal SIGUSR1
void handle_sigusr1(int sig) {
    printf("Proceso padre (PID: %d) recibió SIGUSR1.\n", getpid());
}

int main() {
    pid_t pid_hijo;

    // Configurar el manejador para SIGUSR1 en el padre
    signal(SIGUSR1, handle_sigusr1);

    printf("Proceso padre iniciado (PID: %d).\n", getpid());

    pid_hijo = fork();

    if (pid_hijo < 0) {
        perror("Error al crear el proceso hijo");
        exit(EXIT_FAILURE);
    } else if (pid_hijo == 0) {
        // Código del proceso hijo
        printf("Proceso hijo iniciado (PID: %d, Padre PID: %d).\n", getpid(), getppid());

        // Enviar 3 señales SIGUSR1 al padre
        for (int i = 0; i < 3; i++) {
            printf("Hijo (PID: %d) enviando SIGUSR1 al padre (PID: %d). (Envío %d)\n", getpid(), getppid(), i + 1);
            kill(getppid(), SIGUSR1);
            sleep(1); // Esperar un poco entre señales
        }

        // Enviar señal SIGKILL al padre
        printf("Hijo (PID: %d) enviando SIGKILL al padre (PID: %d) para terminarlo.\n", getpid(), getppid());
        kill(getppid(), SIGKILL);

        printf("Hijo (PID: %d) terminando.\n", getpid());
        exit(EXIT_SUCCESS);
    } else {
        // Código del proceso padre
        printf("Padre (PID: %d) esperando las señales del hijo (PID: %d).\n", getpid(), pid_hijo);

        // El padre esperará indefinidamente o hasta que termine.
        // wait(NULL) no es adecuado aquí si el hijo mata al padre.
        // Podemos usar un bucle infinito o simplemente esperar el fin.
        while(1) {
            // Mantener al padre vivo para recibir señales
            sleep(1);
        }
    }

    return 0;
}
