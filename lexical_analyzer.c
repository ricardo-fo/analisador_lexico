#include "lexical_analyzer.h"
#include "token.h"
#include <stdio.h>
#include <string.h>

bool analyse(const char *expression)
{
    int type = -2;
    int previous_type = -2;
    char aux[(int)(strlen(expression) / sizeof(char)) + 1];
    char symbol[(int)(strlen(expression) / sizeof(char)) + 1];
    strcpy(aux, expression);
    strcpy(symbol, "");

    do
    {
        if (((int)(strlen(aux) / sizeof(char))) > 0 && !get_symbol(aux, symbol))
            return true;

        previous_type = type;
        type = check_type(aux, symbol);

        if ((previous_type == SYM_FLOAT || previous_type == SYM_INTEGER) && (type == SYM_FLOAT || type == SYM_INTEGER))
            type = -1;
        printf("%10s ", symbol);

        switch (type)
        {
        case SYM_INTEGER:
            printf("=> número inteiro\n");
            break;
        case SYM_FLOAT:
            printf("=> ponto flutuante\n");
            break;
        case SYM_UNARY_OPERATOR:
            printf("=> operador unário\n");
            break;
        case SYM_BINARY_OPERATOR:
            printf("=> operador binário\n");
            break;
        case SYM_COMMAND:
            printf("=> comando\n");
            break;
        default:
            printf("=> erro\n");
            return false;
        }
    } while ((int)(strlen(aux) / sizeof(char)) > 0);

    return true;
}

int check_type(char *expression, char symbol[])
{
    char copy[(int)(strlen(expression) / sizeof(char)) + 1];
    strcpy(copy, expression);

    int type = is_sym_number(copy, symbol);
    if (type == -1)
        type = is_sym_operator(symbol);
    if (type == -1)
        type = is_sym_command(symbol);

    strcpy(expression, copy);

    return type;
}

// Número: (\+ | \-){0, 1}\d{1, 8}(\.\d{1, 6}){0, 1}(\E(\+ | \-){0, 1}\d{1, 6}){0, 1}
int is_sym_number(char expression[], char symbol[])
{
    char original[(int)(strlen(expression) / sizeof(char)) + 1];
    char number[(int)(strlen(expression) / sizeof(char)) + 1];
    bool is_float = false;        // Indica se é um ponto flutuante
    int counter = 1;              // Conta a quantidade de tokens no número
    strcpy(original, expression); // Salva a string original em casos de não ser um número
    strcpy(number, symbol);

    // Verifica se é um número com sinal
    if (strcmp(symbol, "+") == 0 || strcmp(symbol, "-") == 0)
    {
        if ((int)strlen(expression) == 0)
            return -1;
        get_symbol(expression, symbol);
        strcat(number, symbol);
        counter++;
    }

    // Verifica se há um número no início ou após o sinal
    if (!is_number2(symbol))
    {
        strcpy(expression, original);
        strcpy(symbol, number);
        return -1;
    }

    // Enquanto for um número com menos de 9 dígitos, continua
    while (is_number2(symbol) && counter < 9 && (int)(strlen(expression) / sizeof(char)) > 0)
    {
        get_symbol(expression, symbol);
        strcat(number, symbol);
        counter++;
    }

    // Verifica se após o número há um ponto
    if (strcmp(symbol, ".") == 0)
    {
        is_float = true;
        if ((int)strlen(expression) == 0)
        {
            strcpy(expression, original);
            return -1;
        }
        get_symbol(expression, symbol);
        strcat(number, symbol);
        counter++;

        if (!is_number2(symbol))
        {
            strcpy(expression, "");
            strcpy(symbol, number);
            return -1;
        }
    }

    // Enquanto for um número com menos de 9 dígitos, continua
    while (is_number2(symbol) && counter < 9 && (int)(strlen(expression) / sizeof(char)) > 0)
    {
        get_symbol(expression, symbol);
        strcat(number, symbol);
        counter++;
    }

    // Verifica se após o número há uma exponenciação de 10
    if (strcasecmp(symbol, "e") == 0)
    {
        if ((int)strlen(expression) == 0)
        {
            strcpy(expression, original);
            return -1;
        }
        get_symbol(expression, symbol);
        strcat(number, symbol);
        counter++;

        if (strcmp(symbol, "-") == 0 || strcmp(symbol, "+") == 0)
        {
            if ((int)strlen(expression) == 0)
            {
                strcpy(expression, original);
                return -1;
            }
            get_symbol(expression, symbol);
            strcat(number, symbol);
            counter++;
        }

        // Verifica se há um número após o sinal
        if (!is_number2(symbol))
        {
            strcpy(expression, original);
            strcpy(symbol, number);
            return -1;
        }

        // Enquanto for um número com menos de 9 dígitos, continua
        while (is_number2(symbol) && counter < 9 && (int)(strlen(expression) / sizeof(char)) > 0)
        {
            get_symbol(expression, symbol);
            strcat(number, symbol);
            counter++;
        }
    }

    if (counter > 9)
    {
        printf("Erro! Muitos tokens em um mesmo número. Limite: 8.\n");
        strcpy(expression, "");
        strcpy(symbol, number);
        return -1;
    }

    // Reinserção do símbolo removido
    if (!is_number2(symbol))
    {
        strcpy(original, "<");
        strcat(original, symbol);
        strcat(original, ">");
        strcat(original, expression);
        strcpy(expression, original);

        // Remoção do símbolo adicional
        int idx = strlen(number) - strlen(symbol);
        strncpy(symbol, number, idx);
        symbol[idx] = '\0';
    }
    else
    {
        strcpy(symbol, number);
    }

    return is_float ? SYM_FLOAT : SYM_INTEGER;
}

