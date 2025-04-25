#include <stdio.h>  // ǥ�� ����� �Լ� ����� ���� ���
#include <stdlib.h> // ǥ�� ���̺귯�� (malloc, exit ��)
#include <string.h> // ���ڿ� ó�� �Լ��� ��� (strcpy, strlen ��)
#include <ctype.h>  // ���� �˻� �Լ� (isdigit ��)

#define MAX_POLY 1001 // �ִ� ���׽� ����: x^1000���� ó���ϱ� ���� ũ�⸦ 1001�� ����

// ���׽��� ǥ���ϴ� ����ü ����
typedef struct
{
    int coef[MAX_POLY]; // coef[i]�� x^i�� ����� �ǹ���
} Polynomial;

// ���׽� ����ü�� 0���� �ʱ�ȭ�ϴ� �Լ�
void init_poly(Polynomial *p)
{
    for (int i = 0; i < MAX_POLY; ++i)
        p->coef[i] = 0; // ��� ����� 0���� �ʱ�ȭ
}

// �Է� ���ڿ����� ������ �����ϰ� '**'�� '^'�� �ٲٴ� �Լ�
void clean_string(char *s)
{
    char temp[256];
    int j = 0;
    for (int i = 0; s[i]; ++i)
    {
        if (s[i] == '*' && s[i + 1] == '*')
        { // '**'�� '^'�� �ٲٱ�
            temp[j++] = '^';
            i++; // '*' �ϳ� �� �ѱ��
        }
        else if (s[i] != ' ')
        { // ������ ����
            temp[j++] = s[i];
        }
    }
    temp[j] = '\0';  // ���ڿ� �� ǥ��
    strcpy(s, temp); // ������ ���ڿ��� ���� s�� �����
}

// ���ڿ��� �Ľ��Ͽ� ���׽� ����ü�� �����ϴ� �Լ�
void parse_poly(char *s, Polynomial *p)
{
    init_poly(p);               // ���׽� �ʱ�ȭ
    int i = 0, len = strlen(s); // ���ڿ� ����

    while (i < len)
    {
        int sign = 1; // �⺻ ��ȣ�� +��� ����

        // ��ȣ ó��
        if (s[i] == '+')
        {
            sign = 1;
            i++;
        }
        else if (s[i] == '-')
        {
            sign = -1;
            i++;
        }

        // ��� �б�
        int coef = 0, coef_set = 0;
        while (i < len && isdigit(s[i]))
        {
            coef = coef * 10 + (s[i] - '0'); // ���� ����
            i++;
            coef_set = 1;
        }
        if (!coef_set)
            coef = 1; // ����� ������ ��� 1�� ó��

        // x�� �����ϴ� ���
        if (i < len && s[i] == 'x')
        {
            i++;
            int exp = 1; // �⺻ ������ 1

            if (i < len && s[i] == '^')
            {
                i++;
                exp = 0;
                while (i < len && isdigit(s[i]))
                {
                    exp = exp * 10 + (s[i] - '0'); // ���� ����
                    i++;
                }
            }

            // �ش� ������ ��� ����
            p->coef[exp] += coef * sign;
        }
        else
        {
            // ����� ó�� (x ����)
            p->coef[0] += coef * sign;
        }
    }
}

// �� ���׽��� ���ϴ� �Լ�
Polynomial add_poly(Polynomial *a, Polynomial *b)
{
    Polynomial res;
    init_poly(&res); // ����� �ʱ�ȭ
    for (int i = 0; i < MAX_POLY; ++i)
        res.coef[i] = a->coef[i] + b->coef[i]; // ���� �������� ����
    return res;
}

// �� ���׽��� ���ϴ� �Լ�
Polynomial mul_poly(Polynomial *a, Polynomial *b)
{
    Polynomial res;
    init_poly(&res); // ��� �ʱ�ȭ
    for (int i = 0; i < MAX_POLY; ++i)
    {
        if (a->coef[i] == 0)
            continue; // 0 ����� �ǳʶ�
        for (int j = 0; j < MAX_POLY; ++j)
        {
            if (b->coef[j] == 0)
                continue;
            if (i + j < MAX_POLY)
                res.coef[i + j] += a->coef[i] * b->coef[j]; // ������ ���ϰ�, ����� ���ؼ� ����
        }
    }
    return res;
}

// ���׽��� ������������ ����ϴ� �Լ�
void print_poly(Polynomial *p)
{
    int started = 0; // �տ� ��µ� ���� �ִ��� üũ
    for (int i = MAX_POLY - 1; i >= 0; --i)
    {
        int c = p->coef[i];
        if (c == 0)
            continue;

        if (started)
            printf(" %c ", c > 0 ? '+' : '-'); // ��ȣ ���
        else if (c < 0)
            printf("-"); // ù ���� ������ ���

        if (abs(c) != 1 || i == 0)
            printf("%d", abs(c)); // ��� ��� (1�� ����, ������� ������ ���)
        if (i >= 1)
            printf("x"); // x ���
        if (i > 1)
            printf("^%d", i); // ���� ���

        started = 1;
    }

    if (!started)
        printf("0"); // 0 ���׽��̸� 0 ���
    printf("\n");
}

// ���α׷��� ���� �Լ�
int main()
{
    FILE *fp = fopen("input.txt", "r"); // ���� ����
    if (!fp)
    {
        printf("���� ���� ����\n");
        return 1; // ���� �� ����
    }

    int n;
    fscanf(fp, "%d\n", &n); // ���׽� ���� ���� �б�

    char line1[256], line2[256]; // ���׽� ���ڿ��� ������ �迭

    for (int i = 0; i < n; ++i)
    {
        fgets(line1, sizeof(line1), fp); // ù ��° ���׽�
        fgets(line2, sizeof(line2), fp); // �� ��° ���׽�

        // ���� ���� ����
        line1[strcspn(line1, "\n")] = 0;
        line2[strcspn(line2, "\n")] = 0;

        // ���ڿ� ����: ���� ����, '**' �� '^'
        clean_string(line1);
        clean_string(line2);

        Polynomial A, B;
        parse_poly(line1, &A); // ���ڿ� �� ���׽� ����ü
        parse_poly(line2, &B);

        Polynomial sum = add_poly(&A, &B);  // ���� ��� ���
        Polynomial prod = mul_poly(&A, &B); // ���� ��� ���

        // ��� ���
        printf("== %d��° ���׽� �� ==\n", i + 1);
        printf("ù ��°: ");
        print_poly(&A);
        printf("�� ��°: ");
        print_poly(&B);
        printf("���� ���: ");
        print_poly(&sum);
        printf("���� ���: ");
        print_poly(&prod);
        printf("\n");
    }

    fclose(fp); // ���� �ݱ�
    return 0;   // ���� ����
}
