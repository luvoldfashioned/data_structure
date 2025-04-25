#include <stdio.h>  // 표준 입출력 함수 사용을 위한 헤더
#include <stdlib.h> // 표준 라이브러리 (malloc, exit 등)
#include <string.h> // 문자열 처리 함수들 사용 (strcpy, strlen 등)
#include <ctype.h>  // 문자 검사 함수 (isdigit 등)

#define MAX_POLY 1001 // 최대 다항식 차수: x^1000까지 처리하기 위해 크기를 1001로 설정

// 다항식을 표현하는 구조체 정의
typedef struct
{
    int coef[MAX_POLY]; // coef[i]는 x^i의 계수를 의미함
} Polynomial;

// 다항식 구조체를 0으로 초기화하는 함수
void init_poly(Polynomial *p)
{
    for (int i = 0; i < MAX_POLY; ++i)
        p->coef[i] = 0; // 모든 계수를 0으로 초기화
}

// 입력 문자열에서 공백을 제거하고 '**'를 '^'로 바꾸는 함수
void clean_string(char *s)
{
    char temp[256];
    int j = 0;
    for (int i = 0; s[i]; ++i)
    {
        if (s[i] == '*' && s[i + 1] == '*')
        { // '**'를 '^'로 바꾸기
            temp[j++] = '^';
            i++; // '*' 하나 더 넘기기
        }
        else if (s[i] != ' ')
        { // 공백은 제거
            temp[j++] = s[i];
        }
    }
    temp[j] = '\0';  // 문자열 끝 표시
    strcpy(s, temp); // 정리된 문자열을 원래 s에 덮어쓰기
}

// 문자열을 파싱하여 다항식 구조체에 저장하는 함수
void parse_poly(char *s, Polynomial *p)
{
    init_poly(p);               // 다항식 초기화
    int i = 0, len = strlen(s); // 문자열 길이

    while (i < len)
    {
        int sign = 1; // 기본 부호는 +라고 가정

        // 부호 처리
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

        // 계수 읽기
        int coef = 0, coef_set = 0;
        while (i < len && isdigit(s[i]))
        {
            coef = coef * 10 + (s[i] - '0'); // 숫자 누적
            i++;
            coef_set = 1;
        }
        if (!coef_set)
            coef = 1; // 계수가 생략된 경우 1로 처리

        // x가 등장하는 경우
        if (i < len && s[i] == 'x')
        {
            i++;
            int exp = 1; // 기본 차수는 1

            if (i < len && s[i] == '^')
            {
                i++;
                exp = 0;
                while (i < len && isdigit(s[i]))
                {
                    exp = exp * 10 + (s[i] - '0'); // 차수 누적
                    i++;
                }
            }

            // 해당 차수에 계수 누적
            p->coef[exp] += coef * sign;
        }
        else
        {
            // 상수항 처리 (x 없음)
            p->coef[0] += coef * sign;
        }
    }
}

// 두 다항식을 더하는 함수
Polynomial add_poly(Polynomial *a, Polynomial *b)
{
    Polynomial res;
    init_poly(&res); // 결과도 초기화
    for (int i = 0; i < MAX_POLY; ++i)
        res.coef[i] = a->coef[i] + b->coef[i]; // 같은 차수끼리 더함
    return res;
}

// 두 다항식을 곱하는 함수
Polynomial mul_poly(Polynomial *a, Polynomial *b)
{
    Polynomial res;
    init_poly(&res); // 결과 초기화
    for (int i = 0; i < MAX_POLY; ++i)
    {
        if (a->coef[i] == 0)
            continue; // 0 계수는 건너뜀
        for (int j = 0; j < MAX_POLY; ++j)
        {
            if (b->coef[j] == 0)
                continue;
            if (i + j < MAX_POLY)
                res.coef[i + j] += a->coef[i] * b->coef[j]; // 차수는 더하고, 계수는 곱해서 누적
        }
    }
    return res;
}

// 다항식을 내림차순으로 출력하는 함수
void print_poly(Polynomial *p)
{
    int started = 0; // 앞에 출력된 항이 있는지 체크
    for (int i = MAX_POLY - 1; i >= 0; --i)
    {
        int c = p->coef[i];
        if (c == 0)
            continue;

        if (started)
            printf(" %c ", c > 0 ? '+' : '-'); // 부호 출력
        else if (c < 0)
            printf("-"); // 첫 항이 음수일 경우

        if (abs(c) != 1 || i == 0)
            printf("%d", abs(c)); // 계수 출력 (1은 생략, 상수항은 무조건 출력)
        if (i >= 1)
            printf("x"); // x 출력
        if (i > 1)
            printf("^%d", i); // 차수 출력

        started = 1;
    }

    if (!started)
        printf("0"); // 0 다항식이면 0 출력
    printf("\n");
}

// 프로그램의 메인 함수
int main()
{
    FILE *fp = fopen("input.txt", "r"); // 파일 열기
    if (!fp)
    {
        printf("파일 열기 실패\n");
        return 1; // 실패 시 종료
    }

    int n;
    fscanf(fp, "%d\n", &n); // 다항식 쌍의 개수 읽기

    char line1[256], line2[256]; // 다항식 문자열을 저장할 배열

    for (int i = 0; i < n; ++i)
    {
        fgets(line1, sizeof(line1), fp); // 첫 번째 다항식
        fgets(line2, sizeof(line2), fp); // 두 번째 다항식

        // 개행 문자 제거
        line1[strcspn(line1, "\n")] = 0;
        line2[strcspn(line2, "\n")] = 0;

        // 문자열 정리: 공백 제거, '**' → '^'
        clean_string(line1);
        clean_string(line2);

        Polynomial A, B;
        parse_poly(line1, &A); // 문자열 → 다항식 구조체
        parse_poly(line2, &B);

        Polynomial sum = add_poly(&A, &B);  // 덧셈 결과 계산
        Polynomial prod = mul_poly(&A, &B); // 곱셈 결과 계산

        // 결과 출력
        printf("== %d번째 다항식 쌍 ==\n", i + 1);
        printf("첫 번째: ");
        print_poly(&A);
        printf("두 번째: ");
        print_poly(&B);
        printf("덧셈 결과: ");
        print_poly(&sum);
        printf("곱셈 결과: ");
        print_poly(&prod);
        printf("\n");
    }

    fclose(fp); // 파일 닫기
    return 0;   // 정상 종료
}
