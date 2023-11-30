#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct No
{
    double dado;
    struct No *proximo;
} No;

typedef struct
{
    No *topo;
} Pilha;

void inicializar(Pilha *pilha)
{
    pilha->topo = NULL;
}

void empilhar(Pilha *pilha, double valor)
{
    No *novoNo = (No *)malloc(sizeof(No));
    if (novoNo == NULL)
    {
        printf("Erro: Falha na alocação de memória\n");
        exit(EXIT_FAILURE);
    }
    novoNo->dado = valor;
    novoNo->proximo = pilha->topo;
    pilha->topo = novoNo;
}

double desempilhar(Pilha *pilha)
{
    if (pilha->topo != NULL)
    {
        No *temp = pilha->topo;
        double valor = temp->dado;
        pilha->topo = temp->proximo;
        free(temp);
        return valor;
    }
    else
    {
        printf("Erro: Pilha vazia\n");
        exit(EXIT_FAILURE);
    }
}

double avaliarExpressaoPosfixada(const char *expressao)
{
    Pilha pilha;
    inicializar(&pilha);
    char *endptr;
    for (int i = 0; expressao[i] != '\0'; ++i)
    {
        if (isdigit(expressao[i]) || (expressao[i] == '-' && isdigit(expressao[i + 1])))
        {
            empilhar(&pilha, strtod(&expressao[i], &endptr));
            i += endptr - &expressao[i] - 1;
        }
        else if (expressao[i] == '.' || expressao[i] == ',')
        {
            // Trata ponto ou vírgula como separador decimal
            empilhar(&pilha, strtod(&expressao[i], &endptr));
            i += endptr - &expressao[i] - 1;
        }
        else if (expressao[i] == ' ' || expressao[i] == '\t')
        {
            continue;
        }
        else if (strncmp(&expressao[i], "sqrt", 4) == 0)
        {
            double operando = desempilhar(&pilha);
            empilhar(&pilha, sqrt(operando));
            i += 3;
        }
        else if (strncmp(&expressao[i], "log", 3) == 0)
        {
            double operando = desempilhar(&pilha);
            empilhar(&pilha, log10(operando));
            i += 2;
        }
        else if (strncmp(&expressao[i], "sen", 3) == 0)
        {
            double operando = desempilhar(&pilha);
            empilhar(&pilha, sin(operando));
            i += 2;
        }
        else if (strncmp(&expressao[i], "cos", 3) == 0)
        {
            double operando = desempilhar(&pilha);
            empilhar(&pilha, cos(operando));
            i += 2;
        }
        else if (strncmp(&expressao[i], "tan", 3) == 0)
        {
            double operando = desempilhar(&pilha);
            empilhar(&pilha, tan(operando));
            i += 2;
        }
        else if (expressao[i] == '+' || expressao[i] == '-' || expressao[i] == '*' || expressao[i] == '/' || expressao[i] == '^')
        {
            double operando2 = desempilhar(&pilha);
            double operando1 = desempilhar(&pilha);

            switch (expressao[i])
            {
            case '+':
                empilhar(&pilha, operando1 + operando2);
                break;
            case '-':
                empilhar(&pilha, operando1 - operando2);
                break;
            case '*':
                empilhar(&pilha, operando1 * operando2);
                break;
            case '/':
                empilhar(&pilha, operando1 / operando2);
                break;
            case '^':
                empilhar(&pilha, pow(operando1, operando2));
                break;
            }
        }
        else
        {
            printf("Erro: Expressão inválida\n");
            exit(EXIT_FAILURE);
        }
    }
    return desempilhar(&pilha);
}

void inverterExpressao(char *expressao)
{
    int length = strlen(expressao);
    for (int i = 0; i < length / 2; ++i)
    {
        char temp = expressao[i];
        expressao[i] = expressao[length - i - 1];
        expressao[length - i - 1] = temp;
    }
}

void converterParaNotacaoPrefixada(char *expressao)
{
    inverterExpressao(expressao);
    int length = strlen(expressao);
    for (int i = 0; i < length; ++i)
    {
        if (expressao[i] == '(')
            expressao[i] = ')';
        else if (expressao[i] == ')')
            expressao[i] = '(';
    }

    // Invertentndo Caracteres
    for (int j = 0; j < length; j++)
    {
        if (expressao[j] == 'n' && expressao[j + 2] == 't')
        {
            expressao[j] = 't';
            expressao[j + 2] = 'n';
        }
        else if (expressao[j] == 'n' && expressao[j + 2] == 's')
        {
            expressao[j] = 's';
            expressao[j + 2] = 'n';
        }
        else if (expressao[j] == 's' && expressao[j + 2] == 'c')
        {
            expressao[j] = 'c';
            expressao[j + 2] = 's';
        }
        else if (expressao[j] == 'g' && expressao[j + 2] == 'l')
        {
            expressao[j] = 'l';
            expressao[j + 2] = 'g';
        }
    }
}

int main()
{
    char expressaoPosfixada[100];
    printf("Digite a expressao em notacao posfixada: ");
    fgets(expressaoPosfixada, sizeof(expressaoPosfixada), stdin);

    size_t len = strlen(expressaoPosfixada);
    if (len > 0 && expressaoPosfixada[len - 1] == '\n')
        expressaoPosfixada[len - 1] = '\0';

    printf("Resultado: %f\n", avaliarExpressaoPosfixada(expressaoPosfixada));

    char expressaoPrefixada[100];
    strcpy(expressaoPrefixada, expressaoPosfixada);
    converterParaNotacaoPrefixada(expressaoPrefixada);

    printf("Expressao em notacao prefixada: %s\n", expressaoPrefixada);

    return 0;
}