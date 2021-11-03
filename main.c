// montador para SO21b

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "cpu.h"

#define MEMORYSIZE 20
// auxiliares

// aborta o programa com uma mensagem de erro
void erro_brabo(char *msg)
{
  fprintf(stderr, "ERRO FATAL: %s\n", msg);
  exit(1);
}

// retorna true se tem um n�mero na string s (e retorna o n�mero tamb�m); false se n�o
bool tem_numero(char *s, int *num)
{
  if (isdigit(*s) || (*s == '-' && isdigit(*(s+1)))) {
    *num = atoi(s);
    return true;
  }
  return false;
}

// memoria de saida

// representa a mem�ria do programa -- a sa�da do montador � colocada aqui

#define MEM_TAM 1000
int mem[MEM_TAM];
int mem_pos;        // pr�xima posi�ao livre da mem�ria

// coloca um valor no final da mem�ria
void mem_insere(int val)
{
  if (mem_pos >= MEM_TAM-1) {
    erro_brabo("programa muito grande!");
  }
  mem[mem_pos++] = val;
}

// altera o valor em uma posi��o j� ocupada da mem�ria
void mem_altera(int pos, int val)
{
  if (pos < 0 || pos >= mem_pos) {
    erro_brabo("erro interno, altera��o de regi�o n�o inicializada");
  }
  mem[pos] = val;
}

// imprime o conte�do da mem�ria
int* mem_imprime()
{
  int tamVetor = 0;
  for (int i = 0; i < mem_pos; i+=10) {
    printf("    /* %03d */", i);
    for (int j = i; j < i+10 && j < mem_pos; j++) {
      printf(" %d,", mem[j]);
      tamVetor++;
    }
    printf("\n");
  }
  if(tamVetor > MEMORYSIZE){
    printf("tamanho do programa nao suportado !");
    return 0;
  }
  int *vetorSaida =  (int*) malloc (MEMORYSIZE*sizeof (int));
  int aux = 0;

  for (int i = 0; i < mem_pos; i+=10) {
    for (int j = i; j < i+10 && j < mem_pos; j++) {
      vetorSaida[aux] = mem[j];
      aux++;
    }
  }

  int sobra = MEMORYSIZE - tamVetor;
  if(sobra){
    for(int i = MEMORYSIZE - 1; i >= MEMORYSIZE - sobra; i--){
        vetorSaida[i] = -1;
    }
  }

  return vetorSaida;

}

// simbolos

// tabela com os s�mbolos (labels) j� definidos pelo programa, e o valor (endere�o) deles

#define SIMB_TAM 1000
struct {
  char *nome;
  int endereco;
} simbolo[SIMB_TAM];
int simb_num;             // n�mero d s�mbolos na tabela

// retorna o valor de um s�mbolo, ou -1 se n�o existir na tabela
int simb_endereco(char *nome)
{
  for (int i=0; i<simb_num; i++) {
    if (strcmp(nome, simbolo[i].nome) == 0) {
      return simbolo[i].endereco;
    }
  }
  return -1;
}

// insere um novo s�mbolo na tabela
void simb_novo(char *nome, int endereco)
{
  if (nome == NULL) return;
  if (simb_endereco(nome) != -1) {
    fprintf(stderr, "ERRO: redefinicao do simbolo '%s'\n", nome);
    return;
  }
  if (simb_num >= SIMB_TAM) {
    erro_brabo("Excesso de s�mbolos");
  }
  simbolo[simb_num].nome = strdup(nome);
  simbolo[simb_num].endereco = endereco;
  simb_num++;
}


// referencias

// tabela com refer�ncias a s�mbolos, cont�m a linha e o endere�o correspondente onde o s�mbolo foi referenciado

#define REF_TAM 1000
struct {
  char *nome;
  int linha;
  int endereco;
} ref[REF_TAM];
int ref_num;      // numero de refer�ncias criadas

// insere uma nova refer�ncia na tabela
void ref_nova(char *nome, int linha, int endereco)
{
  if (nome == NULL) return;
  if (ref_num >= REF_TAM) {
    erro_brabo("excesso de refer�ncias");
  }
  ref[ref_num].nome = strdup(nome);
  ref[ref_num].linha = linha;
  ref[ref_num].endereco = endereco;
  ref_num++;
}

// resolve as refer�ncias -- para cada refer�ncia, coloca o valor do s�mbolo no endere�o onde ele � referenciado
void ref_resolve(void)
{
  for (int i=0; i<ref_num; i++) {
    int endereco = simb_endereco(ref[i].nome);
    if (endereco == -1) {
      fprintf(stderr,
              "ERRO: simbolo '%s' referenciado na linha %d n�o foi definido\n",
              ref[i].nome, ref[i].linha);
    }
    mem_altera(ref[i].endereco, endereco);
  }
}


