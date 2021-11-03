#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

struct cpu_estado{
    int PC;
    int A;
    int X;
    cpu_err_t modo;
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
    printf("PC = %d\n", cpu->estado->PC);
    printf("A = %d\n", cpu->estado->A);
    printf("X = %d\n", cpu->estado->X);
    printf("comp_mem = %d\n", cpu->estado->complemento_memoria);
    printf("com_es = %d\n", cpu->estado->complemento_es);
    printf("erro = %d\n\n",cpu->estado->modo);
    return;
}
cpu_estado_t *cpu_estado(cpu_t *cpu){
    return cpu->estado;
}
cpu_err_t cpu_executa_1(cpu_t *cpu){
    if(cpu->estado->modo != CPU_ERR_OK){
        return cpu->estado->modo;
    }
    int PC = cpu->estado->PC;
    int dado;
    mem_le(cpu->memoria,PC,&dado);
    if(dado == 0){
        cpu->estado->PC+=1;
        return cpu->estado->modo;
    }else if(dado == 1){
        cpu->estado->PC+=1;
        cpu->estado->modo = CPU_ERR_CPU_PARADA;
        return cpu->estado->modo;
    }else if(dado == 2){
        int A1;
        if(mem_le(cpu->memoria,PC+1,&A1) != MEM_ERR_OK){
            cpu->estado->complemento_memoria = PC+1;
            cpu->estado->modo = CPU_ERR_MEM;
            return cpu->estado->modo;
        }
        cpu->estado->A = A1;
        cpu->estado->PC+=2;
        return cpu->estado->modo;
    }else if(dado == 3){
        int A1;
        if(mem_le(cpu->memoria,PC+1,&A1) != MEM_ERR_OK){
            cpu->estado->complemento_memoria = PC+1;
            cpu->estado->modo = CPU_ERR_MEM;
            return cpu->estado->modo;
        }
        int memA1;
        if(mem_le(cpu->memoria,A1,&memA1) != MEM_ERR_OK){
            cpu->estado->complemento_memoria = A1;
            cpu->estado->modo = CPU_ERR_MEM;
            return cpu->estado->modo;
        }
        cpu->estado->A = memA1;
        cpu->estado->PC+=2;
        return cpu->estado->modo;
    }else if(dado == 4){

        int A1;
        if(mem_le(cpu->memoria,PC+1,&A1) != MEM_ERR_OK){
            cpu->estado->complemento_memoria = PC+1;
            cpu->estado->modo = CPU_ERR_MEM;
            return cpu->estado->modo;
        }

        int soma = A1 + cpu->estado->X;
        int memIndexado;
        if(mem_le(cpu->memoria,soma,&memIndexado) != MEM_ERR_OK){
            cpu->estado->complemento_memoria = soma;
            cpu->estado->modo = CPU_ERR_MEM;
            return cpu->estado->modo;
        }
        cpu->estado->A = memIndexado;
        cpu->estado->PC+=2;
        return cpu->estado->modo;
    }else if(dado == 5){

        int A1;
        if(mem_le(cpu->memoria,PC+1,&A1) != MEM_ERR_OK){
            cpu->estado->complemento_memoria = PC+1;
            cpu->estado->modo = CPU_ERR_MEM;
            return cpu->estado->modo;
        }
        if(mem_escreve(cpu->memoria,A1,cpu->estado->A) != MEM_ERR_OK){
            cpu->estado->complemento_memoria = A1;
            cpu->estado->modo = CPU_ERR_MEM;
            return cpu->estado->modo;
        }
        cpu->estado->PC+=2;
        return cpu->estado->modo;

    }else if(dado == 6){
        int A1;

        if(mem_le(cpu->memoria,PC+1,&A1) != MEM_ERR_OK){
            cpu->estado->complemento_memoria = PC+1;
            cpu->estado->modo = CPU_ERR_MEM;
            return cpu->estado->modo;
        }

        int soma = A1 + cpu->estado->X;

        if(mem_escreve(cpu->memoria,soma,cpu->estado->A) != MEM_ERR_OK){
            cpu->estado->complemento_memoria = soma;
            cpu->estado->modo = CPU_ERR_MEM;
            return cpu->estado->modo;
        }
        cpu->estado->PC+=2;
        return cpu->estado->modo;
    }else if(dado == 7){
        cpu->estado->X = cpu->estado->A;
        cpu->estado->PC+=1;
        return cpu->estado->modo;
    }else if(dado == 8){
        cpu->estado->A = cpu->estado->X;
        cpu->estado->PC+=1;
        return cpu->estado->modo;
    }else if(dado == 9){
        cpu->estado->X+=1;
        cpu->estado->PC+=1;
        return cpu->estado->modo;
    }else if(dado == 10){
        int A1;
        if(mem_le(cpu->memoria,PC+1,&A1) != MEM_ERR_OK){
            cpu->estado->complemento_memoria = PC+1;
            cpu->estado->modo = CPU_ERR_MEM;
            return cpu->estado->modo;
        }

        int memA1;
        if(mem_le(cpu->memoria,A1,&memA1) != MEM_ERR_OK){
            cpu->estado->complemento_memoria = A1;
            cpu->estado->modo = CPU_ERR_MEM;
            return cpu->estado->modo;
        }

        cpu->estado->A+=memA1;
        cpu->estado->PC+=2;
        return cpu->estado->modo;
    }else if(dado == 11){
        int A1;
        if(mem_le(cpu->memoria,PC+1,&A1) != MEM_ERR_OK){
            cpu->estado->complemento_memoria = PC+1;
            cpu->estado->modo = CPU_ERR_MEM;
            return cpu->estado->modo;
        }

        int memA1;
        if(mem_le(cpu->memoria,A1,&memA1) != MEM_ERR_OK){
            cpu->estado->complemento_memoria = A1;
            cpu->estado->modo = CPU_ERR_MEM;
            return cpu->estado->modo;
        }

        cpu->estado->A-=memA1;
        cpu->estado->PC+=2;
        return cpu->estado->modo;
    }else if(dado == 12){
        int A1;
        if(mem_le(cpu->memoria,PC+1,&A1) != MEM_ERR_OK){
            cpu->estado->complemento_memoria = PC+1;
            cpu->estado->modo = CPU_ERR_MEM;
            return cpu->estado->modo;
        }

        int memA1;
        if(mem_le(cpu->memoria,A1,&memA1) != MEM_ERR_OK){
            cpu->estado->complemento_memoria = A1;
            cpu->estado->modo = CPU_ERR_MEM;
            return cpu->estado->modo;
        }

        cpu->estado->A*=memA1;
        cpu->estado->PC+=2;
        return cpu->estado->modo;
    }else if(dado == 13){
        int A1;
        if(mem_le(cpu->memoria,PC+1,&A1) != MEM_ERR_OK){
            cpu->estado->complemento_memoria = PC+1;
            cpu->estado->modo = CPU_ERR_MEM;
            return cpu->estado->modo;
        }

        int memA1;
        if(mem_le(cpu->memoria,A1,&memA1) != MEM_ERR_OK){
            cpu->estado->complemento_memoria = A1;
            cpu->estado->modo = CPU_ERR_MEM;
            return cpu->estado->modo;
        }

        cpu->estado->A/=memA1;
        cpu->estado->PC+=2;
        return cpu->estado->modo;
    }else if(dado == 14){
        int A1;
        if(mem_le(cpu->memoria,PC+1,&A1) != MEM_ERR_OK){
            cpu->estado->complemento_memoria = PC+1;
            cpu->estado->modo = CPU_ERR_MEM;
            return cpu->estado->modo;
        }

        int memA1;
        if(mem_le(cpu->memoria,A1,&memA1) != MEM_ERR_OK){
            cpu->estado->complemento_memoria = A1;
            cpu->estado->modo = CPU_ERR_MEM;
            return cpu->estado->modo;
        }

        cpu->estado->A%=memA1;
        cpu->estado->PC+=2;
        return cpu->estado->modo;
    }else if(dado == 15){
        cpu->estado->A = cpu->estado->A * -1;
        cpu->estado->PC+=1;
        return cpu->estado->modo;
    }else if(dado == 16){
        int A1;
        if(mem_le(cpu->memoria,PC+1,&A1) != MEM_ERR_OK){
            cpu->estado->complemento_memoria = PC+1;
            cpu->estado->modo = CPU_ERR_MEM;
            return cpu->estado->modo;
        }
        cpu->estado->PC = A1;
        return cpu->estado->modo;
    }else if(dado == 17){
        int A1;
        if(cpu->estado->A == 0){
            if(mem_le(cpu->memoria,PC+1,&A1) != MEM_ERR_OK){
            cpu->estado->complemento_memoria = PC+1;
            cpu->estado->modo = CPU_ERR_MEM;
            return cpu->estado->modo;
        }
        cpu->estado->PC = A1;
        return cpu->estado->modo;
        }
        cpu->estado->PC+=2;
        return cpu->estado->modo;
    }else if(dado == 18){
        int A1;
        if(cpu->estado->A != 0){
            if(mem_le(cpu->memoria,PC+1,&A1) != MEM_ERR_OK){
            cpu->estado->complemento_memoria = PC+1;
            cpu->estado->modo = CPU_ERR_MEM;
            return cpu->estado->modo;
        }
        cpu->estado->PC = A1;
        return cpu->estado->modo;
        }
        cpu->estado->PC+=2;
        return cpu->estado->modo;
    }else if(dado == 19){
        int valor;
        if(es_le(cpu->es,0,&valor) != ES_ERR_OK){
            cpu->estado->complemento_es = 0;
            cpu->estado->modo = CPU_ERR_ES;
            return cpu->estado->modo;
        }
        cpu->estado->A = valor;
        cpu->estado->PC+=2;
        return cpu->estado->modo;
    }else if(dado == 20){
        if(es_escreve(cpu->es,1,cpu->estado->A) != ES_ERR_OK){
            cpu->estado->complemento_es = 1;
            cpu->estado->modo = CPU_ERR_ES;
            return cpu->estado->modo;
        }
        cpu->estado->PC+=2;
        return cpu->estado->modo;
    }else{
        cpu->estado->modo = CPU_ERR_CPU_INSTR_INV;
        return cpu->estado->modo;
    }


}


cpu_estado_t* cpu_estado_cria(){
    cpu_estado_t* c = (cpu_estado_t*)malloc(sizeof(cpu_estado_t));
    c->PC = 0;
    c->A = 0;
    c->X = 0;
    c->complemento_memoria = -1;
    c->complemento_es = -1;
    c->modo = CPU_ERR_OK;
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
