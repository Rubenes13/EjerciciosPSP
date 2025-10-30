#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void main (void)
{
    char saludo [] = "Un saludo...\n";
    char buffer[10];
    int fd, bytesleidos;
    fd = open ("archivo.txt", 1);

    if (fd == -1)
    {
        printf ("Error al abrir el archivo\n");
        exit (-1);
    }
    printf ("Escribo saludo...\n");
    write (fd, saludo, strlen (saludo));
    close (fd);
    fd=open ("archivo.txt", 0);
    printf ("Leo saludo...\n");
    bytesleidos = read (fd, buffer, 1);
    while (bytesleidos > 0)
    {
        write (1, buffer, bytesleidos);
        bytesleidos = read (fd, buffer, 1);
    }
    close (fd);

} 