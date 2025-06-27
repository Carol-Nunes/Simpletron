#ifndef FUNCOES_H_INCLUDED
#define FUNCOES_H_INCLUDED

#include "constantes.h"

#include<stdbool.h>

typedef enum
{
    INTERVAL_ERROR,
    DIVIDE_ERROR

}ErrorType;

typedef struct
{
    int memory[MAX_MEMORY];
    int accumulator;
    int instructionCounter;
    int instructionRegister;
    int operationCode;
    int operand;

}Simpletron;

void printMessages();

Simpletron simpletronInitializer();

void computerDump(Simpletron *simpletron);

bool estaNointervalo(int data);

bool ehZero(int data);

void mensagemErro(Simpletron *simpletron, ErrorType type);

bool executarPrograma(Simpletron *simpletron, int operationCode);

void lerPrograma(Simpletron *simpletron, const char *fileName);

#endif // FUNCOES_H_INCLUDED
