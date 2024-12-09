#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define EPS 1e-7

typedef enum ERRORS{
    OK,
    INVALID_INPUT,
    NUMBER_OVERFLOW,
    UNKNOWN_ERROR,
} error;

typedef struct Point{
    double x;
    double y;
} Point;

error polynomial(double *answer, double point, unsigned int degree, ...);

error check_overflow_double(const double *x);

error string_to_ulint(const char *str, unsigned long int *res, int base);

error pow_ulint(unsigned long int *res);

void ulint_to_your_base(char *result, unsigned long int res, int base);

error check_sum_strings(unsigned long int *answer_array, int count, char *original, unsigned long int number,
                        int base);

error Kaprekara_numbers(unsigned long int *answer_array, int base, int count, ...);

int Bulge(int count, ...);

int vector_product(Point p1, Point p2, Point p3);

int my_strlen(const char *str);

int main() {

    int count = 4, i;
    int counts_of_points = 4;
    unsigned long int answer_array[100];
    Point p1 = {0, 0};
    Point p2 = {1, 0};
    Point p3 = {0, 1};
    Point p4 = {0, 1};
    printf("Bulge: \n");
    if (Bulge(counts_of_points, p1, p2, p3, p4) == 0) {
        printf("The polygon is convex\n");
    } else {
        printf("The polygon is NOT convex\n");
    }

    double answer;
    printf("Polynomial: \n");
    switch (polynomial(&answer, 1.5, 2, 3.21, 10.4, 15.65)) {
        case OK:
            printf("Answer: %f\n", answer);
            break;
        case NUMBER_OVERFLOW:
            printf("Number overflow\n");
            return NUMBER_OVERFLOW;
        case INVALID_INPUT:
            printf("Invalid input\n");
            return INVALID_INPUT;
        default:
            printf("unknown error\n");
            return UNKNOWN_ERROR;
    }

    printf("Kaprekar: \n");
    switch (Kaprekara_numbers(answer_array, 10, count, "99", "297", "703", "100")) {
        case OK:
            for (i = 0; i < count * 2; i += 2) {
                if (answer_array[i + 1] == 0) {
                    printf("The number %lu is the Kaprekar number\n", answer_array[i]);
                } else {
                    printf("The number %lu is NOT the Kaprekar number\n", answer_array[i]);
                }
            }
            break;
        case NUMBER_OVERFLOW:
            printf("Number overflow\n");
            return NUMBER_OVERFLOW;
        case INVALID_INPUT:
            printf("Invalid input\n");
            return INVALID_INPUT;
        default:
            printf("unknown error\n");
            return UNKNOWN_ERROR;
    }
    return OK;
}

error polynomial(double *answer, double point, unsigned int degree, ...) {
    if (check_overflow_double(&point) != OK) return NUMBER_OVERFLOW;

    va_list args;
    va_start(args, degree);
    double coef = va_arg(args, double), gorner;
    unsigned int i;
    if (check_overflow_double(&coef) != OK) {
        va_end(args);
        return NUMBER_OVERFLOW;
    }
    gorner = coef;

    for (i = degree; i > 0; --i) {
        coef = va_arg(args, double);
        if (check_overflow_double(&coef) != OK) {
            va_end(args);
            return NUMBER_OVERFLOW;
        }

        gorner = gorner * point + coef;
        if (check_overflow_double(&gorner) != OK) {
            va_end(args);
            return NUMBER_OVERFLOW;
        }
    }

    *answer = gorner;
    va_end(args);
    return OK;
}

error check_overflow_double(const double *x) {
    if (isnan(*x) || isinf(*x)) {
        return NUMBER_OVERFLOW;
    } else {
        return OK;
    }
}

error string_to_ulint(const char *str, unsigned long int *res, int base) {
    char *end = NULL;
    *res = strtoul(str, &end, base);

    if (*res > ULONG_MAX) {
        return NUMBER_OVERFLOW;
    } else if (*res == 0 && end == str) {
        return INVALID_INPUT;
    } else if (*end != '\0') {
        return INVALID_INPUT;
    }

    return OK;
}

