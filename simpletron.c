#include "simpletron.h"

#include "constantes.h"

#include<string.h>
#include<math.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void printMessages()
{
    printf("\n--------------------------------\n");
    printf("\n*** Bem vindo ao Simpletron! ***\n");
    printf("\n*** Favor digitar seu programa, uma instrução ***\n");
    printf("\n*** (ou palavra de dados) por vez. Mostrarei ***\n");
    printf("\n*** o número do local e uma interrogação (?). ***\n");
    printf("\n*** Você, então, deverá digitar a palavra para esse ***\n");
    printf("\n*** local. Digite a sentinela -99999 para ***\n");
    printf("\n*** encerrar a entrada do seu programa. ***\n");
    printf("\n--------------------------------\n");
}

Simpletron simpletronInitializer()
{
    Simpletron simpletron;

    for(int i=0; i < 100; i++)
    {
        simpletron.memory[i] = 0;
    }
    simpletron.accumulator = 0;
    simpletron.instructionCounter = 0;
    simpletron.instructionRegister = 0;
    simpletron.operand = 0;
    simpletron.operationCode = 0;

    return simpletron;
}

void computerDump(Simpletron *simpletron)
{
    printf("\n\nREGISTERS:\n\n");

    printf("\naccumulator: %+05d\n", simpletron->accumulator);
    printf("\ninstructionCounter: %02d\n", simpletron->instructionCounter);
    printf("\ninstructionRegister: %+05d\n", simpletron->instructionRegister);
    printf("\noperationCode: %02d\n", simpletron->operationCode);
    printf("\noperand: %02d\n", simpletron->operand);

    printf("\n\nMEMORY:\n\n");

    printf("\t");
    for(int i = 0; i < 10; i++)
    {
        printf("%d       ", i);
    }

    printf("\n\n");

    for(int j = 0; j < MAX_MEMORY; j+=10)
    {
        printf("%02d   ", j);

        for(int k = 0; k <10; k++)
        {
            printf(" %+05d  ", simpletron->memory[k + j]);
        }

        printf("\n\n");
    }
}

bool estaNointervalo(int data)
{
    return data >= -9999 && data <= 9999;
}

bool ehZero(int data)
{
    return data == 0;
}

void mensagemErro(Simpletron *simpletron, ErrorType type)
{
    switch(type)
    {
        case INTERVAL_ERROR:

            printf("\n** O valor aramzenado está fora do limite previsto. [-9999, 9999]. **\n");
            computerDump(simpletron);
            printf("\n** Encerrando a execução do programa do programa de maneira anormal **\n");
            break;

        case DIVIDE_ERROR:

            printf("\n** Não é possível dividir por zero. **\n");
            computerDump(simpletron);
            printf("\n** Encerrando a execução do programa do programa de maneira anormal **\n");
            break;
    }
}

bool executarPrograma(Simpletron *simpletron, int operationCode)
{

    switch(operationCode)
    {
        case READ:

                printf("\nDigite um número: ");
                scanf("%d", &simpletron->memory[simpletron->operand]);

                if(!estaNointervalo(simpletron->memory[simpletron->operand]))
                {
                    mensagemErro(simpletron, INTERVAL_ERROR);
                    return false;
                }

                break;

            case WRITE:

                printf("\n** Número em %02d: %d. **\n", simpletron->operand, simpletron->memory[simpletron->operand]);
                break;

            case LOAD:

                simpletron->accumulator = simpletron->memory[simpletron->operand];
                break;

            case STORE:

                simpletron->memory[simpletron->operand] = simpletron->accumulator;
                break;

            case ADD:

                simpletron->accumulator += simpletron->memory[simpletron->operand];

                if(!estaNointervalo(simpletron->accumulator))
                {
                    mensagemErro(simpletron, INTERVAL_ERROR);
                    return false;
                }

                break;

            case SUBTRACT:

                simpletron->accumulator -= simpletron->memory[simpletron->operand];

                if(!estaNointervalo(simpletron->accumulator))
                {
                    mensagemErro(simpletron, INTERVAL_ERROR);
                    return false;
                }

                break;

            case MULTIPLY:

                simpletron->accumulator *= simpletron->memory[simpletron->operand];

                 if(!estaNointervalo(simpletron->accumulator))
                {
                    mensagemErro(simpletron, INTERVAL_ERROR);
                    return false;
                }

                break;

            case DIVIDE:

                if(ehZero(simpletron->memory[simpletron->operand]))
                {
                   mensagemErro(simpletron, DIVIDE_ERROR);
                    return false;
                }

                simpletron->accumulator /= simpletron->memory[simpletron->operand];

                 if(!estaNointervalo(simpletron->accumulator))
                {
                    mensagemErro(simpletron, INTERVAL_ERROR);
                    return false;
                }

                break;

            case BRANCH:

                simpletron->instructionCounter = simpletron->operand;
                break;

            case BRANCHNEG:

                if(simpletron->accumulator < 0)
                {
                    simpletron->instructionCounter = simpletron->operand;
                }

                break;

            case BRANCHZERO:

                if(simpletron->accumulator == 0)
                {
                    simpletron->instructionCounter = simpletron->operand;
                }

                break;

            case HALT:

                computerDump(simpletron);
                printf("\n** Encerrando a execução do programa de maneira normal **\n");
                return false; //Só para sair do laço.

            default:

                printf("\n** Instrução inválida. **\n");
                computerDump(simpletron);
                printf("\n** Encerrando a execução do programa de maneira normal **\n");
                return false;
                break;
    }

    return true;
}

void lerPrograma(Simpletron *simpletron, const char *fileName)
{
    srand(time(NULL));

    FILE *file = fopen(fileName, "r");

    if(file == NULL)
    {
        printf("\nErro ao ler o arquivo!\n");
        return;
    }

    printf("\n** Carga do programa concluída. **\n");
    printf("\n** Iniciando a execução do programa. **\n");
    printf("\n--------------------------------\n");

    char linha[7];

    while(fgets(linha, 7, file) && simpletron->instructionCounter < MAX_MEMORY)
    {
        linha[strcspn(linha, "\n")] = '\0';

        if(atoi(linha) == -99999)
        {
            printf("\n** O valor sentinela foi encontrado. **\n");
            printf("\n** Encerrando a execução do programa de maneira normal. **\n");
            computerDump(simpletron);
            fclose(file);
            return;
        }

        simpletron->memory[simpletron->instructionCounter] = atoi(linha);

        if(!estaNointervalo(simpletron->memory[simpletron->instructionCounter]))
        {
            mensagemErro(simpletron, INTERVAL_ERROR);
            fclose(file);
            return;
        }

        simpletron->instructionRegister = simpletron->memory[simpletron->instructionCounter];

        simpletron->operationCode = simpletron->instructionRegister / 100;

        simpletron->operand = simpletron->instructionRegister % 100;

        if(!executarPrograma(simpletron, simpletron->operationCode))
        {
            fclose(file);
            return;
        }

        simpletron->instructionCounter++;
    }
}
