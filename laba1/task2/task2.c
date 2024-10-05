#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <string.h>

int str_to_double(char *str, double *number);

double limit_e(double epsilon);

double limit_pi(double epsilon);

double limit_ln2(double epsilon);

double limit_sqrt2(double epsilon);

double limit_lambda(double epsilon);

double row_e(double epsilon);

double row_pi(double epsilon);

double row_ln2(double epsilon);

double row_sqrt2(double epsilon);

double e_equation(double epsilon);

double pi_equation(double epsilon);

double sqrt2_equation(double epsilon);

double ln2_equation(double epsilon);

enum ERRORS{
    OK,
    ERROR_INPUT,

};

int main(int args, char* argv[]) {

    double epsilon;
    if (str_to_double(argv[1], &epsilon) == -2 || epsilon <= 0.0 || epsilon >= 1.0) {
        printf("Invalid input\n");
        return ERROR_INPUT;
    }

    if (args != 2) {
        printf("Invalid input\n");
        return ERROR_INPUT;
    }

    printf("e:\nlim: %lf, row: %lf, equal: %lf\n", limit_e(epsilon), row_e(epsilon), e_equation(epsilon));
    printf("pi:\nlim: %lf, row: %lf, equal: %lf\n", limit_pi(epsilon), row_pi(epsilon), pi_equation(epsilon));
    printf("ln2:\nlim: %lf, row: %lf, equal: %lf\n", limit_ln2(epsilon), row_ln2(epsilon), ln2_equation(epsilon));
    printf("sqrt2:\nlim: %lf, row: %lf, equal: %lf\n", limit_sqrt2(epsilon), row_sqrt2(epsilon), sqrt2_equation(epsilon));

    return OK;
}

int str_to_double(char *str, double *number) {
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

double limit_e(double epsilon) {
    int n = 1;
    double current_iteration = 0.0, last_iteration;
    do
    {
        last_iteration = current_iteration;
        n *= 2;
        current_iteration = pow((1.0 + 1.0 / n) , n);
    } while (fabs(last_iteration - current_iteration) >= epsilon);
    return current_iteration;
}

double limit_pi(double epsilon) {
    int n = 2;
    double current_iteration = 4.0, last_iteration;
    do
    {
        last_iteration = current_iteration;
        current_iteration *= (4.0 * n * (n - 1.0)) / pow(2.0 * n - 1.0, 2);
        n++;
    } while (fabs(last_iteration - current_iteration) >= epsilon);
    return current_iteration;
}

double limit_ln2(double epsilon) {
    int n = 1;
    double current_iteration = 0.0, last_iteration;
    do
    {
        last_iteration = current_iteration;
        n *= 2;
        current_iteration = n * (pow(2.0, 1.0 / (double) n) - 1.0);
    } while (fabs(last_iteration - current_iteration) >= epsilon);
    return current_iteration;
}

double limit_sqrt2(double epsilon) {
    int n = 1;
    double current_iteration = 0.5, last_iteration;
    do
    {
        last_iteration = current_iteration;
        n++;
        current_iteration = last_iteration - last_iteration * last_iteration * 0.5 + 1.0;
    } while (fabs(last_iteration - current_iteration) >= epsilon);
    return current_iteration;
}

double row_e(double epsilon) {
    double sum = 1.0, last_iteration = 1.0;
    int n = 1;

    do
    {
        last_iteration /= (double)n;
        sum += last_iteration;
        n++;
    } while (last_iteration >= epsilon);

    return sum;
}

double row_pi(double epsilon) {
    double sum = 0.0, last_iteration;
    int n = 1, sign = 1;

    do
    {
        last_iteration = 4 * sign / (2.0 * n - 1.0);
        sum += last_iteration;
        n++;
        sign *= -1;
    } while (fabs(last_iteration) >= epsilon);

    return sum;
}

double row_ln2(double epsilon) {
    double sum = 0.0, last_iteration;
    int n = 1, sign = 1;

    do
    {
        last_iteration = (double)sign / n;
        sum += last_iteration;
        n++;
        sign *= -1;
    } while (fabs(last_iteration) >= epsilon);

    return sum;
}

double row_sqrt2(double epsilon) {
    double sum = pow(2.0, 0.25), last_iteration = 0.0;
    int k = 2;

    do
    {
        last_iteration = sum;
        ++k;
        sum *= pow(2, 1.0 / pow(2, k));
    } while (fabs(sum - last_iteration) >= epsilon);

    return sum;
}

double e_equation(double epsilon) {
    double x = 1;
    double f = 1, df;
    while (fabs(f) > epsilon)
    {
        f = log(x) - 1;
        df = 1.0 / x;
        x = x - f / df;
    }
    return x;
}

double pi_equation(double epsilon) {
    double x = 1;
    double f = 1, df;
    while (fabs(f) > epsilon)
    {
        f = cos(x) + 1;
        df = -sin(x);
        x = x - f / df;
    }
    return x;
}

double ln2_equation(double epsilon) {
    double x = 1;
    double f = 1, df;
    while (fabs(f) > epsilon)
    {
        f = exp(x) - 2;
        df = exp(x);
        x = x - f / df;
    }
    return x;
}

double sqrt2_equation(double epsilon) {
    double x = 1;
    double f = 1, df;
    while (fabs(f) > epsilon)
    {
        f = x * x - 2;
        df = 2 * x;
        x = x - f / df;
    }
    return x;
}