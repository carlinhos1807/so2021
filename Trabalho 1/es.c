#include <stdlib.h>
#include <stdio.h>
#include "es.h"

struct es {
   err_t erro;
};

es_t* es_cria(){
    es_t* e = (es_t*)malloc(sizeof(es_t));
    e->erro = ERR_OK;
    return e;
}

err_t es_le(es_t *es, int dispositivo, int *pvalor){
    int leitura;
    if(dispositivo != 0){
        es->erro = ERR_ES_DISP_INV;
        return es->erro;
    }

    scanf("%d",&leitura);
    *pvalor = leitura;

    es->erro = ERR_OK;
    return es->erro;
}

err_t es_escreve(es_t *es, int dispositivo, int valor){
    if(dispositivo != 1){
        es->erro = ERR_ES_DISP_INV;
        return es->erro;
    }
    printf("escrita de E/S : %d\n", valor);
    es->erro = ERR_OK;
    return es->erro;
}

void es_destroi(es_t *es){
    free(es);
}
