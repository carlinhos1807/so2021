#ifndef UC_H_INCLUDED
#define UC_H_INCLUDED

#include "cpu.h"
#include "so.h"
#include "relogio.h"
#include "err.h"

typedef struct uc uc_t;

uc_t* uc_cria();
void executa(uc_t *uc);
err_t controla(cpu_t *cpu);
void uc_altera_cpu(uc_t *uc, cpu_t* cpu);
void uc_altera_relogio(uc_t *uc, relogio_t* relogio);
void uc_altera_so(uc_t *uc, so_t *so);
void uc_libera(uc_t *uc);

#endif // UC_H_INCLUDED
