#ifndef ES_H_INCLUDED
#define ES_H_INCLUDED

#include "err.h"

typedef struct es es_t;

err_t es_le(es_t *es, int dispositivo, int *pvalor, int pid);
err_t es_escreve(es_t *es, int dispositivo, int valor, FILE *arquivo_escrita);
es_t* es_cria();
void es_destroi(es_t *es);
void ajusta_nome_arquivo_es(int pid, char *vetor);

#endif // ES_H_INCLUDED
