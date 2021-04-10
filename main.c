/*
 * Alunos:
 * Alexandre Saura.
 * Fábio Thomaz.
 * Ricardo Oliveira.
 * Rodrigo Suarez.
 *
 * Trabalho de Compiladores I - UNISANTOS - março de 2021.
 * Prof.: José Fontebasso Neto.
 */
#include <stdlib.h> // free
#include <stdio.h>  // printf

#include "util.h"             // read_string
#include "token.h"            // check_tokens
#include "lexical_analyzer.h" // analyse

int main()
{
    printf("Exemplo de expressão: <4><.><2><*><7><+><log><8>\n");
    printf("Insira sua expressão: ");

    char *expression;
    expression = read_string();

    printf("\n");

    if (!check_tokens(expression))
        return 1;
    if (!analyse(expression))
    {
        printf("\nEntrada inválida!\n");
        return 1;
    }

    free(expression);
    return 0;
}
