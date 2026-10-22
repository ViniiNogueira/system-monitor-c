# Monitor de sistema

Aplicação de terminal escrita em C que acompanha o uso da CPU e da memória RAM em tempo real. A tela é atualizada a cada segundo e mostra os valores em texto e em barras.

## Recursos

- Uso da CPU em porcentagem e quantidade de núcleos lógicos.
- Memória RAM total, livre e usada.
- Barras de uso para CPU e RAM.
- Suporte a Windows e Linux.


## Requisitos

- GCC ou MinGW-w64.
- Windows ou Linux.

Não são necessárias bibliotecas externas.

## Compilar e executar

Execute os comandos a partir da raiz do projeto.

### Linux

```sh
gcc -std=c11 -Wall -Wextra -Wpedantic main.c cpu/cpu.c memoria/memoria.c utils/utils.c -o monitor
./monitor
```

No Linux, as informações são lidas de arquivos virtuais do `procfs`, que expõem estatísticas do kernel. Eles não são arquivos de configuração que o programa altera: são consultados somente para leitura a cada atualização.

#### Consulta da CPU

O monitor abre `/proc/stat` e lê a primeira linha, identificada por `cpu`. Nela, o kernel informa tempos acumulados desde a inicialização do sistema em diferentes estados da CPU, como execução de processos (`user`, `nice` e `system`) e tempo ocioso (`idle`).

O programa guarda uma leitura e, na atualização seguinte, compara os valores. A porcentagem é calculada pela variação do tempo ocupado em relação à variação do tempo total:

```text
uso (%) = (variação total - variação ociosa) / variação total * 100
```

Por isso, o primeiro valor calculado aparece após a segunda leitura. A quantidade de núcleos lógicos é consultada com `sysconf`.

#### Consulta da memória

O monitor percorre `/proc/meminfo` e consulta os campos `MemTotal` (memória total) e `MemAvailable` (estimativa de memória disponível para novos processos, incluindo memória que o Linux pode recuperar). Os valores desses campos são apresentados pelo kernel em kilobytes.

O programa converte os valores para gigabytes e calcula:

```text
memória usada = memória total - memória disponível
uso (%) = memória usada / memória total * 100
```

![Exemplo dos dados de memória disponíveis no Linux](docs-img/Linux-caminho-memoria.png)

### Windows

No terminal com GCC/MinGW-w64:

```sh
gcc -std=c11 -Wall -Wextra -Wpedantic main.c cpu/cpu.c memoria/memoria.c utils/utils.c -o monitor.exe
.\monitor.exe
```

Pressione `Ctrl+C` para encerrar o monitor.

## Estrutura

As implementações de CPU, memória e utilitários ficam separadas em módulos. A função principal coordena a atualização e a exibição das informações.

Onde cada parte tem seu pacote, memoria e cpu.
