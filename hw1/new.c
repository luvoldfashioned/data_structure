#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TERMS 100       // 하나의 다항식이 가질 수 있는 최대 항 개수
#define MAX_POLY_STRING 256 // 한 줄에 입력될 수 있는 최대 다항식 길이

// 각각의 항을 저장할 구조체: 계수(coef)와 차수(exp)를 가짐
typedef struct
{
    int coef; // 계수
    int exp;  // 차수 (exponent)
} Term;

// 다항식 구조체: 여러 개의 항을 가질 수 있음
typedef struct
{
    Term terms[MAX_TERMS]; // 항들을 배열로 저장
    int termCount;         // 현재 저장된 항의 개수
} Polynomial;

// 문자열로 된 다항식을 파싱하여 Polynomial 구조체로 변환하는 함수
Polynomial parsePolynomial(char *str)
{
    Polynomial poly = {.termCount = 0}; // 결과 다항식 구조체 초기화
    int i = 0, sign = 1;

    while (str[i])
    {
        int coef = 0, exp = 0;
        int coefSet = 0; // 계수가 명시적으로 설정되었는지 확인
        sign = 1;

        // 공백 스킵
        while (isspace(str[i]))
            i++;

        // 부호 처리
        if (str[i] == '+')
        {
            sign = 1;
            i++;
        }
        else if (str[i] == '-')
        {
            sign = -1;
            i++;
        }

        // 다시 공백 스킵
        while (isspace(str[i]))
            i++;

        // 숫자가 있는 경우: 계수 설정
        if (isdigit(str[i]))
        {
            while (isdigit(str[i]))
            {
                coef = coef * 10 + (str[i++] - '0');
            }
            coefSet = 1;
        }

        // x가 있는 경우
        if (str[i] == 'x')
        {
            i++;
            if (!coefSet)
                coef = 1; // 계수가 생략되었으면 1로 간주

            // x^n 또는 x**n 형태의 차수 처리
            if (str[i] == '^' || (str[i] == '*' && str[i + 1] == '*'))
            {
                if (str[i] == '*')
                    i += 2; // '**' 건너뛰기
                else
                    i++; // '^' 건너뛰기
                exp = 0;
                while (isdigit(str[i]))
                {
                    exp = exp * 10 + (str[i++] - '0');
                }
            }
            else
            {
                exp = 1; // 그냥 x만 있으면 차수는 1
            }
        }
        else
        {
            if (!coefSet)
                coef = 1; // 그냥 숫자도 없으면 계수는 1
            exp = 0;      // 상수항
        }

        // 항 저장
        poly.terms[poly.termCount].coef = coef * sign;
        poly.terms[poly.termCount].exp = exp;
        poly.termCount++;
    }

    return poly;
}

// 같은 차수끼리 계수를 더해서 항을 정리하는 함수
Polynomial combine_like_terms(Polynomial poly)
{
    Polynomial result = {.termCount = 0};

    // 기존 항들을 하나씩 확인하면서 같은 차수인 항을 합침
    for (int i = 0; i < poly.termCount; i++)
    {
        int found = 0;
        for (int j = 0; j < result.termCount; j++)
        {
            if (poly.terms[i].exp == result.terms[j].exp)
            {
                result.terms[j].coef += poly.terms[i].coef;
                found = 1;
                break;
            }
        }
        // 없으면 새로 추가
        if (!found)
        {
            result.terms[result.termCount++] = poly.terms[i];
        }
    }

    // 계수가 0인 항은 제거
    Polynomial final = {.termCount = 0};
    for (int i = 0; i < result.termCount; i++)
    {
        if (result.terms[i].coef != 0)
        {
            final.terms[final.termCount++] = result.terms[i];
        }
    }

    return final;
}

