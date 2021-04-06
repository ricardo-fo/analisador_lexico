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

#include "util.h"   // read_string
#include "queue.h"  // Queue, create_queue
#include "token.h"  // check_tokens

int main()
{
    char * expression;
    printf("Exemplo de expressão: <4><.><2><*><7><+><log><8>\n");
    printf("Insira sua expressão: ");

    expression = read_string();
    printf("\n");

    Queue * tokens = create_queue();
    if(!check_tokens(tokens, expression)) return 1;

    printf("Entrada valida!\n");
    free(expression);

    return 0;
}
