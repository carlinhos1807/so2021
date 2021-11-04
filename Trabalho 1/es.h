#include "memoria.h"

typedef struct es es_t;

err_t es_le(es_t *es, int dispositivo, int *pvalor);
err_t es_escreve(es_t *es, int dispositivo, int valor);
es_t* es_cria();
void es_destroi(es_t *es);

