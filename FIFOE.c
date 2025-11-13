//Fifo escribe
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main()
{
    int fp;
    int salida=0;
    char saludo[]= "Un saludo!!!\n";

    fp= open("FIFO2", 1);
    if (fp==-1)
    {
        printf("Error al abrir la fifo\n");
        salida = -1;
    }

    if (salida == 0)
    {
        printf("Mandando informacion al FIFO...\n");
        write(fp, saludo, strlen(saludo));
        close(fp);
    }

    return salida  ;
}