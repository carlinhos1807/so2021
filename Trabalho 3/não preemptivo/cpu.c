#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

struct cpu_estado{
    int PC;
    int A;
    int X;
    err_t modo;
    int complemento_memoria;
    int complemento_es;
};
struct cpu {
   cpu_estado_t* estado;
   es_t* es;
   mem_t* memoria;
};

cpu_t* cpu_cria(){

    cpu_t* c = (cpu_t*)malloc(sizeof(cpu_t));
    if(c != NULL){
        c->estado = NULL;
        c->es = NULL;
        c->memoria = NULL;
    }
    return c;
}
void cpu_imprime_estado(cpu_t *cpu){
    printf("\n");
    printf("PC = %d\n", cpu->estado->PC);
    printf("A = %d\n", cpu->estado->A);
    printf("X = %d\n", cpu->estado->X);
    if(cpu->estado->complemento_memoria != -1){
        printf("complemento_memoria = %d\n", cpu->estado->complemento_memoria);
    }
    if(cpu->estado->complemento_es != -1){
        printf("complemento_es = %d\n", cpu->estado->complemento_es);
    }
    //printf("modo = %d\n",cpu->estado->modo);
    printf("\n");
    return;
}
cpu_estado_t *cpu_estado(cpu_t *cpu){
    return cpu->estado;
}
err_t op_NOP(cpu_t *cpu){
    cpu->estado->PC+=1;
    return cpu->estado->modo;
}
err_t op_PARA(cpu_t *cpu){
    cpu->estado->modo = ERR_CPU_PARADA;
    return cpu->estado->modo;
}
err_t op_CARGI(cpu_t *cpu, int PC){
    int A1;
    err_t leituraResponse = mem_le(cpu->memoria,PC+1,&A1);
    if(leituraResponse != ERR_OK){
        cpu->estado->complemento_memoria = PC+1;
        cpu->estado->modo = leituraResponse;
        return cpu->estado->modo;
    }
    cpu->estado->A = A1;
    cpu->estado->PC+=2;
    return cpu->estado->modo;
}
err_t op_CARGM(cpu_t *cpu, int PC){
    int A1;
    err_t leituraResponseA1 = mem_le(cpu->memoria,PC+1,&A1);
    if(leituraResponseA1 != ERR_OK){
        cpu->estado->complemento_memoria = PC+1;
        cpu->estado->modo = leituraResponseA1;
        return cpu->estado->modo;
    }
    int memA1;
    err_t leituraResponseMemA1 = mem_le(cpu->memoria,A1,&memA1);
    if(leituraResponseMemA1 != ERR_OK){
        cpu->estado->complemento_memoria = A1;
        cpu->estado->modo = leituraResponseMemA1;
        return cpu->estado->modo;
    }
    cpu->estado->A = memA1;
    cpu->estado->PC+=2;
    return cpu->estado->modo;
}
err_t op_CARGX(cpu_t *cpu, int PC){
    int A1;
    err_t leituraResponseA1 = mem_le(cpu->memoria,PC+1,&A1);
    if(leituraResponseA1 != ERR_OK){
        cpu->estado->complemento_memoria = PC+1;
        cpu->estado->modo = leituraResponseA1;
        return cpu->estado->modo;
    }

    int soma = A1 + cpu->estado->X;
    int memIndexado;
    err_t leituraResponse = mem_le(cpu->memoria,soma,&memIndexado);
    if(leituraResponse != ERR_OK){
        cpu->estado->complemento_memoria = soma;
        cpu->estado->modo = leituraResponse;
        return cpu->estado->modo;
    }
    cpu->estado->A = memIndexado;
    cpu->estado->PC+=2;
    return cpu->estado->modo;
}
err_t op_ARMM(cpu_t *cpu, int PC){
    int A1;
    err_t leituraResponseA1 = mem_le(cpu->memoria,PC+1,&A1);
    if(leituraResponseA1 != ERR_OK){
        cpu->estado->complemento_memoria = PC+1;
        cpu->estado->modo = leituraResponseA1;
        return cpu->estado->modo;
    }
    err_t escritaResponseA1 = mem_escreve(cpu->memoria,A1,cpu->estado->A);
    if(escritaResponseA1 != ERR_OK){
        cpu->estado->complemento_memoria = A1;
        cpu->estado->modo = escritaResponseA1;
        return cpu->estado->modo;
    }
    cpu->estado->PC+=2;
    return cpu->estado->modo;
}
err_t op_ARMX(cpu_t *cpu, int PC){
    int A1;
    err_t leituraResponseA1 = mem_le(cpu->memoria,PC+1,&A1);
    if(leituraResponseA1 != ERR_OK){
        cpu->estado->complemento_memoria = PC+1;
        cpu->estado->modo = leituraResponseA1;
        return cpu->estado->modo;
    }

    int soma = A1 + cpu->estado->X;
    err_t escritaResponseA1X = mem_escreve(cpu->memoria,soma,cpu->estado->A);
    if(escritaResponseA1X != ERR_OK){
        cpu->estado->complemento_memoria = soma;
        cpu->estado->modo = escritaResponseA1X;
        return cpu->estado->modo;
    }
    cpu->estado->PC+=2;
    return cpu->estado->modo;
}
err_t op_MVAX(cpu_t *cpu){
    cpu->estado->X = cpu->estado->A;
    cpu->estado->PC+=1;
    return cpu->estado->modo;
}
err_t op_MVXA(cpu_t *cpu){
    cpu->estado->A = cpu->estado->X;
    cpu->estado->PC+=1;
    return cpu->estado->modo;
}
err_t op_INCX(cpu_t *cpu){
    cpu->estado->X+=1;
    cpu->estado->PC+=1;
    return cpu->estado->modo;
}
err_t op_SOMA(cpu_t *cpu, int PC){
    int A1;
    err_t leituraResponseA1 = mem_le(cpu->memoria,PC+1,&A1);
    if(leituraResponseA1 != ERR_OK){
        cpu->estado->complemento_memoria = PC+1;
        cpu->estado->modo = leituraResponseA1;
        return cpu->estado->modo;
    }

    int memA1;
    err_t leituraResponseMemA1 = mem_le(cpu->memoria,A1,&memA1);
    if(leituraResponseMemA1 != ERR_OK){
        cpu->estado->complemento_memoria = A1;
        cpu->estado->modo = leituraResponseMemA1;
        return cpu->estado->modo;
    }

    cpu->estado->A+=memA1;
    cpu->estado->PC+=2;
    return cpu->estado->modo;
}
err_t op_SUB(cpu_t *cpu, int PC){
    int A1;
    err_t leituraResponseA1 = mem_le(cpu->memoria,PC+1,&A1);
    if(leituraResponseA1 != ERR_OK){
        cpu->estado->complemento_memoria = PC+1;
        cpu->estado->modo = leituraResponseA1;
        return cpu->estado->modo;
    }

    int memA1;
    err_t leituraResponseMemA1 = mem_le(cpu->memoria,A1,&memA1);
    if(leituraResponseMemA1 != ERR_OK){
        cpu->estado->complemento_memoria = A1;
        cpu->estado->modo = leituraResponseMemA1;
        return cpu->estado->modo;
    }

    cpu->estado->A-=memA1;
    cpu->estado->PC+=2;
    return cpu->estado->modo;
}
err_t op_MULT(cpu_t *cpu, int PC){
    int A1;
    err_t leituraResponseA1 = mem_le(cpu->memoria,PC+1,&A1);
    if(leituraResponseA1 != ERR_OK){
        cpu->estado->complemento_memoria = PC+1;
        cpu->estado->modo = leituraResponseA1;
        return cpu->estado->modo;
    }

    int memA1;
    err_t leituraResponseMemA1 = mem_le(cpu->memoria,A1,&memA1);
    if(leituraResponseMemA1 != ERR_OK){
        cpu->estado->complemento_memoria = A1;
        cpu->estado->modo = leituraResponseMemA1;
        return cpu->estado->modo;
    }

    cpu->estado->A*=memA1;
    cpu->estado->PC+=2;
    return cpu->estado->modo;
}
err_t op_DIV(cpu_t *cpu, int PC){
    int A1;
    err_t leituraResponseA1 = mem_le(cpu->memoria,PC+1,&A1);
    if(leituraResponseA1 != ERR_OK){
        cpu->estado->complemento_memoria = PC+1;
        cpu->estado->modo = leituraResponseA1;
        return cpu->estado->modo;
    }

    int memA1;
    err_t leituraResponseMemA1 = mem_le(cpu->memoria,A1,&memA1);
    if(leituraResponseMemA1 != ERR_OK){
        cpu->estado->complemento_memoria = A1;
        cpu->estado->modo = leituraResponseMemA1;
        return cpu->estado->modo;
    }

    cpu->estado->A/=memA1;
    cpu->estado->PC+=2;
    return cpu->estado->modo;
}
err_t op_RESTO(cpu_t *cpu, int PC){
    int A1;
    err_t leituraResponseA1 = mem_le(cpu->memoria,PC+1,&A1);
    if(leituraResponseA1 != ERR_OK){
        cpu->estado->complemento_memoria = PC+1;
        cpu->estado->modo = leituraResponseA1;
        return cpu->estado->modo;
    }

    int memA1;
    err_t leituraResponseMemA1 = mem_le(cpu->memoria,A1,&memA1);
    if(leituraResponseMemA1 != ERR_OK){
        cpu->estado->complemento_memoria = A1;
        cpu->estado->modo = leituraResponseMemA1;
        return cpu->estado->modo;
    }

    cpu->estado->A%=memA1;
    cpu->estado->PC+=2;
    return cpu->estado->modo;
}
err_t op_NEG(cpu_t *cpu){
    cpu->estado->A = cpu->estado->A * -1;
    cpu->estado->PC+=1;
    return cpu->estado->modo;
}
err_t op_DESV(cpu_t *cpu, int PC){
    int A1;
    err_t leituraResponseA1 = mem_le(cpu->memoria,PC+1,&A1);
    if(leituraResponseA1 != ERR_OK){
        cpu->estado->complemento_memoria = PC+1;
        cpu->estado->modo = leituraResponseA1;
        return cpu->estado->modo;
    }
    cpu->estado->PC = A1;
    return cpu->estado->modo;
}
err_t op_DESVZ(cpu_t *cpu, int PC){
    int A1;
    if(cpu->estado->A == 0){
        err_t leituraResponseA1 = mem_le(cpu->memoria,PC+1,&A1);
        if(leituraResponseA1 != ERR_OK){
            cpu->estado->complemento_memoria = PC+1;
            cpu->estado->modo = leituraResponseA1;
            return cpu->estado->modo;
        }
        cpu->estado->PC = A1;
        return cpu->estado->modo;
    }
    cpu->estado->PC+=2;
    return cpu->estado->modo;
}
err_t op_DESVNZ(cpu_t *cpu, int PC){
    int A1;
    if(cpu->estado->A != 0){
        err_t leituraResponseA1 = mem_le(cpu->memoria,PC+1,&A1);
        if(leituraResponseA1 != ERR_OK){
            cpu->estado->complemento_memoria = PC+1;
            cpu->estado->modo = leituraResponseA1;
            return cpu->estado->modo;
        }
        cpu->estado->PC = A1;
        return cpu->estado->modo;
    }
    cpu->estado->PC+=2;
    return cpu->estado->modo;
}
err_t op_invalido(cpu_t *cpu){
    cpu->estado->modo = ERR_CPU_INSTR_INV;
    return cpu->estado->modo;
}

