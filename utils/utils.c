#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

#include "utils.h"

// desenha a barra: nome = "RAM", "CPU"...
void printaBarra(const char *nome, double porcentagemUso)
{
    int tamanho = 50;
    int blocos = (int)(porcentagemUso * tamanho / 100);

    printf("%s [", nome);

    for (int i = 0; i < blocos; i++)
    {
        printf("|");
    }

    for (int i = blocos; i < tamanho; i++)
    {
        printf("-");
    }

    printf("] %.2f%%\n", porcentagemUso);
}

void limpaTela()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void esperaUmSegundo()
{
#ifdef _WIN32
    Sleep(1000);   // Windows conta em milissegundos
#else
    sleep(1);      // Linux conta em segundos
#endif
}
