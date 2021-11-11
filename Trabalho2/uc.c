#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "uc.h"


typedef enum {
  NOP = 0,    PARA,   CARGI,  CARGM,  CARGX,  ARMM,   ARMX,   MVAX,
  MVXA,   INCX,   SOMA,   SUB,    MULT,   DIV,    RESTO,  NEG,
  DESV,   DESVZ,  DESVNZ, LE,     ESCR,   VALOR,  ESPACO, DEFINE,
} codigo_t;

struct uc{
    cpu_t *cpu;
    so_t *so;
    relogio_t *relogio;
};

uc_t* uc_cria(){
    uc_t* u = (uc_t*)malloc(sizeof(uc_t));
    if(u != NULL){
        u->cpu = NULL;
        u->so = NULL;
        u->relogio = NULL;
    }
    return u;
}
void executa(uc_t *uc){
    while (true) {
    cpu_imprime_estado(uc->cpu);
    err_t err = controla(uc->cpu);
    if (err != ERR_OK) {
        interrupcao(uc->so, err);
        printf("\nEstado final:\n");
        cpu_imprime_estado(uc->cpu);
        break;
    }
    incrementa_tempo(uc->relogio,1);
  }
}
err_t controla(cpu_t *cpu){
    err_t cpu_modo = retorna_modo(cpu);
    if(retorna_modo(cpu) != ERR_OK){
        return cpu_modo;
    }
    int PC = retorna_PC(cpu);
    int instrucao;
    err_t leituraResponse = mem_le(retorna_memoria(cpu),PC,&instrucao);
    if(leituraResponse != ERR_OK){
        cpu_altera_modo(cpu, leituraResponse);
        return leituraResponse;
    }

    if(instrucao == NOP){
        err_t resposta = op_NOP(cpu);
        return resposta;
    }else if(instrucao == PARA){
        err_t resposta = op_PARA(cpu);
        return resposta;
    }else if(instrucao == CARGI){
        err_t resposta = op_CARGI(cpu, PC);
        return resposta;
    }else if(instrucao == CARGM){
        err_t resposta = op_CARGM(cpu, PC);
        return resposta;
    }else if(instrucao == CARGX){
        err_t resposta = op_CARGX(cpu, PC);
        return resposta;
    }else if(instrucao == ARMM){
        err_t resposta = op_ARMM(cpu, PC);
        return resposta;
    }else if(instrucao == ARMX){
        err_t resposta = op_ARMX(cpu, PC);
        return resposta;
    }else if(instrucao == MVAX){
        err_t resposta = op_MVAX(cpu);
        return resposta;
    }else if(instrucao == MVXA){
        err_t resposta = op_MVXA(cpu);
        return resposta;
    }else if(instrucao == INCX){
        err_t resposta = op_INCX(cpu);
        return resposta;
    }else if(instrucao == SOMA){
        err_t resposta = op_SOMA(cpu, PC);
        return resposta;
    }else if(instrucao == SUB){
        err_t resposta = op_SUB(cpu, PC);
        return resposta;
    }else if(instrucao == MULT){
        err_t resposta = op_MULT(cpu, PC);
        return resposta;
    }else if(instrucao == DIV){
        err_t resposta = op_DIV(cpu, PC);
        return resposta;
    }else if(instrucao == RESTO){
        err_t resposta = op_RESTO(cpu, PC);
        return resposta;
    }else if(instrucao == NEG){
        err_t resposta = op_NEG(cpu);
        return resposta;
    }else if(instrucao == DESV){
        err_t resposta = op_DESV(cpu, PC);
        return resposta;
    }else if(instrucao == DESVZ){
        err_t resposta = op_DESVZ(cpu, PC);
        return resposta;
    }else if(instrucao == DESVNZ){
        err_t resposta = op_DESVNZ(cpu, PC);
        return resposta;
    }else if(instrucao == LE){
        err_t resposta = op_LE(cpu);
        return resposta;
    }else if(instrucao == ESCR){
        err_t resposta = op_ESCR(cpu);
        return resposta;
    }else{
        err_t resposta = op_invalido(cpu);
        return resposta;
    }
}
void uc_altera_cpu(uc_t *uc, cpu_t* cpu){
    uc->cpu = cpu;
    return;
}
void uc_altera_relogio(uc_t *uc, relogio_t* relogio){
    uc->relogio = relogio;
    return;
}
void uc_altera_so(uc_t *uc, so_t *so){
    uc->so = so;
    return;
}
void uc_libera(uc_t *uc){
    free(uc);
    return;
}
