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
    despertador_t* d = (despertador_t*)malloc((QNTPROCESSOS + 1) * sizeof(despertador_t));
    if(d != NULL){
        for(int i = 0; i < QNTPROCESSOS + 1; i++){
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
void desfaz_despertador(relogio_t* relogio, int pid){
    relogio->despertador[pid].tempo_alarme = 0;
    return;
}
void define_alarme_escalonamento(relogio_t* relogio, int periodo){
    relogio->despertador[QNTPROCESSOS].tempo_alarme = periodo;
    relogio->despertador[QNTPROCESSOS].periodo = periodo;

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
bool verifica_despertador_escalonamento(relogio_t *relogio){
    if(relogio->tempo == relogio->despertador[QNTPROCESSOS].tempo_alarme){
        relogio->despertador[QNTPROCESSOS].tempo_alarme += relogio->despertador[QNTPROCESSOS].periodo;
        return true;
    }
    return false;
}
int* verifica_despertadores_desbloqueio(relogio_t *relogio){
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
        free(processos_desbloqueados);
        return NULL;
    }
    return processos_desbloqueados;
}