int is_sym_operator(char symbol[])
{
    char aux[((int)(strlen(symbol) / sizeof(char))) + 3];
    strcpy(aux, "");
    strcat(aux, "<");
    strcat(aux, symbol);
    strcat(aux, ">");

    if (!is_operator(aux))
        return -1;
    if (strcasecmp(symbol, "sen") == 0 || strcasecmp(symbol, "cos") == 0)
        return SYM_UNARY_OPERATOR;
    return SYM_BINARY_OPERATOR;
}

int is_sym_command(char symbol[])
{
    char aux[((int)(strlen(symbol) / sizeof(char))) + 3];
    strcpy(aux, "");
    strcat(aux, "<");
    strcat(aux, symbol);
    strcat(aux, ">");

    if (!is_command(aux))
        return -1;
    return SYM_COMMAND;
}

// Busca pelo primeiro token da string
bool get_symbol(char expression[], char symbol[])
{
    char *startAtPtr = strstr(expression, "<");                        // Ponteiro para o primeiro '<'
    char *endAtPtr = strstr(expression, ">");                          // Ponteiro para o primeiro '>'
    int startAt = (startAtPtr == NULL ? -1 : startAtPtr - expression); // Índice para o primeiro '<'
    int endAt = (endAtPtr == NULL ? -1 : endAtPtr - expression);       // Índice para o primeiro '>'

    if (startAt != 0)
        return false;
    if (endAt == -1 || startAt == -1)
        return false;
    if (startAt > endAt)
        return false;

    memset(symbol, 0, strlen(symbol));
    strncpy(symbol, expression + 1, ++endAt - 2);
    symbol[endAt - 2] = '\0';

    if ((int)(strlen(expression) / sizeof(char) - endAt) == 0)
    {
        memset(expression, 0, endAt);
    }
    else
    {
        char aux[(int)(strlen(expression) / sizeof(char)) - endAt];
        strncpy(aux, expression + endAt, (int)(strlen(expression) / sizeof(char)));
        strcpy(expression, aux);
    }

    return true;
}

bool is_number2(const char symbol[])
{
    return strstr("0123456789", symbol) != NULL;
}
