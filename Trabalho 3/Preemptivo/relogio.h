#ifndef RELOGIO_H_INCLUDED
#define RELOGIO_H_INCLUDED

typedef struct relogio relogio_t;
typedef struct despertador despertador_t;

relogio_t* relogio_cria();
void relogio_libera(relogio_t *relogio);
void incrementa_tempo(relogio_t *relogio, int quantidade);
void define_alarme_periodico(relogio_t* relogio, int periodo);
bool verifica_despertador(relogio_t *relogio);
int retorna_tempo(relogio_t *relogio);
#endif // RELOGIO_H_INCLUDED
