#ifndef SO_H_INCLUDED
#define SO_H_INCLUDED


#include "process_state.h"
#include "cpu.h"
#include "relogio.h"


typedef struct so so_t;
typedef struct processo processo_t;
typedef struct quadro quadro_t;

so_t* so_cria();
cpu_t *so_retorna_cpu(so_t *so);
void so_altera_cpu(so_t *so, cpu_t* cpu);
void so_altera_relogio(so_t *so, relogio_t* relogio);
void so_libera(so_t *so);
void so_inicializa_programa(so_t *so, int idPrograma, int tam_programa);
void lista_tempos(so_t *so);
void so_inicializa_cpu(so_t *so, cpu_t *cpu, mmu_t *mmu);
tabpag_t *tabpag_cria();
bool interrupcao(so_t *so,err_t erro);
processo_t *processo_cria();
void recupera_contexto(so_t *so);
int calcula_tamanho_programa(int idPrograma);
void interrompe_para_desbloquear(so_t* so, int pid);
void interrompe_apos_despertar(so_t *so);
err_t op_ESCR(cpu_t *cpu, so_t *so, int pid);
err_t op_LE(cpu_t *cpu, so_t *so, int pid);
cpu_estado_t *retorna_processo_estado(so_t *so, int pid);
int retorna_pid_executando(so_t *so);
void altera_processo_status_executando(so_t* so, int pid, bool valor);
void disponibiliza_quadros(so_t *so, int pid);
#endif // SO_H_INCLUDED
