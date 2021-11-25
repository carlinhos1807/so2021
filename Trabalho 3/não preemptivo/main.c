#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "cpu.h"
#include "so.h"
#include "uc.h"
#include "relogio.h"

int main()
{
  //-1 no final indica o final do vetor de instrucoes
  //para evitar problemas
  int prog1[] = {2, 0, 7, 2, 10, 5, 17, 8, 20, 1,9, 8, 11, 17, 18, 7, 1, 0};
  int prog2[] = {2, 0, 7, 19, 0, 5, 17, 8, 20, 1,9, 8, 11, 17, 18, 7, 1, 0};
  int prog3[] = {2, 0, 7, 2, 10, 5, 17, 8, 20, 1,9, 8, 11, 17, 18, 7, 1, 0};
  int prog4[] = {2, 0, 7, 2, 10, 5, 17, 8, 20, 1,9, 8, 11, 17, 18, 7, 1, 0};

  int tam_prog1 = sizeof(prog1)/sizeof(prog1[0]);
  int tam_prog2 = sizeof(prog2)/sizeof(prog2[0]);
  int tam_prog3 = sizeof(prog3)/sizeof(prog3[0]);
  int tam_prog4 = sizeof(prog4)/sizeof(prog4[0]);

  cpu_t *cpu = cpu_cria();

  relogio_t *relogio = relogio_cria();

  so_t *so = so_cria();
  so_altera_cpu(so, cpu);
  so_inicializa_cpu(so,cpu);
  so_inicializa_programa(so, prog1, tam_prog1);
  so_inicializa_programa(so, prog2, tam_prog2);
  so_inicializa_programa(so, prog3, tam_prog3);
  so_inicializa_programa(so, prog4, tam_prog4);

  so_altera_relogio(so, relogio);

  uc_t *uc = uc_cria();
  uc_altera_cpu(uc, cpu);
  uc_altera_relogio(uc, relogio);
  uc_altera_so(uc, so);


  executa(uc);
  cpu_libera(cpu);
  relogio_libera(relogio);
  uc_libera(uc);
  so_libera(so);

  return 0;
}