// instrucoes

// tabela com as instru��es reconhecidas pelo montador
// al�m das instru��es da CPU, tem ainda algumas pseudo-instru��es, que s�o processadas pelo montador, n�o geram c�digo:
//   VALOR - inicaliza a pr�xima posi��o de mem�ria com o valor do argumento
//   ESPACO - reserva tantas palavras de espa�o nas pr�ximas posi��es da mem�ria (corresponde a tantos "VALOR 0")
//   DEFINE - define um valor para um s�mbolo (obrigatoriamente tem que ter um label,
//            que � definido com o valor do argumento e n�o com a posi��o atual da mem�ria) -- ainda n�o implementado

typedef enum {
  NOP,    PARA,   CARGI,  CARGM,  CARGX,  ARMM,   ARMX,   MVAX,
  MVXA,   INCX,   SOMA,   SUB,    MULT,   DIV,    RESTO,  NEG,
  DESV,   DESVZ,  DESVNZ, LE,     ESCR,   VALOR,  ESPACO, DEFINE,
} opcode_t;
struct {
  char *nome;
  int num_args;
} instrucoes[] = {
  { "NOP",    0 },
  { "PARA",   0 },
  { "CARGI",  1 },
  { "CARGM",  1 },
  { "CARGX",  1 },
  { "ARMM",   1 },
  { "ARMX",   1 },
  { "MVAX",   0 },
  { "MVXA",   0 },
  { "INCX",   0 },
  { "SOMA",   1 },
  { "SUB",    1 },
  { "MULT",   1 },
  { "DIV",    1 },
  { "RESTO",  1 },
  { "NEG",    0 },
  { "DESV",   1 },
  { "DESVZ",  1 },
  { "DESVNZ", 1 },
  { "LE",     1 },
  { "ESCR",   1 },
  // pseudo-instrucoes
  { "VALOR",  1 },
  { "ESPACO", 1 },
  { "DEFINE", 1 },
};
#define INSTR_NUM (sizeof(instrucoes)/sizeof(instrucoes[0]))

// retorna o opcode correspondente a instru��o com o nome dado
int instr_opcode(char *nome)
{
  if (nome == NULL) return -1;
  for (int i=0; i<INSTR_NUM; i++) {
    if (strcmp(instrucoes[i].nome, nome) == 0) {
      return i;
    }
  }
  return -1;
}

// montagem

// realiza a montagem de uma instru��o (gera o c�digo para ela na mem�ria), tendo opcode e argumento
void monta_instrucao(int linha, int opcode, char *arg)
{
  int argn;  // para conter o valor num�rico do argumento

  // trata pseudo-opcodes antes
  if (opcode == ESPACO) {
    if (!tem_numero(arg, &argn) || argn < 1) {
      fprintf(stderr, "ERRO: linha %d 'ESPACO' deve ter valor positivo\n",
              linha);
      return;
    }
    for (int i = 0; i < argn; i++) {
      mem_insere(0);
    }
    return;
  } else if (opcode == VALOR) {
    // nao faz nada, vai inserir o valor definido em arg
  } else {
    // instru��o real, coloca o opcode da instru��o na mem�ria
    mem_insere(opcode);
  }
  if (instrucoes[opcode].num_args == 0) {
    return;
  }
  if (tem_numero(arg, &argn)) {
    mem_insere(argn);
  } else {
    // n�o � n�mero, p�e um 0 e insere uma refer�ncia para alterar depois
    ref_nova(arg, linha, mem_pos);
    mem_insere(0);
  }
}

void monta_linha(int linha, char *label, char *instrucao, char *arg)
{
  int num_instr = instr_opcode(instrucao);
  // pseudo-instru��o DEFINE tem que ser tratada antes, porque n�o pode definir o label de forma normal
  if (num_instr == DEFINE) {
    int argn;  // para conter o valor num�rico do argumento
    if (label == NULL) {
      fprintf(stderr, "ERRO: linha %d: 'DEFINE' exige um label\n",
                      linha);
    } else if (!tem_numero(arg, &argn)) {
      fprintf(stderr, "ERRO: linha %d 'DEFINE' exige valor num�rico\n",
              linha);
    } else {
      // tudo OK, define o s�mbolo
      simb_novo(label, argn);
    }
    return;
  }

  // cria s�mbolo correspondente ao label, se for o caso
  if (label != NULL) {
    simb_novo(label, mem_pos);
  }

  // verifica a exist�ncia de instru��o e n�mero coreto de argumentos
  if (instrucao == NULL) return;
  if (num_instr == -1) {
    fprintf(stderr, "ERRO: linha %d: instrucao '%s' desconhecida\n",
                    linha, instrucao);
    return;
  }
  if (instrucoes[num_instr].num_args == 0 && arg != NULL) {
    fprintf(stderr, "ERRO: linha %d: instrucao '%s' n�o tem argumento\n",
                    linha, instrucao);
    return;
  }
  if (instrucoes[num_instr].num_args == 1 && arg == NULL) {
    fprintf(stderr, "ERRO: linha %d: instrucao '%s' necessita argumento\n",
                    linha, instrucao);
    return;
  }
  //
  monta_instrucao(linha, num_instr, arg);
}

