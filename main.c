#include <stdio.h>
#include <stdlib.h>

char * read_string();

int main()
{
    char * str;

    str = read_string();
    printf(">>> %s\n", str);

    free(str);

    return 0;
}

/*
 * Function: read_string
 * ---------------------
 * Lê uma sequência de caractéres do teclado e cria uma string.
 *
 * returns: um ponteiro para a string gerada.
 */
char * read_string() {
    char * str = NULL, ch;
    size_t size = 0;
    int i = 0;

    // Lê os caractéres enquanto não encontrar um ENTER
    while((ch = getchar()) != '\n') {
        str = realloc(str, size);
        size += sizeof(char);
        str[i++] = ch;
    }

    return str;
}
