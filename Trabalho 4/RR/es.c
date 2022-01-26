#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "es.h"

struct es {
   err_t erro;
};

es_t* es_cria(){
    es_t* e = (es_t*)malloc(sizeof(es_t));
    if(e != NULL){
        e->erro = ERR_OK;
    }
    return e;
}
void ajusta_nome_arquivo_es(int pid, char *vetor){

    char identificador_processo[100];
    sprintf(identificador_processo,"%d", pid);
    strcat(vetor,identificador_processo);

    return;
}
err_t es_le(es_t *es, int dispositivo, int *pvalor, int pid){
   int leitura;

   if(dispositivo != 0){
       es->erro = ERR_ES_DISP_INV;
       return es->erro;
   }

   char nome_arquivo_leitura[15] = "leitura";
   ajusta_nome_arquivo_es(pid, nome_arquivo_leitura);
   FILE *arquivo_leitura;

   if ((arquivo_leitura = fopen(strcat(nome_arquivo_leitura,".txt"),"r")) == NULL){
      es->erro = ERR_ES_LE_FILE;
      return es->erro;
   }

   if(fscanf(arquivo_leitura,"%d", &leitura) == 0){
      es->erro = ERR_ES_LE_FILE;
      return es->erro;
   }

   fclose(arquivo_leitura);
    //scanf("%d",&leitura);
   *pvalor = leitura;

   es->erro = ERR_OK;
   return es->erro;
}

err_t es_escreve(es_t *es, int dispositivo, int valor, FILE *arquivo_escrita){
    if(dispositivo != 1){
        es->erro = ERR_ES_DISP_INV;
        return es->erro;
    }
    //printf("escrita de E/S : %d\n", valor);
    if(fprintf(arquivo_escrita,"%d\n",valor) < 0 || arquivo_escrita == NULL){
        es->erro = ERR_ES_ESCR_FILE;
        return es->erro;
    }
    es->erro = ERR_OK;
    return es->erro;
}

void es_destroi(es_t *es){
    free(es);
}
