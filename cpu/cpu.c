#ifdef _WIN32
    #define _WIN32_WINNT 0x0501
#endif

#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

#include "cpu.h"
#include "../utils/utils.h"

#include <stdio.h>


#ifdef _WIN32
static unsigned long long filetimeParaU64(FILETIME ft)
{
    ULARGE_INTEGER u;
    u.LowPart = ft.dwLowDateTime;
    u.HighPart = ft.dwHighDateTime;
    return u.QuadPart;
}
#endif

void informaCPU(void)
{
    // static: guarda o valor entre uma chamada e outra da funcao
    static unsigned long long anteriorTotal = 0;
    static unsigned long long anteriorIdle = 0;
    static int primeiraLeitura = 1;

    unsigned long long atualTotal; // tempo total da CPU (acumulado desde que ligou)
    unsigned long long atualIdle;  // tempo que a CPU ficou parada (acumulado)
    int nucleos;

    // so essa parte muda de um sistema pra outro: de onde vem o total e o idle
#ifdef _WIN32
    FILETIME idle, kernel, user;

    if (!GetSystemTimes(&idle, &kernel, &user))
    {
        printf("Erro ao consultar a CPU.\n");
        return;
    }

    atualIdle = filetimeParaU64(idle);
    atualTotal = filetimeParaU64(kernel) + filetimeParaU64(user); // kernel ja inclui o idle

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    nucleos = (int)sysInfo.dwNumberOfProcessors;
#else
    FILE *fp = fopen("/proc/stat", "r");

    if (fp == NULL)
    {
        printf("Erro ao abrir o /proc/stat.\n");
        return;
    }

    unsigned long long user, nice, sistema, idle;

    if (fscanf(fp, "cpu %llu %llu %llu %llu", &user, &nice, &sistema, &idle) != 4)
    {
        printf("Erro ao ler o /proc/stat.\n");
        fclose(fp);
        return;
    }
    fclose(fp);

    atualIdle = idle;
    atualTotal = user + nice + sistema + idle;
    nucleos = (int)sysconf(_SC_NPROCESSORS_ONLN);
#endif

    // daqui pra baixo e igual nos dois sistemas
    double cpuPorcentUso = 0.0;

    // na primeira chamada nao existe leitura anterior, entao o uso fica 0
    if (!primeiraLeitura)
    {
        unsigned long long diffTotal = atualTotal - anteriorTotal;
        unsigned long long diffIdle = atualIdle - anteriorIdle;

        if (diffTotal != 0)
        {
            cpuPorcentUso = (diffTotal - diffIdle) * 100.0 / (double)diffTotal;
        }
    }

    // a leitura atual vira a anterior para a proxima chamada
    anteriorTotal = atualTotal;
    anteriorIdle = atualIdle;
    primeiraLeitura = 0;

    printf("\n=======================================\n");
    printf("Gerenciador de CPU \n");
    printf("=======================================\n");

    printf("Nucleos logicos: %d\n", nucleos);

    printaBarra("CPU", cpuPorcentUso);
}