// retorna true se o caractere for um espa�o (ou tab)
bool espaco(char c)
{
  return c == ' ' || c == '\t';
}

// encontra o primeiro caractere que n�o seja espa�o (ou tab) na string
char *pula_ate_espaco(char *s)
{
  while (!espaco(*s) && *s != '\0') {
    s++;
  }
  return s;
}

// troca espa�os por fim de string
char *detona_espacos(char *s)
{
  while (espaco(*s)) {
    *s = '\0';
    s++;
  }
  return s;
}

// faz a string terminar no in�cio de um coment�rio, se houver
// aproveita e termina se chegar no fim de linha
void tira_comentario(char *s)
{
  while(*s != '\0' && *s != ';' && *s != '\n') {
    s++;
  }
  *s = '\0';
}

// uma linha mont�vel � formada por [label][ instrucao[ argumento]]
// o que est� entre [] � opcional
// as partes s�o separadas por espa�o(s)
// de ';' em diante, ignora-se (coment�rio)
// a string � alterada, colocando-se NULs no lugar dos espa�os, para separ�-la em substrings
// quem precisar guardar essas substrings, deve copi�-las.
void monta_string(int linha, char *str)
{
  char *label = NULL;
  char *instrucao = NULL;
  char *arg = NULL;
  tira_comentario(str);
  if (*str == '\0') return;
  if (!espaco(*str)) {
    label = str;
    str = pula_ate_espaco(str);
  }
  str = detona_espacos(str);
  if (*str != '\0') {
    instrucao = str;
    str = pula_ate_espaco(str);
  }
  str = detona_espacos(str);
  if (*str != '\0') {
    arg = str;
    str = pula_ate_espaco(str);
  }
  str = detona_espacos(str);
  if (*str != '\0') {
    fprintf(stderr, "linha %d: ignorando '%s'\n", linha, str);
  }
  if (label != NULL || instrucao != NULL) {
    monta_linha(linha, label, instrucao, arg);
  }
}

void monta_arquivo(char *nome)
{
  int linha = 1;
  FILE *arq;
  arq = fopen(nome, "r");
  if (arq == NULL) {
    fprintf(stderr, "N�o foiposs�vel abrir o arquivo '%s'\n", nome);
    return;
  }
  while (true) {
    char lin[500];
    if (fgets(lin, 500, arq) == NULL) break;
    int n = strlen(lin) - 1;
    if (lin[n] == '\n') lin[n] = '\0';
    monta_string(linha, lin);
    linha++;
  }
  fclose(arq);
  ref_resolve();
}

int main(int argc, char *argv[argc])
{
  if (argc != 2) {
    fprintf(stderr, "ERRO: chame como '%s nome_do_arquivo'\n", argv[0]);
    return 1;
  }

  monta_arquivo(argv[1]);
  int *prog = mem_imprime();
  //printf("%d",saida[17]);

  // vari�veis que representam o computador
  mem_t *mem = mem_cria(MEMORYSIZE);
  es_t *es = es_cria();
  cpu_t *cpu = cpu_cria();
  cpu_estado_t *estado = cpu_estado_cria();

   // copia o programa para a mem�ria
  for (int i = 0; i < MEMORYSIZE; i++) {
     if (mem_escreve(mem, i, prog[i]) != MEM_ERR_OK) {
        printf("Erro de memoria, endereco %d\n", i);
        exit(1);
     }
  }
    // inicializa a CPU com as vari�veis criadas
   cpu_altera_estado(cpu, estado);
   cpu_altera_memoria(cpu, mem);
   cpu_altera_es(cpu, es);

    // executa uma instru��o por vez at� parar
    while (true) {
      cpu_imprime_estado(cpu);
      cpu_err_t err = cpu_executa_1(cpu);
      if (err != CPU_ERR_OK) {
        printf("\nErro na execucao: %d\n", err);
        printf("\nEstado final:\n");
        cpu_imprime_estado(cpu);
        break;
      }
    }
  mem_destroi(mem);
  es_destroi(es);
  cpu_libera(cpu);
  free(prog);
  return 0;
}
