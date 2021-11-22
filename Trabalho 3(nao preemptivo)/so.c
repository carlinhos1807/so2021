#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "so.h"

#define MEMORYSIZE 20

#define QNTPROCESSOS 4

int pid_executando = 0;
struct processo {
   cpu_estado_t *cpu_estado;
   p_state_t estado;
   mem_t *memoria;
   int *programa;
   int pid;
   int tempo_cpu;
   int tempo_bloqueio;
   int tempo_espera;
};
struct so{
    cpu_t* cpu;
    relogio_t* relogio;
    processo_t *processos;
    int qnt_programas;
};

processo_t *processo_cria(){
    processo_t *p = (processo_t*) malloc (QNTPROCESSOS*sizeof (processo_t));
    if(p != NULL){
        for(int i = 0; i < QNTPROCESSOS; i++){
            mem_t *m = mem_cria(MEMORYSIZE);
            if(m == NULL){
                printf(" Erro ao criar memoria para o processo, PID: %d",i);
                return NULL;
            }
            p[i].cpu_estado = NULL;
            p[i].estado = PRONTO;
            p[i].memoria = m;
            p[i].programa = NULL;
            p[i].pid = i;
            p[i].tempo_cpu = 0;
            p[i].tempo_bloqueio = 0;
            p[i].tempo_espera = 0;
        }
    }


    return p;
}

so_t* so_cria(){
    so_t* s = (so_t*)malloc(sizeof(so_t));
    if(s != NULL){
        processo_t *processos = processo_cria();
        if(processos == NULL){
            printf("erro ao iniciar processos");
            return NULL;
        }
        s->cpu = NULL;
        s->relogio = NULL;
        s->processos = processos;
        s->qnt_programas = 0;
    }
    return s;
}

void so_altera_cpu(so_t *so, cpu_t* cpu){

    so->cpu = cpu;
    return;

}
cpu_t *so_retorna_cpu(so_t *so){
    return so->cpu;
}
void so_inicializa_cpu(so_t *so, cpu_t *cpu){
    cpu_estado_t *estado = cpu_estado_cria();
    es_t *es = es_cria();
    cpu_altera_estado(cpu, estado);
    cpu_altera_memoria(cpu, so->processos[pid_executando].memoria);
    so->processos[pid_executando].estado = EXECUCAO;
    cpu_altera_es(cpu, es);
    return;
}
err_t op_ESCR(cpu_t *cpu){
    es_t *cpu_es = retorna_es(cpu);
    int cpu_estado_A = retorna_A(cpu);
    err_t escritaResponseES = es_escreve(cpu_es,1,cpu_estado_A);
        if(escritaResponseES != ERR_OK){
            cpu_altera_complemento_es(cpu, 1);
            cpu_altera_modo(cpu, escritaResponseES);
            return retorna_modo(cpu);
        }
        incrementa_PC(cpu,2);
        return retorna_modo(cpu);
}
err_t op_LE(cpu_t *cpu){
    int valor;
    es_t *cpu_es = retorna_es(cpu);
    err_t leituraResponseES = es_le(cpu_es,0,&valor);
    if(leituraResponseES != ERR_OK){
        cpu_altera_complemento_es(cpu, 0);
        cpu_altera_modo(cpu, leituraResponseES);
        return retorna_modo(cpu);
    }
    cpu_altera_A(cpu, valor);
    incrementa_PC(cpu,2);
    return retorna_modo(cpu);
}
bool escalona_nao_preemptivo(so_t *so){
    for(int i = 0; i < QNTPROCESSOS; i++){
        if(so->processos[i].estado == PRONTO){
            if(so->processos[pid_executando].estado != MORTO){
                so->processos[pid_executando].estado = PRONTO;
            }
            printf("Tava executando = %d", pid_executando);
            pid_executando = i;

            so->processos[i].estado = EXECUCAO;

            printf("agr ta executando = %d", pid_executando);
            return true;
        }
    }
    return false;
}

cpu_t *salva_contexto(so_t *so){
    int PC = retorna_PC(so->cpu);
    int A = retorna_A(so->cpu);
    int X = retorna_X(so->cpu);
    err_t modo = retorna_modo(so->cpu);
    int complemento_memoria = retorna_complemento_memoria(so->cpu);
    int complemento_es = retorna_complemento_es(so->cpu);

    cpu_estado_t *estado = cpu_estado_cria_copia(PC,A,X,modo,complemento_memoria,complemento_es);
    so->processos[pid_executando].cpu_estado = estado;

    cpu_t* c = cpu_cria();
    cpu_altera_estado(c, estado);
    cpu_altera_es(c, retorna_es(so->cpu));
    cpu_altera_memoria(c, so->processos[pid_executando].memoria);

    return c;
}

void recupera_contexto(so_t *so){
    cpu_t* c = cpu_cria();
    cpu_altera_es(c, retorna_es(so->cpu));
    cpu_altera_estado(c, so->processos[pid_executando].cpu_estado);
    cpu_altera_memoria(c, so->processos[pid_executando].memoria);
    so_altera_cpu(so, c);

    return;
}
void trata_ES(so_t *so, cpu_t *cpu_antes, err_t erro, int pid_executando_antes){
    if(erro == ERR_CPU_PRIV_LE){
        op_LE(cpu_antes);
    }else{
        op_ESCR(cpu_antes);
    }
    so->processos[pid_executando_antes].cpu_estado = retorna_estado(cpu_antes);

}
bool interrupcao(so_t *so, err_t erro){
    printf("PC do prog 1 = %d\n", retorna_PC2(so->processos[0].cpu_estado));
    printf("PC do prog 1 = %d", retorna_PC2(so->processos[1].cpu_estado));
    int pid_executando_antes = pid_executando;
    if(erro == ERR_CPU_PRIV_ESCR || erro == ERR_CPU_PRIV_LE){
            cpu_t *cpu_antes = salva_contexto(so);

        escalona_nao_preemptivo(so);
        trata_ES(so, cpu_antes, erro, pid_executando_antes);
        recupera_contexto(so);
        return false;
    }
    if(erro == ERR_CPU_PARADA){
        salva_contexto(so);
        so->processos[pid_executando].estado = MORTO;
        if(!escalona_nao_preemptivo(so)){
            int tempo = retorna_tempo(so->relogio);
            printf("\nErro na execucao: %d\n", erro);
            printf("\ntempo transcorrido: %d\n",tempo+1);
            return true;
        }
        recupera_contexto(so);
        return false;
    }
}
void so_altera_relogio(so_t *so, relogio_t* relogio){
    so->relogio = relogio;
    return;
}
void so_inicializa_programa(so_t *so, int *programa){
    cpu_estado_t* c = cpu_estado_cria();
    so->processos[so->qnt_programas].programa = programa;
    so->processos[so->qnt_programas].cpu_estado = c;

    for (int i = 0; i < MEMORYSIZE; i++) {
     if (mem_escreve(so->processos[so->qnt_programas].memoria, i, programa[i]) != ERR_OK) {
        printf("Erro de memoria, endereco %d\n", i);
        exit(1);
     }
    }
    so->qnt_programas++;
    return;
}
void so_libera(so_t *so){
    free(so->processos);
    free(so);
    return;
}
