#include <stdlib.h>
#include "memoria.h"


struct memoria {
   err_t erro;
   int tam;
   int *posicoes;
};

int mem_tam(mem_t *m)
{
    return m->tam;
}

mem_t *mem_cria(int tam)
{
    mem_t *m = (mem_t*) malloc (sizeof (mem_t));
    if(m != NULL){
        m->erro = ERR_OK;
        m->tam = tam;
        m->posicoes = (int *) malloc(tam*sizeof(int));
        if(m->posicoes == NULL){
            free(m);
            m = NULL;
        }
    }
    return m;
}
void mem_destroi(mem_t *m)
{
    free(m->posicoes);
    free(m);
}
err_t mem_le(mem_t *m, int endereco, int *pvalor)
{
    if(endereco < 0 || endereco > (mem_tam(m)- 1)){
        m->erro = ERR_MEM_END_INV;
        return m->erro;
    }
    *pvalor = m->posicoes[endereco];
    m->erro = ERR_OK;
    return m->erro;
}
err_t mem_escreve(mem_t *m, int endereco, int valor)
{
    if(endereco < 0 || endereco > (mem_tam(m)- 1)){
        m->erro = ERR_MEM_END_INV;
        return m->erro;
    }

    m->posicoes[endereco] = valor;
    m->erro = ERR_OK;
    return m->erro;
}
