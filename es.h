
typedef enum{
    ES_ERR_OK = 0,
     //...
     ES_ERR_ES_DISP_INV,  // acesso a dispositivo inexistente
     ES_ERR_ES_OP_INV,    // operação inválida em dispositivo
     //...
} es_err_t;

typedef struct es es_t;

es_err_t es_le(es_t *es, int dispositivo, int *pvalor);
es_err_t es_escreve(es_t *es, int dispositivo, int valor);
es_t* es_cria();
void es_destroi(es_t *es);

