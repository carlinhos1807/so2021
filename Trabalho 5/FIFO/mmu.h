#ifndef MMU_H_INCLUDED
#define MMU_H_INCLUDED

#include "memoria.h"
#include "constantes.h"
#include <stdbool.h>

typedef struct mmu mmu_t;
typedef struct tabpag tabpag_t;
typedef struct pagina pagina_t;

mmu_t* mmu_cria();
err_t mmu_le(mmu_t *m, int endereco, int *pvalor);
err_t mmu_escreve(mmu_t *m, int endereco, int valor);
void mmu_altera_memoria(mmu_t *m, mem_t *mem);
void mmu_altera_tabela_pagina(mmu_t *m, tabpag_t *tab);
void mmu_libera(mmu_t *mmu);
void associa(tabpag_t *tabpag, int numero_pagina, int quadro);
bool verifica_alterado(tabpag_t *tab, int pagid);
void reseta_pagina(tabpag_t *tab, int id);
int tabela_retorna_fal_pag(tabpag_t *tab);
void libera_tabela(tabpag_t *tab);
void reseta_todas_paginas(tabpag_t *tab);
mem_t* mmu_retorna_memoria(mmu_t *mmu);
#endif // MMU_H_INCLUDED
