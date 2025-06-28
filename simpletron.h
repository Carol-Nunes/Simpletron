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
IMPORTANTE: Resolvi tratar o Simpletron como uma struct para evitar o uso de variáveis globais,
consideradas uma má prática de programação.
*/
typedef struct
{
    int memory[MAX_MEMORY];
    int accumulator; //Representa o registrador do acumulador.
    int instructionCounter; //Registra o local da memória que contém a instrução que está sendo executada.
    int instructionRegister; //Armazena a instrução que está sendo executada no momento.
    int operationCode; //Indica a operação que está sendo executado no momento.
    int operand; //Indica o local da memória onde a instrução está operando no momento.

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
