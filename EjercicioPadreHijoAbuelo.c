#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
//ABUELO-HIJO-NIETO 
void main(){
    pid_t pid, Hijo_pid, pid2, Hijo2_pid;

    int fd1[2], fd2[2];
    int salida;
    char quienSoy[15];

    char saludoAbuelo[]="Saludos del abuelo\0";
    char saludoPadre[]="Saludos del padre\0";
    char saludoHijo[]="Saludos del hijo\0";
    char saludoNieto[]="Saludos del nieto\0";

    char buffer[80]="";

    pipe(fd1); //pipe para comunicacion de padre a hijo
    pipe(fd2); //pipe para comunicacion de hijo a padre

    pid = fork(); //Soy el Abuelo, creo a hijo

    if (pid==-1){
        perror("Error en fork");
        salida = -1;
    }

    if (pid==0){
        pid2 = fork(); //Soy el Hijo, creo a nieto
        switch (pid2){
            case -1:
                perror("Error en fork");
                salida = -1;
                break;
            case 0: //Soy el Nieto
                strcpy(quienSoy, "Nieto");
                //El nieto recive del padre, envia al padre, recibe del abuelo, envia al abuelo
                close (fd1[0]); //Cierro lectura de pipe abuelo-hijo
                close (fd2[1]); //Cierro escritura de pipe hijo-abuelo
                printf("Nieto: escribo al padre\n");
                write (fd2[0], saludoNieto, sizeof(saludoNieto));
                printf("Nieto: leo del abuelo\n");
                read (fd1[1], buffer, sizeof(buffer));
                printf("Nieto: mensaje leido: %s\n", buffer);
                close (fd2[0]); //Cierro lectura de pipe hijo-abuelo
                close (fd1[1]); //Cierro escritura de pipe abuelo-hijo
                salida=0;
                break;
            default: //Proceso padre
                strcpy(quienSoy, "Hijo");
                //El hijo recive del abuelo, envia al nieto, recive del nieto, envia al abuelo
                close (fd1[1]); //Cierro escritura de pipe abuelo-hijo
                close (fd2[0]); //Cierro lectura de pipe hijo-abuelo
                printf("Hijo: leo del abuelo\n");
                read (fd1[0], buffer, sizeof(buffer));
                printf("Hijo: mensaje leido: %s\n", buffer);
                printf("Hijo: escribo al nieto\n");
                write (fd2[1], saludoPadre, sizeof(saludoPadre));
                printf("Hijo: leo del nieto\n");
                read (fd2[0], buffer, sizeof(buffer));
                printf("Hijo: mensaje leido: %s\n", buffer);
                printf("Hijo: escribo al abuelo\n");
                write (fd1[1], saludoHijo, sizeof(saludoHijo));
                close (fd1[0]); //Cierro lectura de pipe abuelo-hijo
                close (fd2[1]); //Cierro escritura de pipe hijo-abuelo

                salida=0;
                break;
        }
    }
    else{ //Soy el abuelo
        strcpy(quienSoy, "Abuelo");
        //El abuelo envia al hijo, recive del hijo
        close (fd1[0]); //Cierro lectura de pipe abuelo-hijo
        close (fd2[1]); //Cierro escritura de pipe hijo-abuelo
        printf("Abuelo: escribo al hijo\n");
        write (fd1[1], saludoAbuelo, sizeof(saludoAbuelo));
        printf("Abuelo: leo del hijo\n");
        read (fd2[0], buffer, sizeof(buffer));
        printf("Abuelo: mensaje leido: %s\n", buffer);
        close (fd1[1]); //Cierro escritura de pipe abuelo-hijo
        close (fd2[0]); //Cierro lectura de pipe hijo-abuelo
        salida=0;
    }
    printf("Fin del %s\n", quienSoy);
    exit(salida);
}