//Fifo lee
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    int fp;
    int p, bytesLeidos, salida=0;
    char buffer[10];

    p=mkfifo("FIFO2", S_IFIFO|0666);

    if (p==-1)
    {
        printf("Error al crear la fifo\n");
        salida = -1;
    }
    while(1)
    {
        fp=open("FIFO2", 0);
        bytesLeidos=read(fp, buffer, 1);
        printf("Obteniendo informacion...\n");
        while (bytesLeidos!=0)
        {
            printf("%s", buffer);
            bytesLeidos=read(fp, buffer, sizeof(buffer));
        }
        close(fp);
    }


    return salida  ;
}