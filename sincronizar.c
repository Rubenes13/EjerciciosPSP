#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>

// Gestion del proceso padre
void gestionPadre( int sig ){
    printf("Padre ha recibido la señal %d\n", sig);
}
//Gestion del proceso hijo
void manejador( int sig ){
    printf("Hijo ha recibido la señal %d\n", sig);
}

int main() {
    pid_t pid_hijo, pid_padre;
    int salida = 0;

    pid_padre = getpid();
    pid_hijo = fork();

    switch (pid_hijo) {
        case -1:
            perror("Error al crear el proceso hijo");
            salida = -1;
            break;
        case 0: // Proceso hijo
            signal(SIGUSR1, manejador);
            printf("Hijo esperando señal...\n");
            while(1){
                sleep(1);
                kill(pid_padre, SIGUSR1); // Mantiene el proceso vivo para recibir señales
                pause();
            };
        default: // Proceso padre
            signal(SIGUSR1, gestionPadre);
            while(1){
                pause();
                sleep(1); // Espera para asegurar que el hijo esté listo
                kill(pid_hijo, SIGUSR1);
            }
            break;

    }
    return salida;
}