// 다항식을 차수 기준으로 내림차순 정렬하는 함수
void sort_polynomial(Polynomial *poly)
{
    for (int i = 0; i < poly->termCount - 1; i++)
    {
        for (int j = i + 1; j < poly->termCount; j++)
        {
            if (poly->terms[i].exp < poly->terms[j].exp)
            {
                Term temp = poly->terms[i];
                poly->terms[i] = poly->terms[j];
                poly->terms[j] = temp;
            }
        }
    }
}

// 두 다항식을 더하는 함수
Polynomial add(Polynomial a, Polynomial b)
{
    Polynomial result = a;

    // b의 항들을 결과에 추가
    for (int i = 0; i < b.termCount; i++)
    {
        result.terms[result.termCount++] = b.terms[i];
    }

    // 같은 차수끼리 정리하고 정렬
    result = combine_like_terms(result);
    sort_polynomial(&result);
    return result;
}

// 두 다항식을 곱하는 함수
Polynomial multiply(Polynomial a, Polynomial b)
{
    Polynomial result = {.termCount = 0};

    // 각각의 항을 곱해서 새로운 항을 만든다
    for (int i = 0; i < a.termCount; i++)
    {
        for (int j = 0; j < b.termCount; j++)
        {
            Term t;
            t.coef = a.terms[i].coef * b.terms[j].coef;
            t.exp = a.terms[i].exp + b.terms[j].exp;
            result.terms[result.termCount++] = t;
        }
    }

    // 항 정리 및 정렬
    result = combine_like_terms(result);
    sort_polynomial(&result);
    return result;
}

// 다항식을 화면에 출력하는 함수
void print_polynomial(Polynomial poly)
{
    for (int i = 0; i < poly.termCount; i++)
    {
        int coef = poly.terms[i].coef;
        int exp = poly.terms[i].exp;

        if (coef == 0)
            continue;

        // 첫 항이면 부호 출력 안 함 (음수면 -만 붙임)
        if (i == 0)
        {
            if (coef < 0)
                printf("-");
        }
        else
        {
            printf(coef < 0 ? " - " : " + ");
        }

        int absCoef = abs(coef);

        // 항 출력 규칙: 상수항, x, x^n
        if (exp == 0)
        {
            printf("%d", absCoef);
        }
        else if (exp == 1)
        {
            if (absCoef == 1)
                printf("x");
            else
                printf("%dx", absCoef);
        }
        else
        {
            if (absCoef == 1)
                printf("x^%d", exp);
            else
                printf("%dx^%d", absCoef, exp);
        }
    }
    printf("\n");
}

// 메인 함수: 파일에서 입력을 받아서 각 다항식 쌍에 대해 연산 수행
int main()
{
    FILE *fp = fopen("input.txt", "r");
    if (!fp)
    {
        perror("파일 열기 실패");
        return 1;
    }

    int n;
    fscanf(fp, "%d\n", &n); // 첫 줄: 다항식 쌍 개수

    char line1[MAX_POLY_STRING], line2[MAX_POLY_STRING];

    for (int i = 0; i < n; i++)
    {
        fgets(line1, sizeof(line1), fp);
        fgets(line2, sizeof(line2), fp);

        // 줄 끝 개행 제거
        line1[strcspn(line1, "\n")] = '\0';
        line2[strcspn(line2, "\n")] = '\0';

        // 각 줄을 다항식으로 파싱
        Polynomial p1 = parsePolynomial(line1);
        Polynomial p2 = parsePolynomial(line2);

        // 정렬해서 내림차순으로 출력하도록 보장
        sort_polynomial(&p1);
        sort_polynomial(&p2);

        // 덧셈/곱셈 결과 계산
        Polynomial sum = add(p1, p2);
        Polynomial prod = multiply(p1, p2);

        // 결과 출력
        printf("== %d번째 다항식쌍 ==\n", i + 1);
        printf("첫 번째: ");
        print_polynomial(p1);
        printf("두 번째: ");
        print_polynomial(p2);
        printf("덧셈 결과: ");
        print_polynomial(sum);
        printf("곱셈 결과: ");
        print_polynomial(prod);
        printf("\n");
    }

    fclose(fp);
    return 0;
}
