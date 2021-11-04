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
    c->estado = NULL;
    c->es = NULL;
    c->memoria = NULL;
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
err_t cpu_executa_1(cpu_t *cpu){
    if(cpu->estado->modo != ERR_OK){
        return cpu->estado->modo;
    }
    int PC = cpu->estado->PC;
    int instrucao;
    err_t leituraResponse = mem_le(cpu->memoria,PC,&instrucao);
    if(leituraResponse != ERR_OK){
        cpu->estado->modo = leituraResponse;
        return cpu->estado->modo;
    }
    if(instrucao == 0){
        cpu->estado->PC+=1;
        return cpu->estado->modo;
    }else if(instrucao == 1){
        cpu->estado->modo = ERR_CPU_PARADA;
        return cpu->estado->modo;
    }else if(instrucao == 2){
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
    }else if(instrucao == 3){
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
    }else if(instrucao == 4){
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
    }else if(instrucao == 5){
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

    }else if(instrucao == 6){
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
    }else if(instrucao == 7){
        cpu->estado->X = cpu->estado->A;
        cpu->estado->PC+=1;
        return cpu->estado->modo;
    }else if(instrucao == 8){
        cpu->estado->A = cpu->estado->X;
        cpu->estado->PC+=1;
        return cpu->estado->modo;
    }else if(instrucao == 9){
        cpu->estado->X+=1;
        cpu->estado->PC+=1;
        return cpu->estado->modo;
    }else if(instrucao == 10){
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
    }else if(instrucao == 11){
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
    }else if(instrucao == 12){
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
    }else if(instrucao == 13){
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
    }else if(instrucao == 14){
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
    }else if(instrucao == 15){
        cpu->estado->A = cpu->estado->A * -1;
        cpu->estado->PC+=1;
        return cpu->estado->modo;
    }else if(instrucao == 16){
        int A1;
        err_t leituraResponseA1 = mem_le(cpu->memoria,PC+1,&A1);
        if(leituraResponseA1 != ERR_OK){
            cpu->estado->complemento_memoria = PC+1;
            cpu->estado->modo = leituraResponseA1;
            return cpu->estado->modo;
        }
        cpu->estado->PC = A1;
        return cpu->estado->modo;
    }else if(instrucao == 17){
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
    }else if(instrucao == 18){
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
    }else if(instrucao == 19){
        int valor;
        err_t leituraResponseES = es_le(cpu->es,0,&valor);
        if(leituraResponseES != ERR_OK){
            cpu->estado->complemento_es = 0;
            cpu->estado->modo = leituraResponseES;
            return cpu->estado->modo;
        }
        cpu->estado->A = valor;
        cpu->estado->PC+=2;
        return cpu->estado->modo;
    }else if(instrucao == 20){
        err_t escritaResponseES = es_escreve(cpu->es,1,cpu->estado->A);
        if(escritaResponseES != ERR_OK){
            cpu->estado->complemento_es = 1;
            cpu->estado->modo = escritaResponseES;
            return cpu->estado->modo;
        }
        cpu->estado->PC+=2;
        return cpu->estado->modo;
    }else{
        cpu->estado->modo = ERR_CPU_INSTR_INV;
        return cpu->estado->modo;
    }


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
    free(cpu);
}