cpu_estado_t* cpu_estado_cria(){
    cpu_estado_t* c = (cpu_estado_t*)malloc(sizeof(cpu_estado_t));
    //valores iniciais
    c->PC = 0;
    c->A = 0;
    c->X = 0;
    c->complemento_memoria = -1;
    c->complemento_es = -1;
    c->modo = ERR_OK;
    return c;
}
cpu_estado_t * cpu_estado_cria_copia(int PC, int A, int X, err_t modo, int complemento_memoria, int complemento_es){
    cpu_estado_t* c = (cpu_estado_t*)malloc(sizeof(cpu_estado_t));
    c->PC = PC;
    c->A = A;
    c->X = X;
    c->modo = modo;
    c->complemento_memoria = complemento_memoria;
    c->complemento_es = complemento_es;
    return c;

}
void cpu_altera_estado(cpu_t *cpu, cpu_estado_t *estado){
    cpu->estado = estado;
    return;
}
void cpu_altera_memoria(cpu_t *cpu, mem_t *mem){
    cpu->memoria = mem;
    return;
}
void cpu_altera_es(cpu_t *cpu, es_t *es){
    cpu->es = es;
    return;
}
void cpu_libera(cpu_t *cpu){
    free(cpu->estado);
    es_destroi(cpu->es);
    free(cpu);
}
void cpu_altera_modo(cpu_t *cpu, err_t modo){
    cpu->estado->modo = modo;
    return;
}
void cpu_altera_complemento_es(cpu_t *cpu, int valor){
    cpu->estado->complemento_es = valor;
    return;
}
void cpu_altera_A(cpu_t *cpu, int valor){
    cpu->estado->A = valor;
    return;
}
err_t retorna_modo(cpu_t *cpu){
    return cpu->estado->modo;
}
es_t *retorna_es(cpu_t *cpu){
    return cpu->es;
}
cpu_estado_t *retorna_estado(cpu_t *cpu){
    return cpu->estado;
}
int retorna_A(cpu_t *cpu){
    return cpu->estado->A;
}
int retorna_complemento_es(cpu_t * cpu){
    return cpu->estado->complemento_es;
}
int retorna_complemento_memoria(cpu_t * cpu){
    return cpu->estado->complemento_memoria;
}
int retorna_PC(cpu_t *cpu){
    return cpu->estado->PC;
}
int retorna_PC2(cpu_estado_t *cpu_estado){
    return cpu_estado->PC;
}
int retorna_X(cpu_t * cpu){
    return cpu->estado->X;
}
void incrementa_PC(cpu_t *cpu, int valor){
    cpu->estado->PC += valor;
    return;
}
mem_t* retorna_memoria(cpu_t *cpu){
    return cpu->memoria;
}
