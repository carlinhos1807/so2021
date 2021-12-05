#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "err.h"
#include "memoria.h"
#include "es.h"



typedef struct cpu cpu_t;
typedef struct cpu_estado cpu_estado_t;

cpu_t* cpu_cria();
cpu_estado_t* cpu_estado_cria();
es_t *retorna_es(cpu_t *cpu);
mem_t* retorna_memoria(cpu_t *cpu);
cpu_estado_t *cpu_estado(cpu_t *cpu);
cpu_estado_t *retorna_estado(cpu_t *cpu);
cpu_estado_t * cpu_estado_cria_copia(int PC, int A, int X, err_t modo, int complemento_memoria, int complemento_es);
cpu_estado_t * estado_cria_copia(cpu_estado_t *estado);

int retorna_A(cpu_t *cpu);
int retorna_PC(cpu_t *cpu);
int retorna_complemento_es(cpu_t * cpu);
int retorna_complemento_memoria(cpu_t * cpu);
int retorna_X(cpu_t * cpu);

void cpu_altera_estado(cpu_t *cpu, cpu_estado_t *estado);
void cpu_altera_memoria(cpu_t *cpu, mem_t *mem);
void cpu_altera_es(cpu_t *cpu, es_t *es);
void cpu_altera_modo(cpu_t *cpu, err_t modo);
void cpu_libera(cpu_t *cpu);
void cpu_libera_estado(cpu_t *cpu);
void cpu_imprime_estado(cpu_t *cpu);
void incrementa_PC(cpu_t *cpu, int valor);
void cpu_altera_A(cpu_t *cpu, int valor);
void cpu_altera_complemento_es(cpu_t *cpu, int valor);

err_t cpu_executa_1(cpu_t *cpu);
err_t retorna_modo(cpu_t *cpu);
err_t op_NOP(cpu_t *cpu);
err_t op_PARA(cpu_t *cpu);
err_t op_CARGI(cpu_t *cpu, int PC);
err_t op_CARGM(cpu_t *cpu, int PC);
err_t op_CARGX(cpu_t *cpu, int PC);
err_t op_ARMM(cpu_t *cpu, int PC);
err_t op_ARMX(cpu_t *cpu, int PC);
err_t op_MVAX(cpu_t *cpu);
err_t op_MVXA(cpu_t *cpu);
err_t op_INCX(cpu_t *cpu);
err_t op_SOMA(cpu_t *cpu, int PC);
err_t op_SUB(cpu_t *cpu, int PC);
err_t op_MULT(cpu_t *cpu, int PC);
err_t op_DIV(cpu_t *cpu, int PC);
err_t op_RESTO(cpu_t *cpu, int PC);
err_t op_NEG(cpu_t *cpu);
err_t op_DESV(cpu_t *cpu, int PC);
err_t op_DESVZ(cpu_t *cpu, int PC);
err_t op_DESVNZ(cpu_t *cpu, int PC);
err_t op_invalido(cpu_t *cpu);

#endif // CPU_H_INCLUDED
