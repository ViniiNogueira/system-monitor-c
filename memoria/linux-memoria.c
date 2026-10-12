#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
    int blocos = (int) (memoriaPorcentUso * valor / 100); // faz o casting pra int para printa mais facil

    printf("RAM [");

    for (int i = 0; i < blocos; i++){
        printf("|");
    }

    for (int i = blocos; i < valor; i++){
        printf("-");
    }

    printf("] %.2f%%\n", memoriaPorcentUso);
}

void memoriaLinux() {

    FILE *fp = fopen("/proc/meminfo", "r");     //Abre o arq. em modo leitura ("r")
    
    if (fp == NULL) { // Testa se o arquivo abriu corretamente
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    char linha[256];
    double totalKB = 0;
    double livreKB = 0;

    /*
    percorre linha por linha do arquivo (igual o try with resources do java)
    while le com o fgets, sizeof le o tamanho da linha no arquivo(fp)
    */
    while (fgets(linha ,sizeof(linha),fp ) )
    {
        sscanf(linha, "MemTotal: %lf kB", &totalKB);
        sscanf(linha, "MemAvailable: %lf kB", &livreKB);
    }
    fclose(fp);

    double memoriaTotalGB = totalKB / (1024.0 * 1024.0);
    double memoriaLivreGB = livreKB / (1024.0 * 1024.0);
    double memoriaUsadaGB = memoriaTotalGB - memoriaLivreGB;
    double memoriaPorcentUso = memoriaUsadaGB / memoriaTotalGB * 100;

    printf("Memoria Total: %.2f GB\n", memoriaTotalGB);
    printf("Memoria Livre: %.2f GB\n", memoriaLivreGB);
    printf("Memoria Usada: %.2f GB\n", memoriaUsadaGB);

    printaBarra(memoriaPorcentUso);

    printf("Digite ctrl + c para finalizar o programa \n");
}

int main () {
    
    while (1) {
        system("clear");
        memoriaLinux();
        sleep(1);
    }

    return 0;
}