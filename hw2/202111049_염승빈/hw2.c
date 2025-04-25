#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// ��ū ������ ����: ����, ������, ��ȣ
typedef enum
{
    NUMBER,
    OPERATOR,
    PAREN_LEFT,
    PAREN_RIGHT
} TokenType;

// ��ū ����ü: ����(type)�� ���ڿ� ǥ��(str)
typedef struct
{
    TokenType type;
    char str[32]; // ���ڳ� ������ ���� ����
} Token;

// Token�� ������ �ǹ� ����(����, ������, ��ȣ)�� type�� str�� ���� ����
// ���� ��� -2.5e1�� type=NUMBER, str="-2.5e1"

#define MAX_TOKENS 100 // �ִ� ��ū ����

// ? ���������� Ȯ���ϴ� �Լ�
int is_operator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// ? �������� �켱���� ��ȯ
int precedence(char op)
{
    if (op == '^')
        return 3;
    if (op == '*' || op == '/')
        return 2;
    if (op == '+' || op == '-')
        return 1;
    return 0;
}

// ? ������ ���� ���������� Ȯ�� (^�� �ش�)
int is_right_associative(char op)
{
    return op == '^';
}

// ? ���� ǥ���� ���Ե� �� �ִ� ���� Ȯ�� �Լ�
int is_number_char(char c)
{
    return isdigit(c) || c == '.' || c == 'e' || c == 'E' || c == 'f' || c == 'F';
}

// ? ���� ���ڿ��� ��ū �迭�� �и��ϴ� �Լ�
int tokenize(const char *expr, Token tokens[])
{
    int i = 0, k = 0; // i: ���� �ε���, k: ��ū �ε���

    while (expr[i])
    {
        if (isspace(expr[i]))
        { // ������ ����
            i++;
            continue;
        }

        if (expr[i] == '(')
        { // ���� ��ȣ�� �ٷ� �߰� type = PAREN_LEFT, str = "("
            tokens[k++] = (Token){PAREN_LEFT, "("};
            i++;
        }
        else if (expr[i] == ')')
        { // �ݴ� ��ȣ�� �߰�
            tokens[k++] = (Token){PAREN_RIGHT, ")"};
            i++;
        }

        // �������� ���
        else if (is_operator(expr[i]))
        {
            // ���� �������� �Ǵ�: ���� ����(-3 + 2)�̰ų� �տ� ��ȣ ((-3)) /�����ڰ� �ִ� ��� (2 * -3)
            if ((i == 0 || expr[i - 1] == '(' || is_operator(expr[i - 1])) && expr[i] == '-')
            {
                int j = i;
                char buf[32] = {0};
                buf[0] = '-';
                j++;

                // ���� ���ڸ� ������ ���� (��: -3.14e2)
                while (is_number_char(expr[j]))
                {
                    strncat_s(buf, sizeof(buf), &expr[j], 1);
                    j++;
                }

                tokens[k++] = (Token){NUMBER, ""};
                strcpy_s(tokens[k - 1].str, sizeof(tokens[k - 1].str), buf);
                i = j;
            }
            else
            {
                // �Ϲ� ������ ó��
                tokens[k++] = (Token){OPERATOR, {expr[i]}};
                i++;
            }
        }

        // ���� (����, �Ǽ�, ���� ����)
        else if (isdigit(expr[i]) || expr[i] == '.')
        {
            char buf[32] = {0};
            int j = 0;

            // ���� ��ü�� ����
            while (is_number_char(expr[i]))
            {
                buf[j++] = expr[i++];
            }

            tokens[k++] = (Token){NUMBER, ""};
            strcpy_s(tokens[k - 1].str, sizeof(tokens[k - 1].str), buf);
        }

        // �� �� ���ڴ� ����
        else
        {
            i++;
        }
    }

    return k; // ��ū ���� ��ȯ
}

// ? ���� �� ���� ��ȯ (Shunting Yard �˰���)
int infix_to_postfix(Token *infix, int len, Token *postfix)
{
    Token stack[MAX_TOKENS]; // ������ ����
    int top = -1;
    int j = 0; // postfix �ε���

    for (int i = 0; i < len; i++)
    {
        if (infix[i].type == NUMBER)
        {
            // ���ڴ� �״�� ������� �߰�
            postfix[j++] = infix[i];
        }

        else if (infix[i].type == OPERATOR)
        {
            // ������ �����ڵ�� �켱���� ���ؼ� pop
            while (top != -1 && stack[top].type == OPERATOR &&
                   ((precedence(stack[top].str[0]) > precedence(infix[i].str[0])) ||
                    (precedence(stack[top].str[0]) == precedence(infix[i].str[0]) &&
                     !is_right_associative(infix[i].str[0]))))
            {
                postfix[j++] = stack[top--];
            }
            // ���� ������ push
            stack[++top] = infix[i];
        }

        else if (infix[i].type == PAREN_LEFT)
        {
            // ���� ��ȣ�� ���ÿ� ����
            stack[++top] = infix[i];
        }

        else if (infix[i].type == PAREN_RIGHT)
        {
            // �ݴ� ��ȣ�� '(' ���� ������ pop
            while (top != -1 && stack[top].type != PAREN_LEFT)
            {
                postfix[j++] = stack[top--];
            }
            top--; // '(' ������
        }
    }

    // ���� ������ ��� ���
    while (top != -1)
    {
        postfix[j++] = stack[top--];
    }

    return j; // ���� ��ū ����
}

// ? ���� ǥ����� ����ϴ� �Լ�
double evaluate_postfix(Token *postfix, int len)
{
    double stack[MAX_TOKENS]; // �ǿ����� ���� ����
    int top = -1;

    for (int i = 0; i < len; i++)
    {
        if (postfix[i].type == NUMBER)
        {
            // ���ڿ��� �Ǽ��� ��ȯ �� push
            stack[++top] = strtod(postfix[i].str, NULL);
        }

        else if (postfix[i].type == OPERATOR)
        {
            // �����ڸ� �ǿ����� 2�� pop
            double b = stack[top--];
            double a = stack[top--];

            // ���� ����
            switch (postfix[i].str[0])
            {
            case '+':
                stack[++top] = a + b;
                break;
            case '-':
                stack[++top] = a - b;
                break;
            case '*':
                stack[++top] = a * b;
                break;
            case '/':
                stack[++top] = a / b;
                break;
            case '^':
                stack[++top] = pow(a, b);
                break;
            }
        }
    }

    return stack[top]; // ��� ���
}

// ? ���� �Լ�: ���Ͽ��� ���� �а� ó��
int main()
{
    FILE *fp = fopen("input.txt", "r");
    if (!fp)
    {
        perror("���� ���� ����");
        return 1;
    }

    int n;
    fscanf_s(fp, "%d\n", &n); // ���� ���� �б�

    char line[256];
    for (int i = 0; i < n; i++)
    {
        fgets(line, sizeof(line), fp); // �� �� �б�

        // ��ū ����
        Token infix[MAX_TOKENS], postfix[MAX_TOKENS];
        int token_len = tokenize(line, infix);

        // ���� ��ȯ
        int postfix_len = infix_to_postfix(infix, token_len, postfix);

        // ���� ���
        double result = evaluate_postfix(postfix, postfix_len);

        // ��� ���
        printf("Infix: %s", line);
        printf("Postfix: ");
        for (int j = 0; j < postfix_len; j++)
        {
            printf("%s ", postfix[j].str);
        }
        printf("\nResult: %.6lf\n\n", result);
    }

    fclose(fp); // ���� �ݱ�
    return 0;
}
