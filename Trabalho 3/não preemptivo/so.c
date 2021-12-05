#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "so.h"

//tamanho maximo da memoria dos processo
#define MEMORYSIZE 20

//numero maximo de processos suportados
#define QNTPROCESSOS 10

//processo que vai comecar executando
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
   int tempo_estado_anterior;
   bool incrementa_final;
   FILE *arquivo_escrita;
   FILE *arquivo_leitura;
   err_t erro_execucao;
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
            p[i].tempo_estado_anterior = 0;
            p[i].tempo_bloqueio = 0;
            p[i].tempo_espera = 0;
            p[i].incrementa_final = false;
            p[i].arquivo_escrita = NULL;
            p[i].arquivo_leitura = NULL;
            p[i].erro_execucao = ERR_OK;
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
err_t op_ESCR(cpu_t *cpu, so_t *so, int pid){
    es_t *cpu_es = retorna_es(cpu);
    int cpu_estado_A = retorna_A(cpu);
    err_t escritaResponseES = es_escreve(cpu_es,1,cpu_estado_A, so->processos[pid].arquivo_escrita);
        if(escritaResponseES != ERR_OK){
            cpu_altera_complemento_es(cpu, 1);
            cpu_altera_modo(cpu, escritaResponseES);
            return retorna_modo(cpu);
        }
        incrementa_PC(cpu,2);
        return ERR_OK;
}
err_t op_LE(cpu_t *cpu, so_t *so, int pid){
    int valor;
    es_t *cpu_es = retorna_es(cpu);
    err_t leituraResponseES = es_le(cpu_es,0,&valor, pid);
    if(leituraResponseES != ERR_OK){
        cpu_altera_complemento_es(cpu, 0);
        cpu_altera_modo(cpu, leituraResponseES);
        return retorna_modo(cpu);
    }
    cpu_altera_A(cpu, valor);
    incrementa_PC(cpu,2);
    return ERR_OK;
}
void lista_tempos(so_t *so){
    for(int i = 0; i < QNTPROCESSOS; i++){
        if(so->processos[i].programa != NULL){
            printf("Tempos do processo numero %d:\n\n",i);
            printf("Tempo de execucao = %d \n",so->processos[i].tempo_cpu);
            printf("Tempo pronto = %d \n",so->processos[i].tempo_espera);
            printf("Tempo estado anterior = %d \n",so->processos[i].tempo_estado_anterior);
            printf("Tempo de relogio = %d \n",retorna_tempo(so->relogio));
            if(so->processos[i].erro_execucao != ERR_OK){
                printf("TEVE ERRO NA EXECUCAO -> CODIGO ERRO_CPU = %d", so->processos[i].erro_execucao);
            }
            printf("\n");
            printf("\n");
        }
    }
}
bool escalona(so_t *so, int compensador){
    for(int i = 0; i < QNTPROCESSOS; i++){
        if(so->processos[i].estado == PRONTO && so->processos[i].programa != NULL){
            int tempo_relogio = retorna_tempo(so->relogio) + compensador;
            if(so->processos[pid_executando].estado != MORTO){
                so->processos[pid_executando].estado = PRONTO;
                so->processos[pid_executando].tempo_cpu += tempo_relogio - so->processos[pid_executando].tempo_estado_anterior;
                so->processos[pid_executando].tempo_estado_anterior = tempo_relogio;
            }else{
                if(!so->processos[pid_executando].incrementa_final){
                    so->processos[pid_executando].tempo_cpu += tempo_relogio - so->processos[pid_executando].tempo_estado_anterior;
                    so->processos[pid_executando].tempo_estado_anterior = tempo_relogio;
                    so->processos[pid_executando].incrementa_final = true;
                }
            }

            pid_executando = i;
            so->processos[pid_executando].tempo_espera += tempo_relogio - so->processos[pid_executando].tempo_estado_anterior;
            so->processos[pid_executando].tempo_estado_anterior = tempo_relogio;
            so->processos[i].estado = EXECUCAO;


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

    free(so->processos[pid_executando].cpu_estado);
    cpu_estado_t *estado = cpu_estado_cria_copia(PC,A,X,modo,complemento_memoria,complemento_es);
    so->processos[pid_executando].cpu_estado = estado;

    cpu_estado_t *estado_cpu = cpu_estado_cria_copia(PC,A,X,modo,complemento_memoria,complemento_es);
    cpu_t* c = cpu_cria();
    cpu_altera_estado(c, estado_cpu);
    cpu_altera_es(c, retorna_es(so->cpu));
    cpu_altera_memoria(c, so->processos[pid_executando].memoria);

    return c;
}

void recupera_contexto(so_t *so){
    cpu_libera_estado(so->cpu);
    cpu_estado_t *e = estado_cria_copia(so->processos[pid_executando].cpu_estado);
    cpu_altera_es(so->cpu, retorna_es(so->cpu));
    cpu_altera_estado(so->cpu, e);
    cpu_altera_memoria(so->cpu, so->processos[pid_executando].memoria);
    return;
}
err_t trata_ES(so_t *so, cpu_t *cpu_antes, err_t erro, int pid_executando_antes){
    err_t resposta;
    if(erro == ERR_CPU_PRIV_LE){
        resposta = op_LE(cpu_antes, so, pid_executando_antes);
    }else{
        resposta = op_ESCR(cpu_antes, so, pid_executando_antes);
    }
    cpu_estado_t *c = retorna_estado(cpu_antes);
    cpu_altera_modo(cpu_antes, resposta);
    free(so->processos[pid_executando_antes].cpu_estado);
    so->processos[pid_executando_antes].cpu_estado = c;
    return resposta;

}
bool interrupcao(so_t *so, err_t erro){
    int pid_executando_antes = pid_executando;
    if(erro == ERR_CPU_PRIV_ESCR || erro == ERR_CPU_PRIV_LE){
        cpu_t *cpu_antes = salva_contexto(so);
        escalona(so, 1);
        err_t resposta = trata_ES(so, cpu_antes, erro, pid_executando_antes);
        free(cpu_antes);
        if(resposta != ERR_OK){
            so->processos[pid_executando_antes].estado = MORTO;
            so->processos[pid_executando].erro_execucao = resposta;
        }
        recupera_contexto(so);
        return false;
    }else{
        so->processos[pid_executando].estado = MORTO;
        if(erro != ERR_CPU_PARADA){
            so->processos[pid_executando].erro_execucao = erro;
        }
        fclose(so->processos[pid_executando].arquivo_escrita);
        if(!escalona(so, 1)){
            int tempo_relogio = retorna_tempo(so->relogio) + 1;
            so->processos[pid_executando].tempo_cpu += tempo_relogio - so->processos[pid_executando].tempo_estado_anterior;
            so->processos[pid_executando].tempo_estado_anterior = tempo_relogio;
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
void ajusta_nome_arquivo(int pid, char *vetor){

    char identificador_processo[100];
    sprintf(identificador_processo,"%d", pid);
    strcat(vetor,identificador_processo);

    return;
}
void processo_cria_arquivos(so_t *so, int pid){
    char nome_arquivo_leitura[15] = "leitura";
    ajusta_nome_arquivo(pid, nome_arquivo_leitura);

    char nome_arquivo_escrita[15] = "escrita";
    ajusta_nome_arquivo(pid, nome_arquivo_escrita);

    FILE *arquivo_leitura;
    //valor do teclado para o programa ler depois(pode ser alterado) usei pid + 2 como exemplo
    int valor_leitura = pid+2;
    if ((arquivo_leitura = fopen(strcat(nome_arquivo_leitura,".txt"),"w")) == NULL){
       printf("Problema na criacao do arquivo de leitura do processo pid = %d",pid);
       exit(1);
    }
    fprintf(arquivo_leitura,"%d\n",valor_leitura);
    fclose(arquivo_leitura);


    FILE *arquivo_escrita;
    if ((arquivo_escrita = fopen(strcat(nome_arquivo_escrita,".txt"),"w")) == NULL){
       printf("Problema na criacao do arquivo de escrita do processo pid = %d",pid);
       exit(1);
    }
    so->processos[pid].arquivo_escrita = arquivo_escrita;
    return;

}
void so_inicializa_programa(so_t *so, int *programa, int tam_programa){
    if(so->qnt_programas + 1 > QNTPROCESSOS){
        printf("Aumente a constante que define o numero maximo de processos !");
        exit(1);
    }
    cpu_estado_t* c = cpu_estado_cria();
    so->processos[so->qnt_programas].programa = programa;
    so->processos[so->qnt_programas].cpu_estado = c;

    for (int i = 0; i < tam_programa; i++) {
        err_t resposta = mem_escreve(so->processos[so->qnt_programas].memoria, i, programa[i]);
        if (resposta != ERR_OK) {
            printf("Erro de memoria no processo %d, endereco %d\n",so->qnt_programas, i);
            exit(1);
        }
    }
    processo_cria_arquivos(so, so->qnt_programas);
    so->qnt_programas++;
    return;
}
void processo_libera(processo_t *processos){
    for(int i =0; i < QNTPROCESSOS; i++){
        free(processos[i].cpu_estado);
        mem_destroi(processos[i].memoria);

    }
    free(processos);
}
void so_libera(so_t *so){
    processo_libera(so->processos);
    free(so);
    return;
}
