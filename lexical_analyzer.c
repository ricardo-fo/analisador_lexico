#include "lexical_analyzer.h"
// #include "stack.h"
#include "token.h"
#include <stdio.h>
#include <string.h>

bool analyse(const char * expression) {
    /*Stack * stack;*/
    int type;
    char aux[ (int)(strlen(expression) / sizeof(char)) ];
    char symbol[ (int)(strlen(expression) / sizeof(char)) ];
    strcpy(aux, expression);
    strcpy(symbol, "");

    do {
        if (!get_symbol(aux, symbol)) return true;

        type = check_type(aux/*, stack*/, symbol);
        printf("%10s ", symbol);

        switch (type) {
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
    } while((int)(strlen(aux)/sizeof(char)) > 0);

    return true;
}

int check_type(char * expression/*, Stack * stack*/, char symbol[]) {
    char copy[ (int)(strlen(expression) / sizeof(char)) ];
    strcpy(copy, expression);

    int type = is_sym_number(copy/*, stack*/, symbol);
    if (type == -1) type = is_sym_operator(/* stack, */ symbol);
    if (type == -1) type = is_sym_command(/* stack, */ symbol);

    strcpy(expression, copy);

    return type;
}

// Número: (\+ | \-){0, 1}\d{1, 8}(\.\d{1, 6}){0, 1}(\E(\+ | \-){0, 1}\d{1, 6}){0, 1}
int is_sym_number(char expression[] /*, Stack * stack*/, char symbol[]) {
    char original[ (int)(strlen(expression) / sizeof(char)) ];
    char number[ (int)(strlen(expression) / sizeof(char)) ];
    bool is_float = false; // Indica se é um ponto flutuante
    int counter = 1;       // Conta a quantidade de tokens no número
    strcpy(original, expression);
    strcpy(number, symbol);

    // Verifica se é um número com sinal
    if (strcmp(symbol, "+") == 0 || strcmp(symbol, "-") == 0) {
        get_symbol(expression, symbol);
        strcat(number, symbol);
        counter++;
    }

    // Verifica se há um número no início ou após o sinal
    if (!is_number2(symbol)) {
        strcpy(expression, original);
        strcpy(symbol, number);
        return -1;
    }

    // Enquanto for um número com menos de 9 dígitos, continua
    while (is_number2(symbol) && counter < 9 && (int)(strlen(expression)/sizeof(char)) > 0) {
        get_symbol(expression, symbol);
        strcat(number, symbol);
        counter++;
    }

    // Verifica se após o número há um ponto
    if (strcmp(symbol, ".") == 0) {
        is_float = true;
        get_symbol(expression, symbol);
        strcat(number, symbol);
        counter++;

        if (!is_number2(symbol)) {
            strcpy(expression, "");
            strcpy(symbol, number);
            return -1;
        }
    }

    // Enquanto for um número com menos de 9 dígitos, continua
    while (is_number2(symbol) && counter < 9 && (int)(strlen(expression)/sizeof(char)) > 0) {
        get_symbol(expression, symbol);
        strcat(number, symbol);
        counter++;
    }

    // Verifica se após o número há uma exponenciação de 10
    if (strcasecmp(symbol, "e") == 0) {
        get_symbol(expression, symbol);
        strcat(number, symbol);
        counter++;

        if (strcmp(symbol, "-") == 0 || strcmp(symbol, "+") == 0) {
            get_symbol(expression, symbol);
            strcat(number, symbol);
            counter++;
        }

        // Verifica se há um número após o sinal
        if (!is_number2(symbol)) {
            strcpy(expression, original);
            strcpy(symbol, number);
            return -1;
        }

        // Enquanto for um número com menos de 9 dígitos, continua
        while (is_number2(symbol) && counter < 9 && (int)(strlen(expression)/sizeof(char)) > 0) {
            get_symbol(expression, symbol);
            strcat(number, symbol);
            counter++;
        }
    }

    if (counter > 9) {
        printf("Erro! Muitos tokens em um mesmo número. Limite: 8.\n");
        strcpy(expression, "");
        strcpy(symbol, number);
        return -1;
    }

    // Reinserção do símbolo removido
    if (!is_number2(symbol)) {
        strcpy(original, "<");
        strcat(original, symbol);
        strcat(original, ">");
        strcat(original, expression);
        strcpy(expression, original);

        // Remoção do símbolo adicional
        int idx = strlen(number) - strlen(symbol);
        strncpy(symbol, number, idx);
        symbol[idx] = '\0';
    } else {
        strcpy(symbol, number);
    }

    return is_float ? SYM_FLOAT : SYM_INTEGER;
}

int is_sym_operator(/* Stack * stack,*/ char symbol[]) {
    char aux[((int) (strlen(symbol) / sizeof(char))) + 2];
    strcpy(aux, "");
    strcat(aux, "<");
    strcat(aux, symbol);
    strcat(aux, ">");

    if (!is_operator(aux)) return -1;
    if (strcasecmp(symbol, "sen") == 0 || strcasecmp(symbol, "cos") == 0) return SYM_UNARY_OPERATOR;
    return SYM_BINARY_OPERATOR;
}

int is_sym_command(/* Stack * stack,*/ char symbol[]) {
    char aux[((int) (strlen(symbol) / sizeof(char))) + 2];
    strcpy(aux, "");
    strcat(aux, "<");
    strcat(aux, symbol);
    strcat(aux, ">");

    if (!is_command(aux)) return -1;
    return SYM_COMMAND;
}

// Busca pelo primeiro token da string
bool get_symbol(char expression[], char symbol[]) {
    char * startAtPtr = strstr(expression, "<"); // Ponteiro para o primeiro '<'
    char * endAtPtr = strstr(expression, ">");   // Ponteiro para o primeiro '>'
    int startAt = (startAtPtr == NULL ? -1 : startAtPtr - expression); // Índice para o primeiro '<'
    int endAt = (endAtPtr == NULL ? -1 : endAtPtr - expression);       // Índice para o primeiro '>'

    if (startAt != 0) return false;
    if (endAt == -1 || startAt == -1) return false;
    if (startAt > endAt) return false;

    memset(symbol, 0, strlen(symbol));
    strncpy(symbol, expression + 1, ++endAt - 2);
    symbol[endAt - 2] = '\0';

    if ((int)(strlen(expression) / sizeof(char) - endAt) == 0) {
        strcpy(expression, "");
    } else {
        char aux[(int)(strlen(expression)/sizeof(char)) - endAt];
        strncpy(aux, expression + endAt, (int)(strlen(expression)/sizeof(char)));
        strcpy(expression, aux);
    }

    return true;
}

bool is_number2(const char symbol[]) {
    return strstr("0123456789", symbol) != NULL;
}

// coloca um token na string
void put_token(char expression[], const char token[]) {
    char aux[(int)((strlen(expression)+strlen(token)+2)/sizeof(char))];

    memset(aux, 0, strlen(aux));

    strcat(aux, "<");
    strncat(aux, token, (int)(strlen(token)/sizeof(char)));
    strcat(aux, ">");

    strncat(aux, expression, (int)(strlen(expression)/sizeof(char)));

    memset(expression, 0, strlen(expression));

    strcpy(expression, aux);
}
