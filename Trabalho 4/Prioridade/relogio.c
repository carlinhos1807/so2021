#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "relogio.h"
#include "constantes.h"

struct despertador{
    int tempo_alarme;
    int periodo;
    int pid;

};
struct relogio{
    int tempo;
    despertador_t *despertador;
};

despertador_t* despertador_cria(){
    despertador_t* d = (despertador_t*)malloc(QNTPROCESSOS * sizeof(despertador_t));
    if(d != NULL){
        for(int i = 0; i < QNTPROCESSOS; i++){
            d[i].tempo_alarme = -1;
            d[i].periodo = 0;
            d[i].pid = -1;
        }
    }
    return d;
}
relogio_t* relogio_cria(){
    relogio_t* r = (relogio_t*)malloc(sizeof(relogio_t));
    despertador_t * d = despertador_cria();
    if(r != NULL){
        r->tempo = 0;
        r->despertador = d;
    }
    return r;
}
int retorna_tempo(relogio_t *relogio){
    return relogio->tempo;
}
void define_alarme_desbloqueio(relogio_t* relogio, int tempo, int pid){
    relogio->despertador[pid].tempo_alarme = relogio->tempo + tempo;

    return;
}
void incrementa_tempo(relogio_t *relogio, int quantidade){
    relogio->tempo += quantidade;
    return;
}
void relogio_libera(relogio_t *relogio){
    free(relogio->despertador);
    free(relogio);
    return;
}
int* verifica_despertadores(relogio_t *relogio){
    int *processos_desbloqueados = (int*)malloc(QNTPROCESSOS * sizeof(int));
    int qnt_desbloqueados = 0;
    for(int i = 0; i < QNTPROCESSOS; i++){
        if(relogio->tempo == relogio->despertador[i].tempo_alarme){
            processos_desbloqueados[qnt_desbloqueados] = i;
            qnt_desbloqueados++;
        }
    }
    if(qnt_desbloqueados < QNTPROCESSOS){
        processos_desbloqueados[qnt_desbloqueados] = -1;
    }
    if(qnt_desbloqueados == 0){
        return NULL;
    }
    return processos_desbloqueados;
}
