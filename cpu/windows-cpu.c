#define _WIN32_WINNT 0x0501
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

ULONGLONG conversor_Filetime_To_U64(FILETIME ft)
{
    ULARGE_INTEGER u;
    u.LowPart = ft.dwLowDateTime;
    u.HighPart = ft.dwHighDateTime;
    return u.QuadPart;
}

void printaBarra(double cpuPorcentUso)
{
    // tamanho total da barra
    int valor = 50;
    int blocos = (int)(cpuPorcentUso * valor / 100);

    printf("CPU [");

    for (int i = 0; i < blocos; i++)
    {
        printf("|");
    }

    for (int i = blocos; i < valor; i++)
    {
        printf("-");
    }

    printf("] %.2f%%\n", cpuPorcentUso);
}

void informaCPU()
{
    // static: guarda o valor entre uma chamada e outra da funcao
    static ULONGLONG previewIdle = 0;
    static ULONGLONG previewKernel = 0;
    static ULONGLONG previewUser = 0;
    static int primeiraLeitura = 1;

    FILETIME idle;   // tempo que a cpu ficou parada
    FILETIME kernel; // tempo usando o OS (inclui o idle)
    FILETIME user;   // tempo usando programas normalmente

    if (!GetSystemTimes(&idle, &kernel, &user))
    {
        printf("Erro ao consultar a CPU.\n");
        return;
    }

    ULONGLONG atualIdle = conversor_Filetime_To_U64(idle);
    ULONGLONG atualKernel = conversor_Filetime_To_U64(kernel);
    ULONGLONG atualUser = conversor_Filetime_To_U64(user);

    double cpuPorcentUso = 0.0;

    // na primeira chamada nao existe leitura anterior, entao o uso fica 0
    if (!primeiraLeitura)
    {
        ULONGLONG dIdle = atualIdle - previewIdle;
        ULONGLONG dKernel = atualKernel - previewKernel;
        ULONGLONG dUser = atualUser - previewUser;

        ULONGLONG total = dKernel + dUser;
        ULONGLONG ocupado = total - dIdle;

        if (total != 0)
        {
            cpuPorcentUso = (ocupado * 100.0) / (double)total;
        }
    }

    // a leitura atual vira a anterior para a proxima chamada
    previewIdle = atualIdle;
    previewKernel = atualKernel;
    previewUser = atualUser;
    primeiraLeitura = 0;

    // quantidade de nucleos logicos
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    printf("=======================================\n");
    printf("Gerenciador de CPU \n");
    printf("=======================================\n");

    printf("Nucleos logicos: %lu\n", sysInfo.dwNumberOfProcessors);

    printaBarra(cpuPorcentUso);

    printf("Digite ctrl + c para finalizar o programa \n");
}

int main()
{
    while (1)
    {
        system("cls");
        informaCPU();
        Sleep(1000);
    }

    return 0;
}