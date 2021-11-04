typedef enum{
    ERR_OK = 0,
    ERR_MEM_END_INV,
    ERR_ES_DISP_INV,  // acesso a dispositivo inexistente
    ERR_ES_OP_INV,    // operação inválida em dispositivo
    ERR_CPU_PARADA,    // CPU executou instrução PARA
    ERR_CPU_INSTR_INV // tentativa de execução de instrução inexistente
} err_t;
