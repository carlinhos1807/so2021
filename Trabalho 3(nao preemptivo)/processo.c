/*
#include <stdlib.h>
#include "processo.h"


struct processo {
   p_state_t estado;
   int *programa;
   int pid;
   int tempo_cpu;
   int tempo_bloqueio;
   int tempo_espera;
};

processo_t *processo_cria(int *programa, int pid, int quant_processos){
    processo_t *p = (processo_t*) malloc (quant_processos*sizeof (processo_t));
    if(p != NULL){
        for(int i = 0; i < quant_processos; i++){
            p[i].estado = PRONTO;
            p[i].programa = programa;
            p[i].pid = pid;
            p[i].tempo_cpu = 0;
            p[i].tempo_bloqueio = 0;
            p[i].tempo_espera = 0;
        }
    }


    return p;
}

void processo_destroi(processo_t *p){
    free(p);
}

*/

