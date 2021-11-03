#include <stdlib.h>
#include "memoria.h"

#define MEMORYSIZE 20


struct memoria {
   mem_err_t erro;
   int posicoes[MEMORYSIZE];
};

int mem_tam(mem_t *m){
    return MEMORYSIZE;
}

mem_t *mem_cria(int tam){
    mem_t *m = (mem_t*) malloc (sizeof (mem_t));
    m->erro = MEM_ERR_OK;
    return m;
}
void mem_destroi(mem_t *m){
    free(m);
}
mem_err_t mem_le(mem_t *m, int endereco, int *pvalor){
    if(endereco < 0 || endereco > (mem_tam(m)- 1)){
        m->erro = MEM_ERR_MEM_END_INV;
        return m->erro;
    }
    *pvalor = m->posicoes[endereco];
    m->erro = MEM_ERR_OK;
    return m->erro;
}
mem_err_t mem_escreve(mem_t *m, int endereco, int valor){
    if(endereco < 0 || endereco > (mem_tam(m)- 1)){
        m->erro = MEM_ERR_MEM_END_INV;
        return m->erro;
    }

    m->posicoes[endereco] = valor;
    m->erro = MEM_ERR_OK;
    return m->erro;
}
