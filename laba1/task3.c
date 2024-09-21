#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include <float.h>

int check_overflow(long double number);

int str_to_long_double(const char *str, long double *result);

int str_to_long_int(const char *str, long int *number, int system);

void answers(long double a, long double b, long double c, long double epsilon);

int main(int args, char *argv[]) {

    if ( !((argv[1][0] == '-' || argv[1][0] == '/') && (argv[1][2] == '\0'))) {
        printf("Invalid input\n");
        return -1;
    }

    switch (argv[1][1]) {
        case 'q':

            if (args != 6) {
                printf("Invalid input\n");
                return -1;
            } else {
                long double epsilon, coef1, coef2, coef3;

                if (! ((str_to_long_double(argv[2], &epsilon) == 0)
                && (str_to_long_double(argv[3], &coef1) == 0)
                && (str_to_long_double(argv[4], &coef2) == 0)
                && (str_to_long_double(argv[5], &coef3) == 0))) {
                    printf("Invalid input\n");
                    return -1;
                }

                if (epsilon < 0) {
                    printf("Invalid input\n");
                    return -1;
                }

                if (coef1 <= epsilon || coef2 <= epsilon || coef3 <= epsilon) {
                    printf("Invalid input\n");
                    return -1;
                }

                if (fabsl(coef1 - coef2) >= epsilon
                && fabsl(coef1 - coef3) >= epsilon
                && fabsl(coef2 - coef3) >= epsilon) {
                    answers(coef1, coef2, coef3, epsilon);
                    answers(coef1, coef3, coef2, epsilon);
                    answers(coef2, coef3, coef1, epsilon);
                    answers(coef2, coef1, coef3, epsilon);
                    answers(coef3, coef1, coef2, epsilon);
                    answers(coef3, coef2, coef1, epsilon);
                } else if (fabsl(coef1 - coef2) <= epsilon
                && fabsl(coef1 - coef3) <= epsilon) {
                    answers(coef1, coef2, coef3, epsilon);
                } else if (fabsl(coef1 - coef2) <= epsilon
                && fabsl(coef1 - coef3) >= epsilon) {
                    answers(coef1, coef2, coef3, epsilon);
                    answers(coef1, coef3, coef2, epsilon);
                    answers(coef3, coef1, coef2, epsilon);
                } else if (fabsl(coef1 - coef3) <= epsilon
                && fabsl(coef1 - coef2) >= epsilon) {
                    answers(coef1, coef2, coef3, epsilon);
                    answers(coef1, coef3, coef2, epsilon);
                    answers(coef2, coef1, coef3, epsilon);
                } else {
                    answers(coef1, coef2, coef3, epsilon);
                    answers(coef2, coef1, coef3, epsilon);
                    answers(coef2, coef3, coef1, epsilon);
                }

            }

            break;
        case 'm':

            if (args != 4) {
                printf("Invalid input\n");
                return -1;
            } else {

                long int first, second;
                if ( !((str_to_long_int(argv[2], &first, 10) == 0)
                && (str_to_long_int(argv[3], &second, 10) == 0)
                && first != 0 && second != 0)) {
                    printf("Invalid input\n");
                    return -1;
                }

                if (first % second == 0) {
                    printf("the first number IS a multiple of the second\n");
                    return 0;
                } else {
                    printf("the first number IS NOT a multiple of the second\n");
                    return 0;
                }

            }
        case 't':

            if (args != 6) {
                printf("Invalid input\n");
                return -1;
            } else {
                long double epsilon_t, coef1_t, coef2_t, coef3_t;

                if (str_to_long_double(argv[2], &epsilon_t) == -2
                || str_to_long_double(argv[3], &coef1_t) == -2
                || str_to_long_double(argv[4], &coef2_t) == -2
                || str_to_long_double(argv[5], &coef3_t) == -2){
                    printf("Invalid input\n");
                    return -1;
                }

                if (epsilon_t < 0) {
                    printf("Invalid input\n");
                    return -1;
                }

                if (coef1_t <= epsilon_t || coef2_t <= epsilon_t || coef3_t <= epsilon_t) {
                    printf("Invalid input\n");
                    return -1;
                }

                if (!(check_overflow(coef1_t) == 0
                && check_overflow(coef2_t) == 0
                && check_overflow(coef3_t) == 0)) {
                    printf("Element overflow\n");
                    return -3;
                }

                if (fabsl(coef1_t * coef1_t + coef2_t * coef2_t - coef3_t * coef3_t) <= epsilon_t
                || fabsl(coef1_t * coef1_t + coef3_t * coef3_t - coef2_t * coef2_t) <= epsilon_t
                || fabsl(coef2_t * coef2_t + coef3_t * coef3_t - coef1_t * coef1_t) <= epsilon_t) {
                    printf("a right-angled triangle\n");
                    return 0;
                } else {
                    printf("It's not a right-angled triangle\n");
                    return 0;
                }
            }

        default:

            printf("Invalid input\n");
            return -1;
    }

    return 0;
}

int str_to_long_double(const char *str, long double *result) {

    int dots = 0;
    for (int i = 0; i < strlen(str); ++i) {
        if ((str[i] < '0' || str[i] > '9') && str[i] != '.') {
            printf("the problem with the element\n");
            return -2;
        }
        else if (str[i] == '.') {
            dots++;
        }
    }

    if (dots != 1) {
        printf("the problem with the element\n");
        return -2;
    }

    char *end = NULL;
    *result = strtold(str, &end);

    if (*result == HUGE_VAL || *result == -HUGE_VAL) {
        printf("the problem with the element\n");
        return -2;
    } else if (*result == 0 && end == str) {
        printf("the problem with the element\n");
        return -2;
    } else if (*end != '\0') {
        printf("the problem with the element\n");
        return -2;
    } else {
        return 0;
    }
}

int str_to_long_int(const char *str, long int *number, int system) {
    char *end = NULL;
    *number = strtol(str, &end, system);

    if (*number == LONG_MAX || *number == LONG_MIN) {
        printf("the problem with the element\n");
        return -2;
    } else if (end == str && *number == 0) {
        printf("the problem with the element\n");
        return -2;
    } else if (*end != '\0') {
        printf("the problem with the element\n");
        return -2;
    } else {
        return 0;
    }
}

int check_overflow(long double number) {
    if ((sqrt(LDBL_MAX)) + 1.0 > number) {
        return 0;
    } else {
        return -1;
    }
}

void answers(long double a, long double b, long double c, long double epsilon) {
    long double discriminant = b * b - 4.0 * a * c;

    if (discriminant < 0) {
        printf("Discriminant < 0. There are no solutions\n");
    } else if (fabsl(discriminant) <= epsilon) {
        long double x = -b / (2.0 * a);
        printf("x = %Lf\n", x);
    } else {
        discriminant = sqrtl(discriminant);

        long double x1 = (-b + discriminant) / (2.0 * a);
        long double x2 = (-b - discriminant) / (2.0 * a);

        printf("x1 = %Lf x2 = %Lf\n", x1, x2);
    }
}
