#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TERMS 100

// �� �ϳ��� ǥ���ϴ� ����ü: ����� ����
typedef struct {
    int coef; // ���
    int exp;  // ����
} Term;

// ���׽��� ǥ���ϴ� ����ü: Term �迭�� �� ����
typedef struct {
    Term terms[MAX_TERMS];
    int num_terms;
} Polynomial;

// ���׽� �׵��� ���� �������� �������� �����ϴ� �Լ�
void sort_polynomial(Polynomial* poly) {
    for (int i = 0; i < poly->num_terms - 1; i++) {
        for (int j = i + 1; j < poly->num_terms; j++) {
            if (poly->terms[i].exp < poly->terms[j].exp) {
                Term temp = poly->terms[i];
                poly->terms[i] = poly->terms[j];
                poly->terms[j] = temp;
            }
        }
    }
}

// ���� ������ ���� �׵��� ��ġ�� �Լ�
void combine_like_terms(Polynomial* poly) {
    for (int i = 0; i < poly->num_terms; i++) {
        for (int j = i + 1; j < poly->num_terms;) {
            if (poly->terms[i].exp == poly->terms[j].exp) {
                poly->terms[i].coef += poly->terms[j].coef;
                for (int k = j; k < poly->num_terms - 1; k++) {
                    poly->terms[k] = poly->terms[k + 1];
                }
                poly->num_terms--;
            } else {
                j++;
            }
        }
    }
}

// ���׽��� ����ϴ� �Լ�
void print_polynomial(Polynomial* poly) {
    sort_polynomial(poly);
    combine_like_terms(poly);

    int first = 1; // ù ������ Ȯ�� (��ȣ ����� ����)

    for (int i = 0; i < poly->num_terms; i++) {
        int c = poly->terms[i].coef;
        int e = poly->terms[i].exp;

        if (c == 0) continue; // ����� 0�̸� ��� �� ��

        if (!first) {
            printf(" %c ", (c > 0) ? '+' : '-');
        } else {
            if (c < 0) printf("-");
        }

        int abs_coef = abs(c);

        if (e == 0) {
            printf("%d", abs_coef);
        } else if (abs_coef == 1) {
            printf("x");
        } else {
            printf("%dx", abs_coef);
        }

        if (e > 1) {
            printf("^%d", e);
        }

        first = 0;
    }

    if (first) {
        printf("0");
    }

    printf("\n");
}

// ���ڿ��� �޾Ƽ� ���׽��� �Ľ��ϴ� �Լ�
void parse_polynomial(char* str, Polynomial* poly) {
    poly->num_terms = 0;

    char* p = str;
    while (*p != '\0') {
        int sign = 1;
        int coef = 0, exp = 0;
        int coef_set = 0;

        // ���� �ǳʶ�
        while (*p == ' ') p++;

        // ��ȣ ó��
        if (*p == '+') {
            sign = 1;
            p++;
        } else if (*p == '-') {
            sign = -1;
            p++;
        }

        // ��� �Ľ� (���ڸ� ����)
        while (isdigit(*p)) {
            coef = coef * 10 + (*p - '0');
            p++;
            coef_set = 1;
        }

        // 'x'�� ������ ������ �ִ� ��
        if (*p == 'x' || *p == 'X') {
            p++; // 'x' �ǳʶ�

            // ����� ������ �⺻�� 1
            if (!coef_set) coef = 1;

            // '**' �Ǵ� '^' ���·� ���� �Ľ�
            if (*p == '*' && *(p + 1) == '*') {
                p += 2;
            } else if (*p == '^') {
                p++;
            }

            // ���� �б�
            while (isdigit(*p)) {
                exp = exp * 10 + (*p - '0');
                p++;
            }

        } else {
            // 'x'�� ������ �����
            if (!coef_set) coef = 1;
            exp = 0;
        }

        // �� �߰�
        poly->terms[poly->num_terms].coef = sign * coef;
        poly->terms[poly->num_terms].exp = exp;
        poly->num_terms++;
    }
}

// ���׽� ���� �Լ�
void add_polynomials(Polynomial* a, Polynomial* b, Polynomial* result) {
    result->num_terms = 0;

    // a �� ����
    for (int i = 0; i < a->num_terms; i++) {
        result->terms[result->num_terms++] = a->terms[i];
    }

    // b �׵� ����
    for (int i = 0; i < b->num_terms; i++) {
        result->terms[result->num_terms++] = b->terms[i];
    }

    combine_like_terms(result);
    sort_polynomial(result);
}

// ���׽� ���� �Լ�
void multiply_polynomials(Polynomial* a, Polynomial* b, Polynomial* result) {
    result->num_terms = 0;

    for (int i = 0; i < a->num_terms; i++) {
        for (int j = 0; j < b->num_terms; j++) {
            int coef = a->terms[i].coef * b->terms[j].coef;
            int exp = a->terms[i].exp + b->terms[j].exp;

            result->terms[result->num_terms].coef = coef;
            result->terms[result->num_terms].exp = exp;
            result->num_terms++;
        }
    }

    combine_like_terms(result);
    sort_polynomial(result);
}

int main() {
    FILE* fp = fopen("input.txt", "r");
    if (!fp) {
        perror("���� ���� ����");
        return 1;
    }

    int n;
    fscanf(fp, "%d\n", &n); // ���׽� �� ���� �б�

    char line1[256], line2[256];
    for (int i = 0; i < n; i++) {
        fgets(line1, sizeof(line1), fp);
        fgets(line2, sizeof(line2), fp);

        // �� �� ���� ����
        line1[strcspn(line1, "\n")] = 0;
        line2[strcspn(line2, "\n")] = 0;

        Polynomial A, B, sum, product;

        parse_polynomial(line1, &A);
        parse_polynomial(line2, &B);

        add_polynomials(&A, &B, &sum);
        multiply_polynomials(&A, &B, &product);

        printf("���� ���: ");
        print_polynomial(&sum);

        printf("���� ���: ");
        print_polynomial(&product);

        printf("\n");
    }

    fclose(fp);
    return 0;
}

