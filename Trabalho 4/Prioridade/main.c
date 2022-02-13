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
  cpu_t *cpu = cpu_cria();

  relogio_t *relogio = relogio_cria();

  so_t *so = so_cria();
  so_altera_cpu(so, cpu);
  so_altera_relogio(so, relogio);
  so_inicializa_cpu(so,cpu);


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
