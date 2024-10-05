#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

int str_to_double(char *str, double *number);

double teylor_a(double x, double epsilon);

double teylor_b(double x, double epsilon);

double teylor_c(double x, double epsilon);

double teylor_d(double x, double epsilon);

int there_is_inf(double x);

enum ERRORS{
    OK,
    INVALID_INPUT,
    PROBLEMS_WITH_EPS,
};

int main(int args, char *argv[]) {

    if (args != 3) {
        printf("Invalid input\n");
        return INVALID_INPUT;
    }

    double x;
    if (str_to_double(argv[1], &x) == -2) {
        printf("Invalid input\n");
        return INVALID_INPUT;
    }

    double epsilon;
    if (str_to_double(argv[2], &epsilon) == -2 || epsilon <= 0 || epsilon > 1) {
        printf("Problems with epsilon\n");
        return PROBLEMS_WITH_EPS;
    }

    if (teylor_a(x, epsilon) != epsilon) printf("A: %lf\n", teylor_a(x, epsilon));
    else printf("A: Invalid input for that function\n");
    if (teylor_b(x, epsilon) != epsilon) printf("B: %lf\n", teylor_b(x, epsilon));
    else printf("B: Invalid input for that function\n");
    if (teylor_c(x, epsilon) != epsilon) printf("C: %lf\n", teylor_c(x, epsilon));
    else printf("C: Invalid input for that function\n");
    if (teylor_d(x, epsilon) != epsilon) printf("D: %lf\n", teylor_d(x, epsilon));
    else printf("D: Invalid input for that function\n");

    return OK;
}

int str_to_double(char *str, double *number) {
    if (strlen(str) >= 10) {
        return -2;
    }

    int dots = 0;
    for (int i = 0; i < strlen(str); ++i) {
        if ((str[i] < '0' || str[i] > '9') && str[i] != '.') {
            return -2;
        }
        else if (str[i] == '.') {
            dots++;
        }
    }

    if (dots != 1) return -2;

    char *end;
    *number = strtod(str, &end);

    if (*number == HUGE_VAL || *number == -HUGE_VAL) {
        return -2;
    } else if (*number == 0 && end == str) {
        return -2;
    } else if (*end != '\0') {
        return -2;
    } else {
        return 0;
    }

}

double teylor_a(double x, double epsilon) {
    int n = 0;
    double result = 0, iter_answer = 1.0;

    while (fabs(iter_answer) >= epsilon) {
        if (n == INT_MAX) return epsilon;
        result += iter_answer;
        ++n;
        iter_answer *= x / n;
    }

    return result;
}

double teylor_b(double x, double epsilon) {
    int n = 0;
    double result = 0, iter_answer = 1.0;

    while (fabs(iter_answer) >= epsilon) {
        if (n == INT_MAX) return epsilon;
        result += iter_answer;
        ++n;
        iter_answer *= -1.0 * x * x / ((2 * n) * (2 * n - 1.0));
    }

    return result;
}

double teylor_c(double x, double epsilon) {
    int n = 0;
    double result = 0, iter_answer = 1.0;

    while (fabs(iter_answer) >= epsilon) {
        if (n == INT_MAX) return epsilon;
        result += iter_answer;
        ++n;
        iter_answer *= 9.0 * x * x * n * n / (3.0 * n * (3.0 * n - 3.0) + 2.0);

        if (epsilon + iter_answer >= 1.0) return INFINITY;
    }

    return result;
}

double teylor_d(double x, double epsilon) {
    int n = 0;
    if (fabs(x) >= 1) return INFINITY;
    double result = 0, iter_answer = -1.0 * x * x / 2.0;

    while (fabs(iter_answer) >= epsilon) {
        if (there_is_inf(result) == 1 || there_is_inf(iter_answer) == 1) {
            return INFINITY;
        }
        if (n == INT_MAX) return epsilon;
        result += iter_answer;
        ++n;
        iter_answer *= (-1.0 * x * x * (2.0 * n - 1.0)) / (2.0 * n);
    }

    return result;
}

int there_is_inf(double x) {
    if (isinf(x) || fabs(x) > DBL_MAX) {
        return 1;
    }
    return 0;
}