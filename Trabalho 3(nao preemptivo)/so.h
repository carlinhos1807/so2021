#ifndef SO_H_INCLUDED
#define SO_H_INCLUDED


#include "process_state.h"
#include "cpu.h"
#include "relogio.h"


typedef struct so so_t;
typedef struct processo processo_t;

so_t* so_cria();
void so_altera_cpu(so_t *so, cpu_t* cpu);
void so_altera_relogio(so_t *so, relogio_t* relogio);
void so_libera(so_t *so);
void so_inicializa_cpu(so_t *so, cpu_t *cpu);
bool interrupcao(so_t *so,err_t erro);
processo_t *processo_cria();
void recupera_contexto(so_t *so);
#endif // SO_H_INCLUDED
