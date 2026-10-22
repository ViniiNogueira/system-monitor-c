#include <stdio.h>

#ifdef _WIN32
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0501
    #endif
    #include <windows.h>
#endif

#include "memoria.h"
#include "../utils/utils.h"

void informaMemoria()
{
    double memoriaTotalGB;
    double memoriaLivreGB;

    // so essa parte muda de um sistema pra outro: de onde vem o total e o livre
#ifdef _WIN32
    MEMORYSTATUSEX memoryInfo;
    memoryInfo.dwLength = sizeof(memoryInfo);

    if (!GlobalMemoryStatusEx(&memoryInfo))
    {
        printf("Erro ao consultar a memoria.\n");
        return;
    }

    // bytes -> GB (divide por 1024 tres vezes: KB -> MB -> GB)
    memoriaTotalGB = memoryInfo.ullTotalPhys / (1024.0 * 1024.0 * 1024.0);
    memoriaLivreGB = memoryInfo.ullAvailPhys / (1024.0 * 1024.0 * 1024.0);
#else
    FILE *fp = fopen("/proc/meminfo", "r");

    if (fp == NULL)
    {
        printf("Erro ao abrir o /proc/meminfo.\n");
        return;
    }

    char linha[256];
    double totalKB = 0;
    double livreKB = 0;

    while (fgets(linha, sizeof(linha), fp))
    {
        sscanf(linha, "MemTotal: %lf kB", &totalKB);
        sscanf(linha, "MemAvailable: %lf kB", &livreKB);
    }
    fclose(fp);

    // KB -> GB (divide por 1024 duas vezes: KB -> MB -> GB)
    memoriaTotalGB = totalKB / (1024.0 * 1024.0);
    memoriaLivreGB = livreKB / (1024.0 * 1024.0);
#endif

    // daqui pra baixo e igual nos dois sistemas
    double memoriaUsadaGB = memoriaTotalGB - memoriaLivreGB;
    double memoriaPorcentUso = memoriaUsadaGB / memoriaTotalGB * 100;

    printf("=======================================\n");
    printf("Gerenciador de memoria \n");
    printf("=======================================\n");

    printf("Memoria Total: %.2f GB\n", memoriaTotalGB);
    printf("Memoria Livre: %.2f GB\n", memoriaLivreGB);
    printf("Memoria Usada: %.2f GB\n", memoriaUsadaGB);

    printaBarra("RAM", memoriaPorcentUso);
}
