#include <stdio.h>
#include <stdlib.h>
#include<locale.h>

#include "simpletron.h"
#include "constantes.h"

int main()
{
    setlocale(LC_ALL, "Portuguese");

    printMessages();

    Simpletron simpletron = simpletronInitializer();

    executarPrograma(&simpletron);

    return 0;
}
