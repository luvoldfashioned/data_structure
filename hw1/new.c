#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TERMS 100       // �ϳ��� ���׽��� ���� �� �ִ� �ִ� �� ����
#define MAX_POLY_STRING 256 // �� �ٿ� �Էµ� �� �ִ� �ִ� ���׽� ����

// ������ ���� ������ ����ü: ���(coef)�� ����(exp)�� ����
typedef struct
{
    int coef; // ���
    int exp;  // ���� (exponent)
} Term;

// ���׽� ����ü: ���� ���� ���� ���� �� ����
typedef struct
{
    Term terms[MAX_TERMS]; // �׵��� �迭�� ����
    int termCount;         // ���� ����� ���� ����
} Polynomial;

// ���ڿ��� �� ���׽��� �Ľ��Ͽ� Polynomial ����ü�� ��ȯ�ϴ� �Լ�
Polynomial parsePolynomial(char *str)
{
    Polynomial poly = {.termCount = 0}; // ��� ���׽� ����ü �ʱ�ȭ
    int i = 0, sign = 1;

    while (str[i])
    {
        int coef = 0, exp = 0;
        int coefSet = 0; // ����� ��������� �����Ǿ����� Ȯ��
        sign = 1;

        // ���� ��ŵ
        while (isspace(str[i]))
            i++;

        // ��ȣ ó��
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

        // �ٽ� ���� ��ŵ
        while (isspace(str[i]))
            i++;

        // ���ڰ� �ִ� ���: ��� ����
        if (isdigit(str[i]))
        {
            while (isdigit(str[i]))
            {
                coef = coef * 10 + (str[i++] - '0');
            }
            coefSet = 1;
        }

        // x�� �ִ� ���
        if (str[i] == 'x')
        {
            i++;
            if (!coefSet)
                coef = 1; // ����� �����Ǿ����� 1�� ����

            // x^n �Ǵ� x**n ������ ���� ó��
            if (str[i] == '^' || (str[i] == '*' && str[i + 1] == '*'))
            {
                if (str[i] == '*')
                    i += 2; // '**' �ǳʶٱ�
                else
                    i++; // '^' �ǳʶٱ�
                exp = 0;
                while (isdigit(str[i]))
                {
                    exp = exp * 10 + (str[i++] - '0');
                }
            }
            else
            {
                exp = 1; // �׳� x�� ������ ������ 1
            }
        }
        else
        {
            if (!coefSet)
                coef = 1; // �׳� ���ڵ� ������ ����� 1
            exp = 0;      // �����
        }

        // �� ����
        poly.terms[poly.termCount].coef = coef * sign;
        poly.terms[poly.termCount].exp = exp;
        poly.termCount++;
    }

    return poly;
}

// ���� �������� ����� ���ؼ� ���� �����ϴ� �Լ�
Polynomial combine_like_terms(Polynomial poly)
{
    Polynomial result = {.termCount = 0};

    // ���� �׵��� �ϳ��� Ȯ���ϸ鼭 ���� ������ ���� ��ħ
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
        // ������ ���� �߰�
        if (!found)
        {
            result.terms[result.termCount++] = poly.terms[i];
        }
    }

    // ����� 0�� ���� ����
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

// ���׽��� ���� �������� �������� �����ϴ� �Լ�
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

// �� ���׽��� ���ϴ� �Լ�
Polynomial add(Polynomial a, Polynomial b)
{
    Polynomial result = a;

    // b�� �׵��� ����� �߰�
    for (int i = 0; i < b.termCount; i++)
    {
        result.terms[result.termCount++] = b.terms[i];
    }

    // ���� �������� �����ϰ� ����
    result = combine_like_terms(result);
    sort_polynomial(&result);
    return result;
}

// �� ���׽��� ���ϴ� �Լ�
Polynomial multiply(Polynomial a, Polynomial b)
{
    Polynomial result = {.termCount = 0};

    // ������ ���� ���ؼ� ���ο� ���� �����
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

    // �� ���� �� ����
    result = combine_like_terms(result);
    sort_polynomial(&result);
    return result;
}

// ���׽��� ȭ�鿡 ����ϴ� �Լ�
void print_polynomial(Polynomial poly)
{
    for (int i = 0; i < poly.termCount; i++)
    {
        int coef = poly.terms[i].coef;
        int exp = poly.terms[i].exp;

        if (coef == 0)
            continue;

        // ù ���̸� ��ȣ ��� �� �� (������ -�� ����)
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

        // �� ��� ��Ģ: �����, x, x^n
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

// ���� �Լ�: ���Ͽ��� �Է��� �޾Ƽ� �� ���׽� �ֿ� ���� ���� ����
int main()
{
    FILE *fp = fopen("input.txt", "r");
    if (!fp)
    {
        perror("���� ���� ����");
        return 1;
    }

    int n;
    fscanf(fp, "%d\n", &n); // ù ��: ���׽� �� ����

    char line1[MAX_POLY_STRING], line2[MAX_POLY_STRING];

    for (int i = 0; i < n; i++)
    {
        fgets(line1, sizeof(line1), fp);
        fgets(line2, sizeof(line2), fp);

        // �� �� ���� ����
        line1[strcspn(line1, "\n")] = '\0';
        line2[strcspn(line2, "\n")] = '\0';

        // �� ���� ���׽����� �Ľ�
        Polynomial p1 = parsePolynomial(line1);
        Polynomial p2 = parsePolynomial(line2);

        // �����ؼ� ������������ ����ϵ��� ����
        sort_polynomial(&p1);
        sort_polynomial(&p2);

        // ����/���� ��� ���
        Polynomial sum = add(p1, p2);
        Polynomial prod = multiply(p1, p2);

        // ��� ���
        printf("== %d��° ���׽Ľ� ==\n", i + 1);
        printf("ù ��°: ");
        print_polynomial(p1);
        printf("�� ��°: ");
        print_polynomial(p2);
        printf("���� ���: ");
        print_polynomial(sum);
        printf("���� ���: ");
        print_polynomial(prod);
        printf("\n");
    }

    fclose(fp);
    return 0;
}
