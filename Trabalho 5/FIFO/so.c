#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "so.h"

//tamanho maximo da memoria dos processo
const int S_MEMORYSIZE = 32;

const int P_MEMORYSIZE = 16;

//tamanho de paginas / quadros
const int tamanho = 2;

const int numero_paginas = S_MEMORYSIZE / tamanho;

const int numero_quadros = P_MEMORYSIZE / tamanho;

//numero maximo de processos suportados
const int QNTPROCESSOS = 50;

//processo que vai comecar executando
int pid_executando = 0;

//tempo para o processo
const int QUANTUM = 0;

struct processo {
   cpu_estado_t *cpu_estado;
   p_state_t estado;
   p_state_t ultimo_estado;
   mem_t *memoria;
   tabpag_t *tabpag;
   int *programa;
   int pid;
   int tempo_cpu;
   int tempo_bloqueio;
   int tempo_espera;
   int tempo_estado_anterior;
   int tempo_leitura;
   int tempo_escrita;
   int numero_vezes_execucao;
   int tamanho_programa;
   int quant_falha_pagina;
   bool instrucao_executando;
   int tempo_criacao;
   FILE *arquivo_escrita;
   FILE *arquivo_leitura;
   err_t erro_execucao;
};

struct quadro {
    bool disponivel;
    int id;
    int idProcesso;
    int idPagina;
    bool alterando;
};
struct so{
    cpu_t* cpu;
    relogio_t* relogio;
    processo_t *processos;
    int *processosIds;
    int *quadrosIds;
    int qnt_programas;
    quadro_t *quadros;
};
int programas[][50] = {
    {2, 0, 7, 2, 49, 5, 16, 21, 1, 9,8, 11, 16, 18, 7, 1, 0,-1},//p0(cria 49 processos com o programa p1)
    {2, 0, 7, 19, 0, 5, 15, 8, 9, 8,11, 15, 18, 7, 1, 0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}//p1 limitado por cpu
    //{2, 0, 7, 2, 10, 5, 17, 8, 20, 1,9, 8, 11, 17, 18, 7, 1, 0,-1, -1, -1, -1, -1, -1,-1,-1,-1,-1,-1,-1,-1,-1},//p2(limitado por E/S)
};
processo_t *processo_cria()
{
    processo_t *p = (processo_t*) malloc (QNTPROCESSOS*sizeof (processo_t));
    if(p != NULL){
        for(int i = 0; i < QNTPROCESSOS; i++){
            mem_t *m = mem_cria(S_MEMORYSIZE);
            tabpag_t *tab = tabpag_cria();
            if(m == NULL){
                printf(" Erro ao criar memoria para o processo, PID: %d",i);
                return NULL;
            }
            if(tab == NULL){
                printf("Erro ao criar tabela de paginas");
                return NULL;
            }
            p[i].cpu_estado = NULL;
            p[i].estado = PRONTO;
            p[i].ultimo_estado = PRONTO;
            p[i].memoria = m;
            p[i].tabpag = tab;
            p[i].programa = NULL;
            p[i].pid = i;
            p[i].tempo_cpu = 0;
            p[i].tempo_estado_anterior = 0;
            p[i].tempo_bloqueio = 0;
            p[i].tempo_espera = 0;
            p[i].tempo_leitura = 10;
            p[i].tempo_escrita = 10;
            p[i].arquivo_escrita = NULL;
            p[i].arquivo_leitura = NULL;
            p[i].erro_execucao = ERR_OK;
            p[i].numero_vezes_execucao = 0;
            p[i].tamanho_programa = 0;
            p[i].quant_falha_pagina = 0;
            p[i].instrucao_executando = false;
            p[i].tempo_criacao = 0;
        }
    }


    return p;
}

