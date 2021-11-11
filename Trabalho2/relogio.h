#ifndef RELOGIO_H_INCLUDED
#define RELOGIO_H_INCLUDED

typedef struct relogio relogio_t;

relogio_t* relogio_cria();
void relogio_libera(relogio_t *relogio);
void incrementa_tempo(relogio_t *relogio, int quantidade);
int retorna_tempo(relogio_t *relogio);
#endif // RELOGIO_H_INCLUDED
