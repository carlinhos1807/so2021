
typedef enum{
    MEM_ERR_OK = 0,
    MEM_ERR_MEM_END_INV
} mem_err_t;

typedef struct memoria mem_t;

int mem_tam(mem_t *m);

mem_err_t mem_le(mem_t *m, int endereco, int *pvalor);

mem_err_t mem_escreve(mem_t *m, int endereco, int valor);

mem_t *mem_cria(int tam);

void mem_destroi(mem_t *m);
