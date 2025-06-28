#ifndef FUNCOES_H_INCLUDED
#define FUNCOES_H_INCLUDED

#include "constantes.h"

#include<stdbool.h>

typedef enum
{
    INTERVAL_ERROR,
    DIVIDE_ERROR,
    INVALID_INSTRUCTION

}ErrorType;

/*
IMPORTANTE: Resolvi tratar o Simpletron como uma struct para evitar o uso de vari�veis globais,
consideradas uma m� pr�tica de programa��o.
*/
typedef struct
{
    int memory[MAX_MEMORY];
    int accumulator; //Representa o registrador do acumulador.
    int instructionCounter; //Registra o local da mem�ria que cont�m a instru��o que est� sendo executada.
    int instructionRegister; //Armazena a instru��o que est� sendo executada no momento.
    int operationCode; //Indica a opera��o que est� sendo executado no momento.
    int operand; //Indica o local da mem�ria onde a instru��o est� operando no momento.

}Simpletron;

void printMessages();

Simpletron simpletronInitializer();

void computerDump(Simpletron *simpletron);

bool estaNointervalo(int data);

bool ehZero(int data);

void mensagemErro(Simpletron *simpletron, ErrorType type);

bool executarInstrucao(Simpletron *simpletron, int operationCode);

bool lerPrograma(Simpletron *simpletron);

void executarPrograma(Simpletron *simpletron);

#endif // FUNCOES_H_INCLUDED