error pow_ulint(unsigned long int *res) {
    if (*res > sqrt(ULONG_MAX)) {
        return NUMBER_OVERFLOW;
    } else {
        *res *= *res;
        return OK;
    }
}

void ulint_to_your_base(char *result, unsigned long int res, int base) {
    int count = 0, num, i;
    char swap;
    while (res > 0) {
        num = res % base;
        if (num > 9) {
            result[count] = num + 'A' - 10;
        } else {
            result[count] = num + '0';
        }

        res /= base;
        ++count;
    }

    for (i = 0; i < count / 2; ++i) {
        swap = result[i];
        result[i] = result[count - i - 1];
        result[count - i - 1] = swap;
    }

    result[count] = '\0';
}

int my_strlen(const char *str) {
    char const *end = str;
    while (*end++);
    return end - str - 1;
}

error check_sum_strings(unsigned long int *answer_array, int count, char *original, unsigned long int number,
                        int base) {
    int len = my_strlen(original), i;
    original[len] = '\0';
    char new[100];
    unsigned long int num1, num2;

    for (i = 0; i < len; ++i) {
        new[i] = original[i];
        new[i + 1] = '\0';
        original[i] = '0';

        if (string_to_ulint(new, &num1, base) == OK
        && string_to_ulint(original, &num2, base) == OK) {
            if (num1 + num2 == number && num1 != 0 && num2 != 0) {
                answer_array[count + 1] = 0;
                return OK;
            }
        } else {
            return INVALID_INPUT;
        }
    }

    answer_array[count + 1] = 1;
    return OK;
}

error Kaprekara_numbers(unsigned long int *answer_array, int base, int count, ...) {
    if (base < 2 || base > 36 || count < 1) return INVALID_INPUT;

    int i;
    unsigned long int new;
    va_list args;
    va_start(args, count);
    char number_line[BUFSIZ];
    unsigned long int number;

    for (i = 0; i < count; ++i) {
        // просто считываю число, закидываю в переменную, перевожу в uns long int
        strcpy(number_line, va_arg(args, char *));
        if (string_to_ulint(number_line, &number, base) != OK) {
            va_end(args);
            return INVALID_INPUT;
        }
        // делаю 2*i с идеей того, что в i элементе будет написано преобразовалось ли число, а в 2*i само число
        // + возведение в квадрат
        answer_array[2 * i] = number;
        new = number;
        if (pow_ulint(&number) != OK) {
            va_end(args);
            return NUMBER_OVERFLOW;
        }
        // Перевожу число в квадрате в строчку, чтобы начать разбивать
        ulint_to_your_base(number_line, number, base);

        // Тут вся дробежка + переводы и сравнения
        if (check_sum_strings(answer_array, 2 * i, number_line, new, base) != OK) {
            va_end(args);
            return INVALID_INPUT;
        }
    }

    va_end(args);
    return OK;
}

int Bulge(int count, ...) {
    if (count < 3) return 1;

    int i;
    va_list args;
    double answer;
    va_start(args, count);

    Point point1, point2;

    Point p1 = va_arg(args, Point);
    Point p2 = va_arg(args, Point);
    Point p3 = va_arg(args, Point);

    answer = vector_product(p1, p2, p3);

    point1 = p1;
    point2 = p2;

    for (i = 0; i < count - 3; ++i) {
        point1 = point2;
        point2 = p3;
        p3 = va_arg(args, Point);

        if (vector_product(point1, point2, p3) != answer) {
            va_end(args);
            return 1;
        }
    }

    if (vector_product(point2, p3, point1) != answer
    || vector_product(p3, point1, p1) != answer) {
        va_end(args);
        return 1;
    }

    va_end(args);
    return 0;
}

int vector_product(Point p1, Point p2, Point p3) {
    if ((((p2.x - p1.x) * (p3.y - p2.y)) - ((p2.y - p1.y) * (p3.x - p2.x))) - EPS > 0) {
        return 1;
    } else {
        return -1;
    }
}