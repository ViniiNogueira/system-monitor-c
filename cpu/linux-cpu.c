#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_CPUS 1   // só o total cpu aumente para ler também cpu0, cpu1

typedef struct {
    unsigned long long user, nice, system, idle;
} CpuTimes;

int LeitorCpu(CpuTimes *t, int max) {

    FILE *fp = fopen("/proc/stat", "r");
    
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return -1;                      
    }

    char linha[256];
    int n = 0;

    while (n < max && fgets(linha, sizeof(linha), fp)) {
        if (strncmp(linha, "cpu", 3) != 0) {
            break;
        }

        CpuTimes c = {0};
        char *p = strchr(linha, ' ');
        if (!p) continue;

        int ok = sscanf(p, "%llu %llu %llu %llu",
                        &c.user, &c.nice, &c.system, &c.idle);
        if (ok < 4) continue;

        t[n++] = c;
    }

    fclose(fp);
    return n;
}

static double porcentagemUso(const CpuTimes *antes, const CpuTimes *depois) {
    unsigned long long totalAntes  = antes->user + antes->nice + antes->system + antes->idle;
    unsigned long long totalDepois = depois->user + depois->nice + depois->system + depois->idle;

    unsigned long long totalDiff = totalDepois - totalAntes;
    unsigned long long idleDiff  = depois->idle - antes->idle;

    if (totalDiff == 0)
        return 0.0;

    return 100.0 * (double)(totalDiff - idleDiff) / (double)totalDiff;
}

void printaBarra(double cpuPorcentUso) {
    int tamanho = 50;
    int blocos = (int)(cpuPorcentUso * tamanho / 100);

    printf("CPU [");
    for (int i = 0; i < blocos; i++)       printf("|");
    for (int i = blocos; i < tamanho; i++) printf("-");
    printf("] %.2f%%\n", cpuPorcentUso);
}

int main() {
    CpuTimes antes[MAX_CPUS];
    CpuTimes depois[MAX_CPUS];

    // primeira leitura, para ter com o que comparar
    if (LeitorCpu(antes, MAX_CPUS) <= 0) {
        return 1;
    }

    while (1) {
        sleep(1);                          

        if (LeitorCpu(depois, MAX_CPUS) <= 0) {
            break;
        }

        system("clear");
        printaBarra(porcentagemUso(&antes[0], &depois[0]));

        antes[0] = depois[0];              // a leitura atual vira a anterior
    }

    return 0;
}