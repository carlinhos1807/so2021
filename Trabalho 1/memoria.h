#include "err.h"

typedef struct memoria mem_t;

int mem_tam(mem_t *m);

err_t mem_le(mem_t *m, int endereco, int *pvalor);

err_t mem_escreve(mem_t *m, int endereco, int valor);

mem_t *mem_cria(int tam);

void mem_destroi(mem_t *m);
