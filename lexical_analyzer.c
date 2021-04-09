#include "lexical_analyzer.h"
#include "stack.h"
#include "token.h"
#include <stdio.h>
#include <string.h>

bool analyse(const char * expression) {
    Stack * stack;
    int type;
    char aux[ (int)(strlen(expression) / sizeof(char)) ];
    char symbol[ (int)(strlen(expression) / sizeof(char)) ];
    strcpy(aux, expression);
    strcpy(symbol, "");

    do {
        // printf(">>> %s\n", aux);
        if (!get_symbol(aux, symbol)) return true;

        type = check_type(aux, stack, symbol);
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
        // printf("aux: %s\n", aux);
        // printf("l: %d\n", (int)(strlen(aux)/sizeof(char)));
    } while((int)(strlen(aux)/sizeof(char)) > 0);

    return true;
}

int check_type(char * expression, Stack * stack, char symbol[]) {
    char copy[ (int)(strlen(expression) / sizeof(char)) ];
    strcpy(copy, expression);

    int type = is_sym_number(copy, stack, symbol);
    if (type == -1) type = is_sym_operator(copy, stack, symbol);
    if (type == -1) type = is_sym_command(copy, stack, symbol);

    strcpy(expression, copy);

    return type;
}

// Número: (\+ | \-){0, 1}\d{1, 8}(\.\d{1, 6}){0, 1}(\E(\+ | \-){0, 1}\d{1, 6}){0, 1}
int is_sym_number(char expression[], Stack * stack, char symbol[]) {
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

int is_sym_operator(char expression[], Stack * stack, char symbol[]) {
    char aux[((int) (strlen(symbol) / sizeof(char))) + 2];
    strcpy(aux, "");
    strcat(aux, "<");
    strcat(aux, symbol);
    strcat(aux, ">");

    if (!is_operator(aux)) return -1;
    if (strcasecmp(symbol, "sen") == 0 || strcasecmp(symbol, "cos") == 0) return SYM_UNARY_OPERATOR;
    return SYM_BINARY_OPERATOR;
}

int is_sym_command(char expression[], Stack * stack, char symbol[]) {
    char aux[((int) (strlen(symbol) / sizeof(char))) + 2];
    strcpy(aux, "");
    strcat(aux, "<");
    strcat(aux, symbol);
    strcat(aux, ">");

    if (!is_command(aux)) return -1;
    return SYM_COMMAND;
}
/*
    bool last_char_and_binary_operator = false;
    bool last_char_and_number = false;
    bool second_number = false;
    bool first_char = true;
    bool is_valid_expression = true;

        // Caso o symbol for um numero
        if (is_number(symbol)) {
            
            char integer_aux[ (int)((strlen(symbol)) / sizeof(char)) ];

            memset(integer_aux, 0, strlen(integer_aux));
            strcpy(integer_aux, symbol);

            if (!get_symbol(aux,  symbol)) {
                printf("%s => numero inteiro\n", integer_aux);
                return is_valid_expression;
            } else {
                printf("O symbol '%s' é inválido!\n", symbol);
                return false;
            }

            while(is_number(symbol)) {
                push(stack, symbol);

                if (get_symbol(aux,  symbol) == 1) {
                    printf("%s => numero inteiro\n", integer_aux);
                    printStack(stack);
                    return is_valid_expression;
                }
            }

            // vai verificar se o conjunto de symbols pego é um numero inteiro ou com ponto flutuante
            if(strstr(".", symbol)) {
                printf("%s\n", integer_aux);
            } else {
                printf("%s => numero inteiro\n", integer_aux);
            }

            printStack(stack);
            putsymbol(aux, symbol);
            memset(symbol, 0, strlen(symbol));
            strcat(symbol, integer_aux);
        // vai verificar se é um comando
        } else if(strstr(symbol, "enter")) {
            printf("%s => comando\n", symbol);
            second_number = !second_number;

        // vai verificar se é um numero com sinal
        } else if(first_char && strstr("-", symbol)) {

            bool valido = false;

            if (pegasymbol(aux,  symbol) == 1) {
                printf("- => erro\n");
                return false;
            }

            while(strstr("0123456789", symbol)) {
                push(stack, symbol);
                valido = true;

                if (pegasymbol(aux,  symbol) == 1) {
                    printf("- => sinal negativo\n");
                    printStack(stack);
                    return is_valid_expression;
                }
            }

            if(valido) {
                printf("- => sinal negativo\n");
                printStack(stack);

                putsymbol(aux, symbol);

                memset(symbol, 0, strlen(symbol));
                strcat(symbol, "-");
            } else {
                is_valid_expression = false;

                printf("- => erro\n");
                printStack(stack);

                if (pegasymbol(aux, symbol) == 1) return 1;

                while(!strstr("enter", symbol)) {
                    printf("%s\n", symbol);

                    if (pegasymbol(aux, symbol) == 1) return 1;
                }

                printf("%s\n", symbol);
            }

        // vai verificar se é um numero com ponto flutuante
        } else if(strstr(".", symbol)) {

            if (pegasymbol(aux, symbol) == 1) {
                printf("%s => erro\n", symbol);
                return false;
            }

            if(!last_char_and_number && !first_char) {
                is_valid_expression = false;

                printf(". => erro\n");

                while(!strstr("enter", symbol)) {
                    printf("%s\n", symbol);

                    if (pegasymbol(aux, symbol) == 1) return is_valid_expression;
                }

                printf("%s\n", symbol);
            } else {

                bool valido = false;

                while(strstr("0123456789", symbol)) {
                    push(stack, symbol);
                    valido = true;

                    if (pegasymbol(aux,  symbol) == 1) {
                        printf(". => ponto fluatuante\n");
                        printStack(stack);
                        return is_valid_expression;
                    }
                }

                if(valido) {
                    printf(". => ponto fluatuante\n");
                    printStack(stack);

                    putsymbol(aux, symbol);
                } else {
                    is_valid_expression = false;

                    printf(". => erro\n");
                    printStack(stack);

                    while(!strstr("enter", symbol)) {
                        printf("%s\n", symbol);

                        if (pegasymbol(aux, symbol) == 1) return is_valid_expression;
                    }

                    printf("%s\n", symbol);
                }
            }
        // vai verificar se é uma potencia
        } else if(strstr(symbol, "^") || strstr(symbol, "E") || strstr(symbol, "e")) {

            if (pegasymbol(aux, symbol) == 1) {
                printf("%s => erro\n", symbol);
                return false;
            }

            char auxPotencia[(int)((strlen(symbol))/sizeof(char))];

            memset(auxPotencia, 0, strlen(auxPotencia));

            strcat(auxPotencia, symbol);

            if(!last_char_and_number && !first_char) {
                is_valid_expression = false;

                printf(". => erro\n");

                while(!strstr("enter", symbol)) {
                    printf("%s\n", symbol);

                    if (pegasymbol(aux, symbol) == 1) return is_valid_expression;
                }

                printf("%s\n", symbol);
            } else {

                bool valido = false;

                while(strstr("0123456789", symbol)) {
                    push(stack, symbol);

                    valido = true;

                    if (pegasymbol(aux, symbol) == 1) {
                        printf("%s => potencia\n", auxPotencia);
                        printStack(stack);
                        return is_valid_expression;
                    }
                }

                if(valido) {
                    printf("%s => potencia\n", auxPotencia);

                    printStack(stack);

                    putsymbol(aux, symbol);
                } else {
                    printf("%s => erro\n", auxPotencia);

                    printStack(stack);

                    while(!strstr("enter", symbol)) {
                        printf("%s\n", symbol);

                        if (pegasymbol(aux, symbol) == 1) return is_valid_expression;
                    }

                    printf("%s\n", symbol);
                }

            }

        // vai verificar se é um operador binario
        } else if(strstr("+-*", symbol)) {

            if(second_number && last_char_and_number) {
                printf("%s => operador binario\n", symbol);
            } else {
                is_valid_expression = false;

                printf("%s => erro\n", symbol);

                if (pegasymbol(aux, symbol) == 1) return is_valid_expression;

                while(!strstr("enter", symbol)) {
                    printf("%s\n", symbol);

                    if (pegasymbol(aux, symbol) == 1) return is_valid_expression;
                }

                printf("%s\n", symbol);
            }

        // vai verificar se é um operador unario
        } else if(strstr(symbol, "log") || strstr(symbol, "cos") || strstr(symbol, "sen")) {

            if(last_char_and_number || last_char_and_binary_operator) {
                printf("%s => operador unario\n", symbol);
            } else {
                printf("%s => erro\n", symbol);

                if (pegasymbol(aux, symbol) == 1) return is_valid_expression;

                while(!strstr("enter", symbol)) {
                    printf("%s\n", symbol);

                    if (pegasymbol(aux, symbol) == 1) return is_valid_expression;
                }

                printf("%s\n", symbol);
            }
        }

        last_char_and_number = strstr("0123456789.", symbol);

        last_char_and_binary_operator = strstr("+-*", symbol);

        first_char = strstr("enter", symbol);
    } while((int)(strlen(aux)/sizeof(char)) > 0);

    return is_valid_expression;
}
*/

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
