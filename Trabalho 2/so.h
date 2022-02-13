#ifndef SO_H_INCLUDED
#define SO_H_INCLUDED

#include "cpu.h"
#include "relogio.h"

typedef struct so so_t;

so_t* so_cria();
void so_altera_cpu(so_t *so, cpu_t* cpu);
void so_altera_relogio(so_t *so, relogio_t* relogio);
void so_altera_programa(so_t *so, int *programa);
void so_libera(so_t *so);
void interrupcao(so_t *so, err_t erro);
#endif // SO_H_INCLUDED
