#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "relogio.h"

struct despertador{
    int tempo_alarme;
    int periodo;

};
struct relogio{
    int tempo;
    despertador_t *despertador;
};

despertador_t* despertador_cria(){
    despertador_t* d = (despertador_t*)malloc(sizeof(despertador_t));
    if(d != NULL){
        d->tempo_alarme = -1;
        d->periodo = 0;
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
void define_alarme_periodico(relogio_t* relogio, int periodo){
    relogio->despertador->tempo_alarme = periodo;
    relogio->despertador->periodo = periodo;
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
bool verifica_despertador(relogio_t *relogio){
    if(relogio->tempo == relogio->despertador->tempo_alarme){
        relogio->despertador->tempo_alarme += relogio->despertador->periodo;
        return true;
    }
    return false;
}
