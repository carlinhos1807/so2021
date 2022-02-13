#include <stdlib.h>
#include <stdbool.h>
#include "mmu.h"

struct pagina {
    bool valido;
    int quadro;
    bool acessado;
    bool alterado;
};
struct tabpag {
    pagina_t *paginas;
    int fal_pag;

};
struct mmu {
    tabpag_t *tabpag;
    mem_t *memoria;
};

pagina_t *paginas_cria()
{
    pagina_t *paginas = (pagina_t*) malloc (numero_paginas*sizeof (pagina_t));
    if(paginas != NULL){
        for(int i = 0; i < numero_paginas;i++){
            paginas[i].acessado = false;
            paginas[i].alterado = false;
            paginas[i].quadro = -1;
            paginas[i].valido = false;
        }
    }
    return paginas;
}
void associa(tabpag_t *tabpag, int numero_pagina, int quadro)
{
    tabpag->paginas[numero_pagina].valido = true;
    tabpag->paginas[numero_pagina].quadro = quadro;
    return;
}
tabpag_t *tabpag_cria()
{
    pagina_t *paginas = paginas_cria();
    if(paginas != NULL){
        tabpag_t *tab = (tabpag_t*) malloc (sizeof (tabpag_t));
        if(tab != NULL){
            tab->paginas = paginas;
            tab->fal_pag = -1;
        }
        return tab;
    }
    return NULL;
}

mmu_t* mmu_cria()
{
    mmu_t* mmu = (mmu_t*)malloc(sizeof(mmu_t));
    if(mmu != NULL){
        mmu->tabpag = NULL;
        mmu->memoria = NULL;
    }
    return mmu;
}
void mmu_altera_memoria(mmu_t *mmu, mem_t *mem)
{
    mmu->memoria = mem;
    return;
}
err_t calcula_endereco_fisico(mmu_t *mmu, int endereco_virtual,int *endereco_fisico, int numero_pagina)
{
    if(numero_pagina > numero_paginas){
        return ERR_MMU_END_INV;
    }
    int deslocamento = endereco_virtual % tamanho;
    if(!mmu->tabpag->paginas[numero_pagina].valido){
        mmu->tabpag->fal_pag = endereco_virtual;
        return ERR_MMU_FAL_PAG;
    }
    *endereco_fisico = mmu->tabpag->paginas[numero_pagina].quadro * tamanho + deslocamento;

    return ERR_OK;
}
err_t mmu_escreve(mmu_t *mmu, int endereco, int valor)
{
    int numero_pagina = endereco / tamanho;
    int endereco_fisico;
    err_t calculoResponse = calcula_endereco_fisico(mmu, endereco,&endereco_fisico, numero_pagina);
    if(calculoResponse != ERR_OK){
        return calculoResponse;
    }
    err_t escritaResponse = mem_escreve(mmu->memoria, endereco_fisico,valor);
    if(escritaResponse == ERR_OK){
        mmu->tabpag->paginas[numero_pagina].alterado = true;
        return ERR_OK;
    }
    return escritaResponse;
}
err_t mmu_le(mmu_t *mmu, int endereco, int *pvalor)
{
    int numero_pagina = endereco / tamanho;
    int endereco_fisico;
    err_t calculoResponse = calcula_endereco_fisico(mmu, endereco,&endereco_fisico, numero_pagina);
    if(calculoResponse != ERR_OK){
        return calculoResponse;
    }

    err_t leituraResponse = mem_le(mmu->memoria, endereco_fisico,pvalor);
    if(leituraResponse == ERR_OK){
        mmu->tabpag->paginas[numero_pagina].acessado = true;
        return ERR_OK;

    }
    return leituraResponse;
}
void mmu_altera_tabela_pagina(mmu_t *mmu, tabpag_t *tab)
{
    mmu->tabpag = tab;
    return;
}
void libera_tabela(tabpag_t *tab)
{
    free(tab->paginas);
    free(tab);
}
void mmu_libera(mmu_t *mmu)
{
    mem_destroi(mmu->memoria);
    free(mmu);
}
void reseta_pagina(tabpag_t *tab, int id)
{
    tab->paginas[id].acessado = false;
    tab->paginas[id].alterado = false;
    tab->paginas[id].quadro = -1;
    tab->paginas[id].valido = false;
    return;
}
void reseta_todas_paginas(tabpag_t *tab)
{
    for(int i =0; i < numero_paginas; i++){
        tab->paginas[i].acessado = false;
        tab->paginas[i].alterado = false;
        tab->paginas[i].quadro = -1;
        tab->paginas[i].valido = false;
    }
    return;
}
int tabela_retorna_fal_pag(tabpag_t *tab)
{
    return tab->fal_pag;
}
mem_t* mmu_retorna_memoria(mmu_t *mmu)
{
    return mmu->memoria;
}
bool verifica_alterado(tabpag_t *tab, int pagid)
{
    return tab->paginas[pagid].alterado;
}
