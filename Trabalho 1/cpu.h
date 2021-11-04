#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "err.h"
#include "memoria.h"
#include "es.h"


typedef struct cpu cpu_t;
typedef struct cpu_estado cpu_estado_t;

cpu_estado_t *cpu_estado(cpu_t *cpu);
void cpu_altera_estado(cpu_t *cpu, cpu_estado_t *estado);
void cpu_altera_memoria(cpu_t *cpu, mem_t *mem);
void cpu_altera_es(cpu_t *cpu, es_t *es);
err_t cpu_executa_1(cpu_t *cpu);
cpu_t* cpu_cria();
cpu_estado_t* cpu_estado_cria();
void cpu_libera(cpu_t *cpu);
void cpu_imprime_estado(cpu_t *cpu);

#endif // CPU_H_INCLUDED
