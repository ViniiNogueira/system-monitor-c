#include <stdio.h>

#include "utils/utils.h"
#include "memoria/memoria.h"
#include "cpu/cpu.h"

int main(void)
{
    while (1)
    {
        limpaTela();       // limpa uma vez so, aqui no main

        informaMemoria();
        informaCPU();
        printf("Digite ctrl + c para finalizar o programa \n");

        esperaUmSegundo();
    }

    return 0;
}
