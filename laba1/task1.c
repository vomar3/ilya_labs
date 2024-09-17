#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <string.h>

long long int factorial(long long int number);
void sys_of_calc(long long int number);
long long int my_pow(long long int footing, long long int degree);
int str_to_lli(const char *str, long long int *x, int system);

int main(int args, char *argv[]) {
    errno = 0;
    long long int x;

    if (args != 3) {
        printf("input error\n");
        return -1;
    }

    if(!((argv[1][0] == '-' || argv[1][0] == '/') && argv[1][2] == '\0') ) {
        printf("input error\n");
        return -1;
    }

    if (str_to_lli(argv[2], &x, 10) != 0) {
        printf("input error\n");
        return -1;
    }

    if (x <= 0) {
        printf("Error a negative number is not suitable for processing\n");
        return -2;
    }

    if (argv[1][1] == 'h') {
        if (x > 100) {
            printf ("There are no such numbers\n");
        } else {
            int count = 0;
            for (int i = 1; i <= 100; ++i) {
                if (i % x == 0) {
                    printf("Answer: %d ", i);
                    count += 1;
                }
            }

            if (count == 0) {
                printf ("There are no such numbers\n");
            }

        }

    } else if (argv[1][1] == 'p') {

        for (int i = 2; i < x; ++i) {
            if (x % i == 0) {
                printf("the number is composite\n");
                return 0;
            }
        }
        printf("the number is simple\n");

    } else if (argv[1][1] == 's') {

        sys_of_calc(x);

    } else if (argv[1][1] == 'e') {
        if (x > 10) {
            printf ("An invalid number for this flag\n");
            return -1;
        } else {

            for (int i = 1; i <= 10; ++i) {
                printf("|");
                for (int j = 1; j <= x; ++j) {

                    long long int answer;
                    answer = my_pow(i, j);
                    if (answer != -1) {
                        printf("%10lli|", answer);
                    } else {
                        printf("Error\n");
                        return -3;
                    }
                }
                printf("\n");
            }

        }
    } else if (argv[1][1] == 'a') {
        long long int sum = 0;
        for (long long int i = 0; i <= x; ++i) {
            if ((unsigned long long int)(sum + i) > LLONG_MAX) {
                printf("type overflow\n");
                return -3;
            }
            sum += i;
        }
        printf("Sum: %lli\n", sum);

    } else if (argv[1][1] == 'f') {

        if (factorial(x) == -1){
            printf("type overflow\n");
            return -3;
        }else {
            printf("Factorial: %lld\n", factorial(x));
        }

    } else {
        printf("input error\n");
        return -1;
    }

    return 0;
}

long long int factorial(long long int number) {
    long long int answer = 1;
    if (number == 0 || number == 1) {
        return 1;
    }

    for (long long int i = 2; i <= number; ++i) {
        if ((unsigned long long int)(answer * i) > LLONG_MAX) {
            return -1;
        } else {
            answer *= i;
        }
    }

    return answer;
}

long long int my_pow(long long int footing, long long int degree) {

    long long int answer = 1;

    for (int i = 1; i <= degree; ++i) {
        if (answer * footing > LLONG_MAX) {
            return -1;
        }

        answer *= footing;
    }

    return answer;
}

void sys_of_calc(long long int number) {

    char hex_str[100];
    sprintf(hex_str, "%llx", number);

    unsigned long long int n = strlen(hex_str);
    printf("Hex answer: ");

    for (unsigned long long int i = 0; i < n; ++i) {
        printf("%c ", hex_str[i]);
    }
    printf("\n");
}

int str_to_lli(const char *str, long long int *x, int system) {

    char *end;
    *x = strtoll(str, &end, system);

    if ((*x == LLONG_MAX || *x == LLONG_MIN) && (errno == ERANGE)) {
        return -1;
    } else if (errno != 0 && *x == 0) {
        return -1;
    } else if (*end != '\0') {
        return -1;
    } else {
        return 0;
    }
}