#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TERMS 100

// 항 하나를 표현하는 구조체: 계수와 차수
typedef struct {
    int coef; // 계수
    int exp;  // 차수
} Term;

// 다항식을 표현하는 구조체: Term 배열과 항 개수
typedef struct {
    Term terms[MAX_TERMS];
    int num_terms;
} Polynomial;

// 다항식 항들을 차수 기준으로 내림차순 정렬하는 함수
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

// 같은 차수를 가진 항들을 합치는 함수
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

// 다항식을 출력하는 함수
void print_polynomial(Polynomial* poly) {
    sort_polynomial(poly);
    combine_like_terms(poly);

    int first = 1; // 첫 항인지 확인 (부호 출력을 위함)

    for (int i = 0; i < poly->num_terms; i++) {
        int c = poly->terms[i].coef;
        int e = poly->terms[i].exp;

        if (c == 0) continue; // 계수가 0이면 출력 안 함

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

// 문자열을 받아서 다항식을 파싱하는 함수
void parse_polynomial(char* str, Polynomial* poly) {
    poly->num_terms = 0;

    char* p = str;
    while (*p != '\0') {
        int sign = 1;
        int coef = 0, exp = 0;
        int coef_set = 0;

        // 공백 건너뜀
        while (*p == ' ') p++;

        // 부호 처리
        if (*p == '+') {
            sign = 1;
            p++;
        } else if (*p == '-') {
            sign = -1;
            p++;
        }

        // 계수 파싱 (숫자면 읽음)
        while (isdigit(*p)) {
            coef = coef * 10 + (*p - '0');
            p++;
            coef_set = 1;
        }

        // 'x'가 나오면 차수도 있는 항
        if (*p == 'x' || *p == 'X') {
            p++; // 'x' 건너뜀

            // 계수가 없으면 기본값 1
            if (!coef_set) coef = 1;

            // '**' 또는 '^' 형태로 차수 파싱
            if (*p == '*' && *(p + 1) == '*') {
                p += 2;
            } else if (*p == '^') {
                p++;
            }

            // 차수 읽기
            while (isdigit(*p)) {
                exp = exp * 10 + (*p - '0');
                p++;
            }

        } else {
            // 'x'가 없으면 상수항
            if (!coef_set) coef = 1;
            exp = 0;
        }

        // 항 추가
        poly->terms[poly->num_terms].coef = sign * coef;
        poly->terms[poly->num_terms].exp = exp;
        poly->num_terms++;
    }
}

// 다항식 덧셈 함수
void add_polynomials(Polynomial* a, Polynomial* b, Polynomial* result) {
    result->num_terms = 0;

    // a 항 복사
    for (int i = 0; i < a->num_terms; i++) {
        result->terms[result->num_terms++] = a->terms[i];
    }

    // b 항도 복사
    for (int i = 0; i < b->num_terms; i++) {
        result->terms[result->num_terms++] = b->terms[i];
    }

    combine_like_terms(result);
    sort_polynomial(result);
}

// 다항식 곱셈 함수
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
        perror("파일 열기 실패");
        return 1;
    }

    int n;
    fscanf(fp, "%d\n", &n); // 다항식 쌍 개수 읽기

    char line1[256], line2[256];
    for (int i = 0; i < n; i++) {
        fgets(line1, sizeof(line1), fp);
        fgets(line2, sizeof(line2), fp);

        // 줄 끝 개행 제거
        line1[strcspn(line1, "\n")] = 0;
        line2[strcspn(line2, "\n")] = 0;

        Polynomial A, B, sum, product;

        parse_polynomial(line1, &A);
        parse_polynomial(line2, &B);

        add_polynomials(&A, &B, &sum);
        multiply_polynomials(&A, &B, &product);

        printf("덧셈 결과: ");
        print_polynomial(&sum);

        printf("곱셈 결과: ");
        print_polynomial(&product);

        printf("\n");
    }

    fclose(fp);
    return 0;
}

