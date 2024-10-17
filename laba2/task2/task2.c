#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdarg.h>


typedef enum ERRORS{
    OK,
    NUMBER_OVERFLOW,
    INCORRECT_INPUT,
} error;

error check_overflow(const double *number);

error check_denormalize(const double *number);

error fast_pow(double base, int degree, double *fast_pow_answer);

error geometric_mean(double *result, unsigned int count, ...);

int main() {

    double fast_pow_answer = 1.0;
    double result = 1.0;

    /*if (fast_pow(11, 23, &fast_pow_answer) != NUMBER_OVERFLOW) {
        printf("%f\n", fast_pow_answer);
    }

    printf("%f\n", pow(11, 23));

    fast_pow_answer = 1.0;

    if (fast_pow(10, 5, &fast_pow_answer) != NUMBER_OVERFLOW) {
        printf("%f\n", fast_pow_answer);
    }

    printf("%f\n", pow(10, 5));

    fast_pow_answer = 1.0;

    if (fast_pow(21.3123, 3, &fast_pow_answer) != NUMBER_OVERFLOW) {
        printf("%f\n", fast_pow_answer);
    }

    printf("%f\n", pow(21.3123, 3));

    fast_pow_answer = 1.0;

    if (fast_pow(121, 2, &fast_pow_answer) != NUMBER_OVERFLOW) {
        printf("%f\n", fast_pow_answer);
    }

    printf("%f\n", pow(121, 2));

    fast_pow_answer = 1.0;

    if (fast_pow(-121.111, 6, &fast_pow_answer) != NUMBER_OVERFLOW) {
        printf("%f\n", fast_pow_answer);
    }

    printf("%f\n", pow(-121.111, 6));

    fast_pow_answer = 1.0;

    if (fast_pow(-121.111, 5, &fast_pow_answer) != NUMBER_OVERFLOW) {
        printf("%f\n", fast_pow_answer);
    }

    printf("%f\n", pow(-121.111, 5));

    fast_pow_answer = 1.0;

    if (fast_pow(11, 12, &fast_pow_answer) != NUMBER_OVERFLOW) {
        printf("%f\n", fast_pow_answer);
    }
    printf("%f\n", pow(11, 12));

    fast_pow_answer = 1.0;

    if (fast_pow(11, 0, &fast_pow_answer) != NUMBER_OVERFLOW) {
        printf("%f\n", fast_pow_answer);
    }

    fast_pow_answer = 1.0;

    printf("%f\n", pow(11, 0));

    if (fast_pow(11, -2, &fast_pow_answer) != NUMBER_OVERFLOW) {
        printf("%f\n", fast_pow_answer);
    }

    printf("%f\n", pow(11, -2));

    fast_pow_answer = 1.0;

    if (fast_pow(11.342, -2, &fast_pow_answer) != NUMBER_OVERFLOW) {
        printf("%f\n", fast_pow_answer);
    }

    printf("%f\n", pow(11.342, -2));

    fast_pow_answer = 1.0;

    if (fast_pow(11.8437487324832784, 7, &fast_pow_answer) != NUMBER_OVERFLOW) {
        printf("%f\n", fast_pow_answer);
    }

    printf("%f\n", pow(11.8437487324832784, 7));

    fast_pow_answer = 1.0;*/

    /*if (geometric_mean(&result, 5, 1.23, 2.52, 3.21, 4.25, 5.22) == OK) {
        printf("%f\n", result);
    }
    result = 1.0;

    if (geometric_mean(&result, 3, 1.23, 2.52, 3.21) == OK) {
        printf("%f\n", result);
    }
    result = 1.0;

    if (geometric_mean(&result, 7, 1.23, 2.52, 3.21, 52.52, 8.9, 6.7, 1.4) == OK) {
        printf("%f\n", result);
    }
    result = 1.0;*/

    return OK;
}

error fast_pow(double base, int degree, double *fast_pow_answer) {
    error check = OK;

    if (degree < 0) {
        return fast_pow(1.0 / base, -degree, fast_pow_answer);
    }

    if (degree == 0) {
        *fast_pow_answer = 1.0;
        return OK;
    }

    if ((degree & 1) == 0) {
        check = fast_pow(base, degree / 2, fast_pow_answer);
        *fast_pow_answer *= *fast_pow_answer;
        //*fast_pow_answer *= base;
        if (check_denormalize(fast_pow_answer) != OK || check_overflow(fast_pow_answer) != OK) {
            return NUMBER_OVERFLOW;
        }
    }

    if ((degree & 1) != 0) {
        check = fast_pow(base, degree - 1, fast_pow_answer);
        *fast_pow_answer *= base;
        if (check_denormalize(fast_pow_answer) != OK || check_overflow(fast_pow_answer) != OK) {
            return NUMBER_OVERFLOW;
        }
    }

    return check;
}

error check_overflow(const double *number) {
    if (isinf(*number) || isnan(*number) || *number == 0.0 || check_denormalize(number) == NUMBER_OVERFLOW) {
        return NUMBER_OVERFLOW;
    } else {
        return OK;
    }
}

error check_denormalize(const double *number) {
    // Проверка на денормализованное число (т.е. переполнение вниз)
    if (*number != 0.0 && fabs(*number) < DBL_MIN) {
        return NUMBER_OVERFLOW;
    } else {
        return OK;
    }
}

error geometric_mean(double *result, unsigned int count, ...) {
    if (count == 0) return INCORRECT_INPUT;

    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; ++i) {
        double number = va_arg(args, double);
        if (number <= 0) {
            va_end(args);
            return INCORRECT_INPUT;
        }
        if (check_overflow(&number) != OK || check_denormalize(&number) != OK
        || check_denormalize(result) != OK || check_overflow(result) != OK) {
            va_end(args);
            return NUMBER_OVERFLOW;
        }

        *result *= number;
    }

    va_end(args);
    *result = pow(*result, (double)(1.0 / count));
    return OK;
}