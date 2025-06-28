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

/*
Função responsável pela inicialização do Simpletron e de seus componentes internos.
*/
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

/*
Função responsável por imprimir o conteúdo de cada registrador, bem como
o conteúdo completo da memória.
*/
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

            printf("\n** O valor armazenado está fora do limite previsto. [-9999, 9999]. **\n");
            printf("\n** Execução do Simpletron encerrada de forma anormal. **\n");
            computerDump(simpletron);
            break;

        case DIVIDE_ERROR:

            printf("\n** Tentativa de divisão por zero. **\n");
            printf("\n** Execução do Simpletron encerrada de forma anormal. **\n");
            computerDump(simpletron);
            break;

        case INVALID_INSTRUCTION:

            printf("\n** Instrução fora do limite previsto. [-9999, 9999]. **\n");
            printf("\n** Execução do Simpletron encerrada de forma anormal. **\n");
            computerDump(simpletron);
            break;
    }
}

bool executarInstrucao(Simpletron *simpletron, int operationCode)
{
    /*
    IMPORTANTE: as instruções de BRANCH, BRANCHNEG (apenas se o acumulador for negativo),
    BRANCHZERO(apenas se o acumulador for zero) e HALT não mudam o contador de instruções , pois
    elas são instruções de transferência de controle.
    */

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

                simpletron->instructionCounter++;

                break;

            case WRITE:

                printf("\n** Número em %02d: %d. **\n", simpletron->operand, simpletron->memory[simpletron->operand]);
                simpletron->instructionCounter++;
                break;

            case LOAD:

                simpletron->accumulator = simpletron->memory[simpletron->operand];
                simpletron->instructionCounter++;
                break;

            case STORE:

                simpletron->memory[simpletron->operand] = simpletron->accumulator;
                simpletron->instructionCounter++;
                break;

            case ADD:

                simpletron->accumulator += simpletron->memory[simpletron->operand];

                if(!estaNointervalo(simpletron->accumulator))
                {
                    mensagemErro(simpletron, INTERVAL_ERROR);
                    return false;
                }

                simpletron->instructionCounter++;

                break;

            case SUBTRACT:

                simpletron->accumulator -= simpletron->memory[simpletron->operand];

                if(!estaNointervalo(simpletron->accumulator))
                {
                    mensagemErro(simpletron, INTERVAL_ERROR);
                    return false;
                }

                simpletron->instructionCounter++;

                break;

            case MULTIPLY:

                simpletron->accumulator *= simpletron->memory[simpletron->operand];

                 if(!estaNointervalo(simpletron->accumulator))
                {
                    mensagemErro(simpletron, INTERVAL_ERROR);
                    return false;
                }

                simpletron->instructionCounter++;

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

                simpletron->instructionCounter++;

                break;

            case BRANCH:

                simpletron->instructionCounter = simpletron->operand;
                break;

            case BRANCHNEG:

                if(simpletron->accumulator < 0)
                {
                    simpletron->instructionCounter = simpletron->operand;
                }
                else
                {
                    simpletron->instructionCounter++;
                }

                break;

            case BRANCHZERO:

                if(simpletron->accumulator == 0)
                {
                    simpletron->instructionCounter = simpletron->operand;
                }
                else
                {
                    simpletron->instructionCounter++;
                }

                break;

            case HALT:

                printf("\n** Encerrando a execução do programa de maneira normal. **\n");
                computerDump(simpletron);
                return false; //Só para sair do laço.

            default:

                printf("\n** Instrução inválida. **\n");
                printf("\n** Encerrando a execução do programa de maneira anormal. **\n");
                computerDump(simpletron);
                return false;
                break;
    }

    return true;
}

bool lerPrograma(Simpletron *simpletron)
{
    char nomeArquivo[NOME_ARQUIVO];

    printf("\nDigite o nome do arquivo onde está o seu programa: ");
    fgets(nomeArquivo, NOME_ARQUIVO, stdin);
    nomeArquivo[strcspn(nomeArquivo, "\r\n")] = '\0';

    FILE *file = fopen(nomeArquivo, "r");

    if(file == NULL)
    {
        return false;
    }

    char linha[10];

    while(fgets(linha, 10, file) && simpletron->instructionCounter < MAX_MEMORY)
    {
        linha[strcspn(linha, "\r\n")] = '\0';

        simpletron->instructionRegister = atoi(linha);

        /*
        Apenas armazena a instrução se ela não for a sentinela e nem for uma instrução
        inválida.
        */
        if(simpletron->instructionRegister == -99999)
        {
            break;
        }
        else if(!estaNointervalo(simpletron->instructionRegister))
        {
            mensagemErro(simpletron, INVALID_INSTRUCTION);
            /*
            Fiz isso, porque quando cair no if de executarPrograma(), o programa não
            será executado o que é conveniente já que uma instrução inválida foi digitada.
            */
            return false;
        }
        else
        {
            simpletron->memory[simpletron->instructionCounter] = simpletron->instructionRegister;

            simpletron->instructionCounter++;
        }

    }

    printf("\n** Carga do programa concluída. **\n");

    fclose(file);

    simpletron->instructionCounter = 0;

    simpletron->instructionRegister = 0;

    return true;

}

/*
IMPORTANTE: Essa função simula o famoso ciclo de processamento ou ciclo de instrução
realizado pelo Processador.
*/
void executarPrograma(Simpletron *simpletron)
{
    /*
    Esse primeiro if garante que o programa apenas será executado se tiver dado tudo certo
    na leitura do arquivo.
    */
    if(!lerPrograma(simpletron))
    {
        printf("\n** Erro ao ler o programa do arquivo. **\n");
        return;
    }

    printf("\n** Iniciando a execução do programa. **\n");

    while(simpletron->instructionCounter < MAX_MEMORY)
    {

        /*
        CICLO DE BUSCA DA INSTRUÇÃO
        Não podemos executar as instruções diretamente da memória, logo, o primeiro passo é transferir
        a instrução que está armzanada na memória para dentro do registrador e instrução.
        */
        simpletron->instructionRegister = simpletron->memory[simpletron->instructionCounter];

        /*
        Recolhendo os dígitos da esquerda e colocando dentro da variável código de operação e
        recolhendo os dígitos da direita e armazenando na variável operando.
        */
        simpletron->operationCode = simpletron->instructionRegister / 100;

        simpletron->operand = simpletron->instructionRegister % 100;

        /*
        CICLO DE EXECUÇÃO DA INSTRUÇÃO.
        */
        if(!executarInstrucao(simpletron, simpletron->operationCode))
        {
            return;
        }
    }

}
