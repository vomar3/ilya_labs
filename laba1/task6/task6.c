#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int str_to_double(char *str, double *number);

double integral_a(double x);

double integral_b(double x);

double integral_c(double x);

double integral_d(double x);

double integrate(double epsilon, double (*integrate_function)(double));

double rectangle_method(int n, double epsilon, double (*integrate_function)(double));

enum ERRORS{
    OK,
    INVALID_INPUT,
    PROBLEMS_WITH_EPS,
};

int main(int args, char *argv[]) {

    if (args > 2) {
        printf("Invalid input\n");
        return INVALID_INPUT;
    }

    double epsilon;
    if (str_to_double(argv[1], &epsilon) == -2) {
        printf("Problems with epsilon\n");
        return PROBLEMS_WITH_EPS;
    }

    if (epsilon <= 0) {
        printf("Problems with epsilon\n");
        return PROBLEMS_WITH_EPS;
    }

    double result_a = integrate(epsilon, integral_a);
    printf("a: %lf\n", result_a);

    double result_b = integrate(epsilon, integral_b);
    printf("b: %lf\n", result_b);

    double result_c = integrate(epsilon, integral_c);
    printf("c: %lf\n", result_c);

    double result_d = integrate(epsilon, integral_d);
    printf("d: %lf\n", result_d);

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

double integral_a(double x) {
    return log(1 + x) / x;
}

double integral_b(double x) {
    return exp(-x * x / 2);
}

double integral_c(double x) {
    return log(1 / (1 - x));
}

double integral_d(double x) {
    return pow(x, x);
}

double integrate(double epsilon, double (*integrate_function)(double)) {
    double last_result = 1.0, result = 0.0;
    int number = 1;

    while (fabs(result - last_result) >= epsilon) {
        last_result = result;
        result = rectangle_method(number, epsilon, integrate_function);
        number *= 2;
    }

    return result;
}

double rectangle_method(int number, double epsilon, double (*integrate_function)(double)) {
    double h = 1.0 / number, sum = 0.0, x, i;
    for (i = epsilon; i < number - epsilon; i++) {
        x = h * i;
        sum += integrate_function(x);
    }
    return h * sum;
}

