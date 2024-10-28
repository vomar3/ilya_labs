#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>

typedef enum ERRORS{
    OK,
    INVALID_INPUT,
    MEMORY_ERROR,
} error;

/*
 * 1) Пока мы полностью не раскроем наш многочлен:
 * - вычисляем f(a)
 * - берем производную по x
 * 2) gi = f^(i) (a) / i!
*/

double calc_equation(const double *coefs, int degree, double point);

double calc_df(double *coefs, int degree, double point);

error calculating_polynomial_coefs(double *array_answers, double eps, int degree, double point, ...);

int main() {

    int degree = 4;
    double epsilon = 1e-6;
    double a = 3.0;
    double *answers = (double *) malloc((degree + 1) * sizeof(double));
    if (answers == NULL) {
        printf("Memory error\n");
        free(answers);
        return MEMORY_ERROR;
    }

    switch (calculating_polynomial_coefs(answers, epsilon, degree, a, -2.0, 1.0, -3.0, 0.0, 1.0)) {
        case MEMORY_ERROR:
            printf("Memory error\n");
            free(answers);
            return MEMORY_ERROR;
        case INVALID_INPUT:
            printf("Invalid input\n");
            free(answers);
            return INVALID_INPUT;
        case OK:
            printf("Answer f(x-a):\ngi = f^(i)(a) / i!\n");
            for (int i = 0; i <= degree; ++i) {
                printf("g%d = %lf\n", i, answers[i]);
            }
    }

    printf("\n");
    for (int x = 0; x < degree + 1; x++)
    {
        double f_x = -2.0 + 1.0 * x - 3.0 * x * x + 0.0 * x * x * x + 1.0 * x * x * x * x;
        printf("f(%d) = %lf\n", x, f_x);
    }
    printf("\n");

    for (int x = 0; x < degree + 1; x++)
    {
        double g_x = 0.0;
        for (int i = 0; i < degree + 1; i++)
        {
            g_x += answers[i] * pow(x - 3.0, i);
        }
        printf("g(%d) = %lf\n", x, g_x);
    }

    free(answers);
    return OK;
}

error calculating_polynomial_coefs(double *array_answers, double eps, int degree, double point, ...) {
    if (degree < 0 || eps < 1e-10) return INVALID_INPUT;

    va_list args;
    va_start(args, point);
    double elements;
    double *coefs = (double *) malloc ((degree + 1) * sizeof(double));

    for (int count = 0; count < degree + 1; ++count) {
        elements = va_arg(args, double);
        coefs[count] = elements;
    }
    coefs[degree + 1] = '\0';

    array_answers[0] = calc_equation(coefs, degree, point);

    double factorial = 1;
    for (int i = 1; i < degree + 1; ++i) {
        array_answers[i] = calc_df(coefs, degree + 1 - i, point) / factorial;
        factorial *= (i + 1);
    }

    free(coefs);
    va_end(args);
    return OK;
}

double calc_equation(const double *coefs, int degree, double point) {
    double answer = 0, multi = 1;
    for (int i = 0; i < degree + 1; ++i) {
        answer += coefs[i] * multi;
        multi *= point;
    }
    return answer;
}

double calc_df(double *coefs, int degree, double point) {
    for (int i = 0; i < degree; ++i) {
        coefs[i] = coefs[i + 1] * (i + 1);
    }
    //coefs[degree] = '\0';
    return calc_equation(coefs, degree - 1, point);
}