so_t* so_cria()
{
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
        int *ids = (int*) malloc(QNTPROCESSOS * sizeof(int));
        for(int i = 0; i < QNTPROCESSOS; i++){
            ids[i] = -1;
        }
        s->processosIds = ids;
        quadro_t *quadros = (quadro_t*) malloc(numero_quadros * sizeof(quadro_t));
        if(quadros == NULL){
            printf("erro ao criar estrutura de quadros");
            return NULL;
        }
        int *quadrosIds = (int*) malloc(numero_quadros * sizeof(int));
        if(quadrosIds == NULL){
            printf("erro ao criar lista de index de quadros");
            return NULL;
        }
        for(int i = 0; i < numero_quadros; i++){
            quadros[i].disponivel = true;
            quadros[i].alterando = false;
            quadros[i].id = i;
            quadros[i].idProcesso = -1;
            quadros[i].idPagina = -1;
            quadrosIds[i] = i;
        }
        s->quadros = quadros;
        s->quadrosIds = quadrosIds;
    }
    return s;
}

void so_altera_cpu(so_t *so, cpu_t* cpu)
{

    so->cpu = cpu;
    return;

}
cpu_t *so_retorna_cpu(so_t *so)
{
    return so->cpu;
}
void so_inicializa_cpu(so_t *so, cpu_t *cpu, mmu_t *mmu)
{
    cpu_estado_t *estado = cpu_estado_cria();
    es_t *es = es_cria();
    cpu_altera_estado(cpu, estado);
    cpu_altera_mmu(cpu, mmu);

    int tamanho_programa0 = calcula_tamanho_programa(0);
    int tamanho_programa1 = calcula_tamanho_programa(1);

    so_inicializa_programa(so, 0, tamanho_programa0);

    mmu_altera_tabela_pagina(mmu, so->processos[0].tabpag);

    so->processos[pid_executando].estado = EXECUCAO;
    so->processos[pid_executando].numero_vezes_execucao++;
    so->processos[pid_executando].ultimo_estado = PRONTO;
    cpu_altera_es(cpu, es);
    return;
}
err_t op_ESCR(cpu_t *cpu, so_t *so, int pid)
{
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
err_t op_LE(cpu_t *cpu, so_t *so, int pid)
{
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
int calcula_tamanho_programa(int idPrograma)
{
    int *vetor = programas[idPrograma];
    int tamanho = 0;
    int i = 0;
    while(vetor[i] != -1){
        tamanho++;
        i++;
    }
    return tamanho;
}
err_t op_CRIA(mmu_t *mmu,  cpu_t *cpu,so_t*so, int PC)
{
    int A1;
    err_t leituraResponseA1 = mmu_le(mmu,PC+1,&A1);
    if(leituraResponseA1 != ERR_OK){
        cpu_altera_complemento_memoria(cpu, PC+1);
        cpu_altera_modo(cpu, leituraResponseA1);
        return retorna_modo(cpu);
    }
    int tamanho_programa = calcula_tamanho_programa(A1);
    so_inicializa_programa(so,A1,tamanho_programa);
    incrementa_PC(cpu,2);
    return ERR_OK;
}
void lista_tempos(so_t *so)
{
    for(int i = 0; i < so->qnt_programas; i++){
        if(so->processos[i].programa != NULL){
            printf("Tempos do processo numero %d:\n\n",i);
            printf("Tempo de execucao = %d \n",so->processos[i].tempo_cpu - so->processos[i].quant_falha_pagina);
            printf("Tempo pronto = %d \n",so->processos[i].tempo_espera);
            printf("Tempo bloqueio = %d \n",so->processos[i].tempo_bloqueio);
            printf("Tempo de retorno = %d \n",so->processos[i].tempo_estado_anterior - so->processos[i].tempo_criacao);
            printf("Tempo de relogio = %d \n",retorna_tempo(so->relogio));
            printf("Tempo de resposta medio = %.2f \n", (float)so->processos[i].tempo_cpu / so->processos[i].numero_vezes_execucao);
            printf("Numero de fal_pag = %d\n",so->processos[i].quant_falha_pagina);
            if(so->processos[i].erro_execucao != ERR_OK){
                printf("TEVE ERRO NA EXECUCAO -> CODIGO ERRO_CPU = %d", so->processos[i].erro_execucao);
            }
            printf("\n");
            printf("\n");
        }
    }
    int numero_fal_pag = 0;
    for(int i = 0; i < so->qnt_programas; i++){
        if(so->processos[i].programa != NULL){
            numero_fal_pag = numero_fal_pag + so->processos[i].quant_falha_pagina;
        }
    }
    printf("Total de falhas de paginas = %d\n",numero_fal_pag);
}
void troca_estado(processo_t *processo, int pid, p_state_t estado_antigo, p_state_t estado_novo, int tempo_relogio)
{
    int tempo_estado_antigo = tempo_relogio - processo[pid].tempo_estado_anterior;
    if(estado_antigo == EXECUCAO){
        processo[pid].tempo_cpu += tempo_estado_antigo;
        processo[pid].numero_vezes_execucao++;
    }else if(estado_antigo == BLOQUEADO){
        processo[pid].tempo_bloqueio += tempo_estado_antigo;
    }else if(estado_antigo == PRONTO){
        processo[pid].tempo_espera += tempo_estado_antigo;
    }
    processo[pid].estado = estado_novo;
    processo[pid].tempo_estado_anterior = tempo_relogio;
    processo[pid].ultimo_estado = estado_antigo;
}
int round_robin(so_t *so)
{
    int quant_bloqueados = 0;
    int quant_prontos = 0;

    for(int i = 0; i < so->qnt_programas; i++){
        if(so->processos[i].estado == PRONTO && so->processos[i].programa != NULL){
            for(int a = 0; a < so->qnt_programas - 1; a++){
                int aux = so->processosIds[a];
                so->processosIds[a] = so->processosIds[a+1];
                so->processosIds[a+1] = aux;

            }
            quant_prontos++;
            return i;
        }else if(so->processos[i].estado == BLOQUEADO && so->processos[i].programa != NULL){
            quant_bloqueados++;
        }
    }
    if(quant_bloqueados > 0 && quant_prontos == 0){
        cpu_altera_modo(so->cpu, ERR_CPU_PARADA_BLOQUEIO);
        return -1;
    }
    return -2;

}
int escalona(so_t *so)
{
    int id_processo = round_robin(so);
    if(id_processo == -1){
        return -1;
    }else if(id_processo == -2){
        return 0;
    }
    int tempo_relogio = retorna_tempo(so->relogio);
    if(pid_executando != -1){
        if(so->processos[pid_executando].estado != MORTO && so->processos[pid_executando].ultimo_estado != BLOQUEADO){
            troca_estado(so->processos,pid_executando,EXECUCAO,PRONTO,tempo_relogio);
        }
    }
    pid_executando = id_processo;
    troca_estado(so->processos, id_processo, PRONTO,EXECUCAO,tempo_relogio);
    mmu_altera_tabela_pagina(retorna_mmu(so->cpu), so->processos[id_processo].tabpag);

    return 1;

}
cpu_t *salva_contexto(so_t *so)
{
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

    return c;
}

void recupera_contexto(so_t *so)
{
    cpu_libera_estado(so->cpu);
    cpu_estado_t *e = estado_cria_copia(so->processos[pid_executando].cpu_estado);
    cpu_altera_es(so->cpu, retorna_es(so->cpu));
    cpu_altera_estado(so->cpu, e);
    return;
}
err_t trata_ES(so_t *so, cpu_t *cpu_antes, err_t erro, int pid_executando_antes)
{
    err_t resposta;
    if(erro == ERR_CPU_PRIV_LE){
        resposta = op_LE(cpu_antes, so, pid_executando_antes);
        define_alarme_desbloqueio(so->relogio, so->processos[pid_executando_antes].tempo_leitura,pid_executando_antes);
    }else{
        resposta = op_ESCR(cpu_antes, so, pid_executando_antes);
        define_alarme_desbloqueio(so->relogio, so->processos[pid_executando_antes].tempo_escrita,pid_executando_antes);
    }
    cpu_estado_t *c = retorna_estado(cpu_antes);
    cpu_altera_modo(cpu_antes, resposta);
    free(so->processos[pid_executando_antes].cpu_estado);
    so->processos[pid_executando_antes].cpu_estado = c;
    return resposta;

}
void ajusta_lista(so_t *so, int inicio)
{
    for(int i = inicio; i < numero_quadros - 1; i++){
        int aux = so->quadrosIds[i];
        so->quadrosIds[i] = so->quadrosIds[i+1];
        so->quadrosIds[i+1] = aux;
    }
    return;
}
void copia(so_t *so, int idQuadro, int idPagina, int pid)
{
    int id_quadro = idQuadro * tamanho;
    for(int i = idPagina * tamanho; i < (idPagina+1)*tamanho;i++ ){
        int valor;
        err_t respostaLeitura = mem_le(so->processos[pid].memoria, i, &valor);
        if (respostaLeitura != ERR_OK) {
            exit(1);
        }
        err_t respostaEscrita = mem_escreve(mmu_retorna_memoria(retorna_mmu(so->cpu)), id_quadro, valor);
        if (respostaEscrita != ERR_OK) {
            exit(1);
        }
        id_quadro++;
    }
    associa(so->processos[pid].tabpag, idPagina, idQuadro);
    so->quadros[idQuadro].idPagina = idPagina;
    so->quadros[idQuadro].idProcesso = pid;
    so->quadros[idQuadro].disponivel = false;
    so->quadros[idQuadro].alterando = true;
}
int escolha_quadro(so_t *so)
{
    int quadro = -1;

    for(int i = 0; i < numero_quadros;i++){
        if(so->quadros[so->quadrosIds[i]].disponivel == true){
            quadro = so->quadrosIds[i];
            ajusta_lista(so, i);
            return quadro;

        }
    }
    if(quadro == -1){
        for(int i = 0; i < numero_quadros;i++){
            if(so->quadros[so->quadrosIds[i]].alterando == false){
                quadro = so->quadrosIds[i];
                ajusta_lista(so, i);
                return quadro;

            }
        }
    }
    return -1;
}
void libera_quadro(so_t *so, int idQuadro)
{
    int idPaginaRelacionada = so->quadros[idQuadro].idPagina;
    int idProcessoRelacionado = so->quadros[idQuadro].idProcesso;
    if(idPaginaRelacionada == -1 || idProcessoRelacionado == -1){
        return;
    }

    int idInicialPagina = idPaginaRelacionada * tamanho;
    if(verifica_alterado(so->processos[idProcessoRelacionado].tabpag, idPaginaRelacionada) == true){
        for(int n = idQuadro * tamanho; n < (idQuadro +1) * tamanho;n++){
            int valor;
            err_t respostaLeitura = mem_le(mmu_retorna_memoria(retorna_mmu(so->cpu)), n, &valor);
            if (respostaLeitura != ERR_OK) {
                exit(1);
            }
            err_t respostaEscrita = mem_escreve(so->processos[idProcessoRelacionado].memoria, idInicialPagina, valor);
            if (respostaEscrita != ERR_OK) {
                exit(1);
            }
            idInicialPagina++;
        }
    }

    reseta_pagina(so->processos[idProcessoRelacionado].tabpag, idPaginaRelacionada);
    so->quadros[idQuadro].idPagina = -1;
    so->quadros[idQuadro].idProcesso = -1;
    so->quadros[idQuadro].disponivel = true;
    so->quadros[idQuadro].alterando = false;
    return;
}
void desvincula_quadros(so_t *so,int pid)
{
    for(int i =0; i < numero_quadros;i++){
        if(so->quadros[i].idProcesso == pid){
            so->quadros[i].idProcesso = -1;
            so->quadros[i].idPagina = -1;
            so->quadros[i].disponivel = true;
            so->quadros[i].alterando = false;
        }
    }
    return;
}
void disponibiliza_quadros(so_t *so, int pid)
{
    for(int i =0; i < numero_quadros;i++){
        if(so->quadros[i].idProcesso == pid){
            so->quadros[i].alterando = false;
        }
    }
    return;
}
void printa_quadro(so_t*so, int idQuadro)
{
    for(int n = idQuadro * tamanho; n < (idQuadro +1) * tamanho;n++){
        int valor;
        err_t respostaLeitura = mem_le(mmu_retorna_memoria(retorna_mmu(so->cpu)), n, &valor);
        if (respostaLeitura != ERR_OK) {
            exit(1);
        }
    }
}
bool interrupcao(so_t *so, err_t erro){
    int pid_executando_antes = pid_executando;
    int tempo_relogio = retorna_tempo(so->relogio);
    if(erro == ERR_CPU_PRIV_ESCR || erro == ERR_CPU_PRIV_LE){
        troca_estado(so->processos, pid_executando_antes, EXECUCAO, BLOQUEADO,tempo_relogio);
        cpu_t *cpu_antes = salva_contexto(so);
        pid_executando = -1;
        escalona(so);
        err_t resposta = trata_ES(so, cpu_antes, erro, pid_executando_antes);
        free(cpu_antes);
        if(resposta != ERR_OK){
            so->processos[pid_executando_antes].estado = MORTO;
            so->processos[pid_executando_antes].ultimo_estado = EXECUCAO;
            so->processos[pid_executando_antes].erro_execucao = resposta;
        }
        if(pid_executando == -1){
            return false;
        }
        recupera_contexto(so);
        return false;
    }
    else if(erro == ERR_CPU_CRIA){
        err_t resposta = op_CRIA(retorna_mmu(so->cpu), so->cpu, so,retorna_PC(so->cpu));
        cpu_altera_modo(so->cpu, resposta);
        return false;
    }else if(erro == ERR_MMU_FAL_PAG){

        int quadro_substituir = escolha_quadro(so);
        if(quadro_substituir == -1){
            return false;
        }
        int endereco_fal = tabela_retorna_fal_pag(so->processos[pid_executando_antes].tabpag);
        int pagina = endereco_fal / tamanho;
        troca_estado(so->processos, pid_executando_antes, EXECUCAO, BLOQUEADO,tempo_relogio);
        define_alarme_desbloqueio(so->relogio, 2, pid_executando_antes);
        so->processos[pid_executando_antes].quant_falha_pagina++;
        cpu_t *cpu_antes = salva_contexto(so);
        pid_executando = -1;
        int resposta_escalona = escalona(so);
        cpu_libera_estado(cpu_antes);
        free(cpu_antes);
        libera_quadro(so, quadro_substituir);
        copia(so, quadro_substituir, pagina, pid_executando_antes);


        if(resposta_escalona == -1){
            return false;
        }
        recupera_contexto(so);
        return false;
    }else{
        if(so->processos[pid_executando].estado == MORTO){
            return false;
        }
        cpu_t *cpu_antes = salva_contexto(so);
        cpu_libera_estado(cpu_antes);
        free(cpu_antes);

        so->processos[pid_executando].estado = MORTO;
        reseta_todas_paginas(so->processos[pid_executando].tabpag);
        desvincula_quadros(so, pid_executando);
        so->processos[pid_executando_antes].ultimo_estado = EXECUCAO;

        tempo_relogio = retorna_tempo(so->relogio);

        so->processos[pid_executando].tempo_cpu += tempo_relogio - so->processos[pid_executando].tempo_estado_anterior;
        so->processos[pid_executando].tempo_estado_anterior = tempo_relogio;

        if(erro != ERR_CPU_PARADA){
            so->processos[pid_executando].erro_execucao = erro;
        }
        fclose(so->processos[pid_executando].arquivo_escrita);

        if(escalona(so) == 0){
            return true;
        }
        recupera_contexto(so);
        return false;
    }
}
void so_altera_relogio(so_t *so, relogio_t* relogio){
    so->relogio = relogio;
    define_alarme_quantum(relogio, QUANTUM);
    return;
}
void trata_interrupcoes_despestadores(so_t *so){
    cpu_t *cpu_antes = salva_contexto(so);
    cpu_libera_estado(cpu_antes);
    free(cpu_antes);
    escalona(so);
    recupera_contexto(so);
    return;
}
void interrompe_para_desbloquear(so_t* so, int pid){
    int tempo_relogio = retorna_tempo(so->relogio);
    troca_estado(so->processos, pid, BLOQUEADO, PRONTO,tempo_relogio);

    if(cpu_estado_retorna_modo(so->processos[pid].cpu_estado) == ERR_MMU_FAL_PAG){
        cpu_estado_altera_modo(so->processos[pid].cpu_estado, ERR_OK);

        if(pid_executando != -1){
            cpu_t *cpu_antes = salva_contexto(so);
            cpu_libera_estado(cpu_antes);
            free(cpu_antes);
            if(so->processos[pid_executando].estado != MORTO && so->processos[pid_executando].ultimo_estado != BLOQUEADO){
                troca_estado(so->processos,pid_executando,EXECUCAO,PRONTO,tempo_relogio);
            }
        }
        pid_executando = pid;
        troca_estado(so->processos, pid, PRONTO,EXECUCAO,tempo_relogio);
        mmu_altera_tabela_pagina(retorna_mmu(so->cpu), so->processos[pid].tabpag);
        recupera_contexto(so);
        return;

    }


    if(pid_executando == -1){
        if(escalona(so) == 1){
            recupera_contexto(so);
        }
        return;
    }
    trata_interrupcoes_despestadores(so);
    return;
}
void interrompe_apos_despertar(so_t *so){
    if(pid_executando == -1){
        if(escalona(so) == 1){
            recupera_contexto(so);
        }
        return;
    }
    trata_interrupcoes_despestadores(so);
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
void so_inicializa_programa(so_t *so, int idPrograma, int tam_programa){
    if(so->qnt_programas + 1 > QNTPROCESSOS){
        printf("Aumente a constante que define o numero maximo de processos !");
        exit(1);
    }
    cpu_estado_t* c = cpu_estado_cria();
    so->processos[so->qnt_programas].programa = programas[idPrograma];
    so->processos[so->qnt_programas].tamanho_programa = tam_programa;
    so->processos[so->qnt_programas].cpu_estado = c;
    so->processos[so->qnt_programas].tempo_criacao = retorna_tempo(so->relogio);

    so->processosIds[so->qnt_programas] = so->qnt_programas;

    for (int i = 0; i < tam_programa; i++) {
        err_t resposta = mem_escreve(so->processos[so->qnt_programas].memoria, i, so->processos[so->qnt_programas].programa[i]);
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
        if(processos[i].programa != NULL){
            free(processos[i].cpu_estado);
        }
        mem_destroi(processos[i].memoria);
        libera_tabela(processos[i].tabpag);

    }
    free(processos);
}
void so_libera(so_t *so){
    processo_libera(so->processos);
    free(so->processosIds);
    free(so->quadrosIds);
    free(so->quadros);
    free(so);
    return;
}
cpu_estado_t *retorna_processo_estado(so_t *so, int pid)
{
    return so->processos[pid].cpu_estado;
}
int retorna_pid_executando(so_t *so)
{
    return pid_executando;
}
void altera_processo_status_executando(so_t* so, int pid, bool valor)
{
    if(pid == -1){
        return;
    }
    so->processos[pid].instrucao_executando = valor;
    return;
}
