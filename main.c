#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
#elif __linux__
    #include <sys/sysinfo.h>
    #include <sys/statvfs.h>
#endif

void printaBarra(double memoriaPorcentUso) // passa de parametro
{
    // variavel que define por quanto vai dividir para printar na barra
    int valor = 50;
    int blocos = (int)(memoriaPorcentUso * valor / 100); // faz o casting pra int para printa mais facil

    printf("RAM [");

    for (int i = 0; i < blocos; i++){
        printf("|");
    }

    for (int i = blocos; i < valor; i++){
        printf("-");
    }

    printf("] %.2f%%\n", memoriaPorcentUso);
}

void informaMemoria()
{
    MEMORYSTATUSEX memoryInfo;
    memoryInfo.dwLength = sizeof(memoryInfo);

    if (!GlobalMemoryStatusEx(&memoryInfo)) {

        printf("Erro ao consultar a memoria.\n");
        return;
    }

    DWORDLONG memoriaTotal = memoryInfo.ullTotalPhys; // para pegar a memoria fisica total
    DWORDLONG memoriaLivre = memoryInfo.ullAvailPhys; // para pegar a memoria fisica total disponivel em bytes
    DWORDLONG memoriaUsada = memoriaTotal - memoriaLivre; // calculo da memoria usada

    // calculo pra converter pra GB(são 3 veze para pular 3 casa  ->KB-> MB->GB)
    double memoriaTotalGB = memoriaTotal / (1024.0 * 1024.0 * 1024.0);
    double memoriaLivreGB = memoriaLivre / (1024.0 * 1024.0 * 1024.0);
    double memoriaUsadaGB = memoriaUsada / (1024.0 * 1024.0 * 1024.0);

    // porcentagem do quanto ta sendo usado
    //  memoria usada / memoria total * 100
    double memoriaPorcentUso = ((double)memoriaUsada / memoriaTotal) * 100;

    printf("=======================================\n");
    printf("Gerenciador de memoria \n");
    printf("=======================================\n");

    printf("Memoria Total: %.2f GB\n", memoriaTotalGB);
    printf("Memoria Livre: %.2f GB\n", memoriaLivreGB);
    printf("Memoria Usada: %.2f GB\n", memoriaUsadaGB);

    printaBarra(memoriaPorcentUso);

    // printf("Uso da memoria: %.2f%%\n", memoriaPorcentUso);

    printf("Digite ctrl + c para finalizar o programa \n");
}

int main()
{
    while (1)
    {
        system("cls");
        informaMemoria();
        Sleep(1000);
    }

    // mostrar a memoria do sistema (total)
    // a que esta em uso
    // a que ta livre
    // a porcentagem do quanto q ta usando
    return 0;
}