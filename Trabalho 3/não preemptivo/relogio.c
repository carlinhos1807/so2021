#include <stdlib.h>
#include <stdio.h>
#include "relogio.h"

struct relogio{
    int tempo;
};

relogio_t* relogio_cria(){
    relogio_t* r = (relogio_t*)malloc(sizeof(relogio_t));
    if(r != NULL){
        r->tempo = 0;
    }
    return r;
}
int retorna_tempo(relogio_t *relogio){
    return relogio->tempo;
}
void incrementa_tempo(relogio_t *relogio, int quantidade){
    relogio->tempo += quantidade;
    return;
}
void relogio_libera(relogio_t *relogio){
    free(relogio);
    return;
}
