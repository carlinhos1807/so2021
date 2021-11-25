#ifndef SO_H_INCLUDED
#define SO_H_INCLUDED


#include "process_state.h"
#include "cpu.h"
#include "relogio.h"


typedef struct so so_t;
typedef struct processo processo_t;

so_t* so_cria();
void so_altera_cpu(so_t *so, cpu_t* cpu);
void ajusta_nome_arquivo(int pid, char *vetor);
void so_altera_relogio(so_t *so, relogio_t* relogio);
void so_libera(so_t *so);
void so_inicializa_cpu(so_t *so, cpu_t *cpu);
void ajusta_nome_arquivo(int pid, char *vetor);
void so_inicializa_programa(so_t *so, int *programa, int tam_programa);
void interrompe_apos_despertar(so_t *so);
void lista_tempos(so_t *so);
void processo_libera(processo_t *processos);
cpu_t *so_retorna_cpu(so_t *so);
bool interrupcao(so_t *so,err_t erro);
processo_t *processo_cria();
void recupera_contexto(so_t *so);
err_t op_LE(cpu_t *cpu, so_t *so, int pid);
err_t op_ESCR(cpu_t *cpu, so_t *so, int pid);
err_t trata_ES(so_t *so, cpu_t *cpu_antes, err_t erro, int pid_executando_antes);
#endif // SO_H_INCLUDED
