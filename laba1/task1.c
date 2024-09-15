#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

long long int factorial(long long int number);
void sys_of_calc(long long int number);
long long int my_pow(long long int footing, long long int degree);


int main() {
    long long int x;
    char flag[10];

    if(2 != scanf("%lli %2s", &x, flag) || (x == LLONG_MAX)) {
        printf("input error\n");
        return -1;
    }
    if (x <= 0) {
        printf("Error a negative number is not suitable for processing\n");
        return -1;
    }

    printf("%lli %s\n", x, flag);

    if (strcmp(flag, "-h") == 0 || strcmp(flag, "/h") == 0) {
        if (x > 100) {
            printf ("There are no such numbers\n");
        } else {
            int count = 0;
            for (int i = 1; i <= 100; ++i) {
                if (i % x == 0) {
                    printf("%d ", i);
                    count += 1;
                }
            }

            if (count == 0) {
                printf ("There are no such numbers\n");
            }

        }

    } else if (strcmp(flag, "-p") == 0 || strcmp(flag, "/p") == 0) {

        for (int i = 2; i < x; ++i) {
            if (x % i == 0) {
                printf("the number is composite\n");
                return 0;
            }
        }
        printf("the number is simple\n");

    } else if (strcmp(flag, "-s") == 0 || strcmp(flag, "/s") == 0) {

        sys_of_calc(x);

    } else if (strcmp(flag, "-e") == 0 || strcmp(flag, "/e") == 0) {
        if (x > 10) {
            printf ("An invalid number for this flag\n");
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
                    }
                }
                printf("\n");
            }

        }
    } else if (strcmp(flag, "-a") == 0 || strcmp(flag, "/a") == 0) {
        long long int sum = 0;
        for (long long int i = 0; i <= x; ++i) {
            if ((unsigned long long int)(sum + i) > LLONG_MAX) {
                printf("type overflow\n");
                return -1;
            }
            sum += i;
        }
        printf("Sum: %lli", sum);

    } else if (strcmp(flag, "-f") == 0 || strcmp(flag, "/f") == 0) {

        if (factorial(x) == -1){
            printf("type overflow\n");
        }else {
            printf("%lld", factorial(x));
        }

    } else {
        printf("input error\n");
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
    printf("Hex answer: %s\n", hex_str);
}