#include <stdlib.h>
#include <stdio.h>
#include "so.h"
#define MEMORYSIZE 20
struct so{
    cpu_t* cpu;
    relogio_t* relogio;
    int *programa;
};

so_t* so_cria(){
    so_t* s = (so_t*)malloc(sizeof(so_t));
    if(s != NULL){
        s->cpu = NULL;
        s->relogio = NULL;
    }
    return s;
}

void so_altera_cpu(so_t *so, cpu_t* cpu){
    so->cpu = cpu;
    cpu_estado_t *estado = cpu_estado_cria();
    mem_t *mem = mem_cria(MEMORYSIZE);
    for (int i = 0; i < MEMORYSIZE; i++) {
     if (mem_escreve(mem, i, so->programa[i]) != ERR_OK) {
        printf("Erro de memoria, endereco %d\n", i);
        exit(1);
     }
  }
    es_t *es = es_cria();
    cpu_altera_estado(cpu, estado);
    cpu_altera_memoria(cpu, mem);
    cpu_altera_es(cpu, es);

    return;
}
void interrupcao(so_t *so, err_t erro){
    int tempo = retorna_tempo(so->relogio);
    printf("\nErro na execucao: %d\n", erro);
    printf("\ntempo transcorrido: %d\n",tempo);
    return;
}
void so_altera_relogio(so_t *so, relogio_t* relogio){
    so->relogio = relogio;
    return;
}
void so_altera_programa(so_t *so, int *programa){
    so->programa = programa;
    return;
}
void so_libera(so_t *so){
    free(so);
    return;
}
