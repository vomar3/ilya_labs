#include <stdio.h>
#include <math.h>

typedef enum ERRORS{
    OK,
    INVALID_INPUT,
    ONE_SIGN,
    PROBLEM_WITH_EPSILON,
} error;

double func1(double x);

double func2(double x);

double func3(double x);

double func4(double x);

double func5(double x);

error dichotomy(double *result, double left, double right, double epsilon, double(*function)(double));

int main() {
    double result = 0.0;

    if (dichotomy(&result, 2, 3, 0.05, func1) == OK) {
        printf("%f\n", result);
    } else if (dichotomy(&result, 2, 3, 0.05, func1) == ONE_SIGN){
        printf("One signs\n");
    } else {
        printf("Problem with epsilon (eps < 0 or eps < 1e-10)\n");
    }
    result = 0.0;

    if (dichotomy(&result, -2, 2, 0.1, func2) == OK) {
        printf("%f\n", result);
    } else if (dichotomy(&result, -2, 2, 0.1, func2) == ONE_SIGN){
        printf("One signs\n");
    } else {
        printf("Problem with epsilon (eps < 0 or eps < 1e-10)\n");
    }
    result = 0.0;

    if (dichotomy(&result, -2, 2, 0.02, func3) == OK) {
        printf("%f\n", result);
    } else if (dichotomy(&result, -2, 2, 0.02, func3) == ONE_SIGN){
        printf("One signs\n");
    } else {
        printf("Problem with epsilon (eps < 0 or eps < 1e-10)\n");
    }
    result = 0.0;

    if (dichotomy(&result, -0.5, 0.5, -2, func4) == OK) {
        printf("%f\n", result);
    } else if (dichotomy(&result, -0.5, 0.5, -2, func4) == ONE_SIGN){
        printf("One signs\n");
    } else {
        printf("Problem with epsilon (eps < 0 or eps < 1e-10)\n");
    }
    result = 0.0;

    if (dichotomy(&result, -5, 2, 0.2, func4) == OK) {
        printf("%f\n", result);
    } else if (dichotomy(&result, -5, 2, 0.2, func4) == ONE_SIGN){
        printf("One signs\n");
    } else {
        printf("Problem with epsilon (eps < 0 or eps < 1e-10)\n");
    }
    result = 0.0;

    if (dichotomy(&result, 5, 8, 0.000000001, func5) == OK) {
        printf("%f\n", result);
    } else if (dichotomy(&result, 5, 8, 0.000000001, func5) == ONE_SIGN){
        printf("One signs\n");
    } else {
        printf("Problem with epsilon (eps < 0 or eps < 1e-10)\n");
    }
    result = 0.0;

    return OK;
}

error dichotomy(double *result, double left, double right, double epsilon, double(*function)(double)) {
    if (epsilon < 1e-10 || epsilon <= 0) {
        return PROBLEM_WITH_EPSILON;
    }
    if (fabs(function(left)) <= epsilon) {
        *result = left;
        return OK;
    }
    if (fabs(function(right)) <= epsilon) {
        *result = right;
        return OK;
    }
    if (function(left) * function(right) > 0) {
        return ONE_SIGN;
    }
    double center = (left + right) / 2;
    while (fabs(function(center)) >= epsilon) {
        if (function(left) * function(right) > 0) {
            return ONE_SIGN;
        }
        center = (left + right) / 2;
        if (function(center) * function(left) < 0) {
            right = center;
        }
        if (function(center) * function(right) < 0) {
            left = center;
        }
    }

    *result = center;
    return OK;
}

double func1(double x) {
    return x * x * x - 2 * x - 5;
}

double func2(double x) {
    return (x - 1) * (x - 1);
}

double func3(double x) {
    return (x - 1) * (x - 1) * (x - 1);
}

double func4(double x) {
    return sin(x) + 1;
}

double func5(double x) {
    return sin(x);
}