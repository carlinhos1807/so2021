#ifndef ERR_H_INCLUDED
#define ERR_H_INCLUDED

// os tipos de erro que podem acontecer ao tentar executar uma instru��o
typedef enum {
  ERR_OK = 0,        // sem erro
  // erros de mem�ria
  ERR_MEM_END_INV,   // acesso a endere�o de mem�ria inv�lido
  // erros de E/S
  ERR_ES_DISP_INV,   // acesso a dispositivo inexistente
  ERR_ES_OP_INV,     // opera��o inv�lida em dispositivo
  // erros de CPU
  ERR_CPU_PARADA,    // CPU executou instru��o PARA
  ERR_CPU_INSTR_INV, // tentativa de execu��o de instru��o inexistente
  ERR_CPU_PRIV_LE,
  ERR_CPU_PRIV_ESCR,
  ERR_ES_ESCR_FILE,
  ERR_ES_LE_FILE

} err_t;

#endif // ERR_H
