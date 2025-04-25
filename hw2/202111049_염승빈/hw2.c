#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// 토큰 종류를 정의: 숫자, 연산자, 괄호
typedef enum
{
    NUMBER,
    OPERATOR,
    PAREN_LEFT,
    PAREN_RIGHT
} TokenType;

// 토큰 구조체: 종류(type)와 문자열 표현(str)
typedef struct
{
    TokenType type;
    char str[32]; // 숫자나 연산자 등을 저장
} Token;

// Token은 수식의 의미 단위(숫자, 연산자, 괄호)를 type과 str로 나눠 저장
// 예를 들어 -2.5e1은 type=NUMBER, str="-2.5e1"

#define MAX_TOKENS 100 // 최대 토큰 개수

// ? 연산자인지 확인하는 함수
int is_operator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// ? 연산자의 우선순위 반환
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

// ? 오른쪽 결합 연산자인지 확인 (^만 해당)
int is_right_associative(char op)
{
    return op == '^';
}

// ? 숫자 표현에 포함될 수 있는 문자 확인 함수
int is_number_char(char c)
{
    return isdigit(c) || c == '.' || c == 'e' || c == 'E' || c == 'f' || c == 'F';
}

// ? 수식 문자열을 토큰 배열로 분리하는 함수
int tokenize(const char *expr, Token tokens[])
{
    int i = 0, k = 0; // i: 수식 인덱스, k: 토큰 인덱스

    while (expr[i])
    {
        if (isspace(expr[i]))
        { // 공백은 무시
            i++;
            continue;
        }

        if (expr[i] == '(')
        { // 여는 괄호는 바로 추가 type = PAREN_LEFT, str = "("
            tokens[k++] = (Token){PAREN_LEFT, "("};
            i++;
        }
        else if (expr[i] == ')')
        { // 닫는 괄호도 추가
            tokens[k++] = (Token){PAREN_RIGHT, ")"};
            i++;
        }

        // 연산자일 경우
        else if (is_operator(expr[i]))
        {
            // 단항 음수인지 판단: 수식 시작(-3 + 2)이거나 앞에 괄호 ((-3)) /연산자가 있는 경우 (2 * -3)
            if ((i == 0 || expr[i - 1] == '(' || is_operator(expr[i - 1])) && expr[i] == '-')
            {
                int j = i;
                char buf[32] = {0};
                buf[0] = '-';
                j++;

                // 음수 숫자를 완전히 읽음 (예: -3.14e2)
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
                // 일반 연산자 처리
                tokens[k++] = (Token){OPERATOR, {expr[i]}};
                i++;
            }
        }

        // 숫자 (정수, 실수, 지수 포함)
        else if (isdigit(expr[i]) || expr[i] == '.')
        {
            char buf[32] = {0};
            int j = 0;

            // 숫자 전체를 읽음
            while (is_number_char(expr[i]))
            {
                buf[j++] = expr[i++];
            }

            tokens[k++] = (Token){NUMBER, ""};
            strcpy_s(tokens[k - 1].str, sizeof(tokens[k - 1].str), buf);
        }

        // 그 외 문자는 무시
        else
        {
            i++;
        }
    }

    return k; // 토큰 개수 반환
}

// ? 중위 → 후위 변환 (Shunting Yard 알고리즘)
int infix_to_postfix(Token *infix, int len, Token *postfix)
{
    Token stack[MAX_TOKENS]; // 연산자 스택
    int top = -1;
    int j = 0; // postfix 인덱스

    for (int i = 0; i < len; i++)
    {
        if (infix[i].type == NUMBER)
        {
            // 숫자는 그대로 출력으로 추가
            postfix[j++] = infix[i];
        }

        else if (infix[i].type == OPERATOR)
        {
            // 스택의 연산자들과 우선순위 비교해서 pop
            while (top != -1 && stack[top].type == OPERATOR &&
                   ((precedence(stack[top].str[0]) > precedence(infix[i].str[0])) ||
                    (precedence(stack[top].str[0]) == precedence(infix[i].str[0]) &&
                     !is_right_associative(infix[i].str[0]))))
            {
                postfix[j++] = stack[top--];
            }
            // 현재 연산자 push
            stack[++top] = infix[i];
        }

        else if (infix[i].type == PAREN_LEFT)
        {
            // 여는 괄호는 스택에 넣음
            stack[++top] = infix[i];
        }

        else if (infix[i].type == PAREN_RIGHT)
        {
            // 닫는 괄호는 '(' 만날 때까지 pop
            while (top != -1 && stack[top].type != PAREN_LEFT)
            {
                postfix[j++] = stack[top--];
            }
            top--; // '(' 버리기
        }
    }

    // 남은 연산자 모두 출력
    while (top != -1)
    {
        postfix[j++] = stack[top--];
    }

    return j; // 후위 토큰 개수
}

// ? 후위 표기식을 계산하는 함수
double evaluate_postfix(Token *postfix, int len)
{
    double stack[MAX_TOKENS]; // 피연산자 저장 스택
    int top = -1;

    for (int i = 0; i < len; i++)
    {
        if (postfix[i].type == NUMBER)
        {
            // 문자열을 실수로 변환 후 push
            stack[++top] = strtod(postfix[i].str, NULL);
        }

        else if (postfix[i].type == OPERATOR)
        {
            // 연산자면 피연산자 2개 pop
            double b = stack[top--];
            double a = stack[top--];

            // 연산 수행
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

    return stack[top]; // 계산 결과
}

// ? 메인 함수: 파일에서 수식 읽고 처리
int main()
{
    FILE *fp = fopen("input.txt", "r");
    if (!fp)
    {
        perror("파일 열기 실패");
        return 1;
    }

    int n;
    fscanf_s(fp, "%d\n", &n); // 수식 개수 읽기

    char line[256];
    for (int i = 0; i < n; i++)
    {
        fgets(line, sizeof(line), fp); // 한 줄 읽기

        // 토큰 분해
        Token infix[MAX_TOKENS], postfix[MAX_TOKENS];
        int token_len = tokenize(line, infix);

        // 후위 변환
        int postfix_len = infix_to_postfix(infix, token_len, postfix);

        // 후위 계산
        double result = evaluate_postfix(postfix, postfix_len);

        // 결과 출력
        printf("Infix: %s", line);
        printf("Postfix: ");
        for (int j = 0; j < postfix_len; j++)
        {
            printf("%s ", postfix[j].str);
        }
        printf("\nResult: %.6lf\n\n", result);
    }

    fclose(fp); // 파일 닫기
    return 0;
}
