#include <stdio.h>
#include <limits.h>

typedef enum ERRORS{
    OK,
    INVALID_INPUT,
    NUMBER_OVERFLOW,
} error;

int plus(int a, int b);

error num_to_new_base(int num, int r, char *answer, int *sign);

error check_overflow_int(int num);

int main() {

    char answer[128];
    int sign = 0;
    switch (num_to_new_base(52, 2, answer, &sign)) {
        case INVALID_INPUT:
            printf("Invalid input\n");
            return INVALID_INPUT;
        case NUMBER_OVERFLOW:
            printf("Number overflow\n");
            return NUMBER_OVERFLOW;
        case OK:
            if (sign == 1) printf("-");
            printf("%s", answer);
            break;
    }

    return OK;
}

error num_to_new_base(int num, int r, char *answer, int *sign) {
    if (r < 1 || r > 5) return INVALID_INPUT;
    char base[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV";

    if (check_overflow_int(num) != OK) return NUMBER_OVERFLOW;

    if (num < 0) {
        num = plus(~num, 1);
        *sign = 1;
    }

    int count = 0;
    while (num != 0) {
        // 22 (10) -> 112 (4)
        int remain = ((num >> r) << r) ^ num; // 2 1 1
        answer[count] = base[remain]; // 2 1 1
        count = plus(count, 1); // 1 2 3
        num = num >> r; // 5 1 0
    }
    answer[count] = '\0';

    int len = count >> 1;
    int ans = plus(count, -1);
    for (int i = 0; i < len; i = plus(i , 1)) {
        char symbol = answer[i];
        answer[i] = answer[plus(ans, -i)];
        answer[plus(ans, -i)] = symbol;
    }

    return OK;
}

int plus(int a, int b) {
    while (b != 0) {
        int sum_ones = a ^ b;
        int shift = (a & b) << 1;

        a = sum_ones;
        b = shift;
    }

    return a;
}

error check_overflow_int(int num) {
    if (num > INT_MAX || num < INT_MIN) {
        return NUMBER_OVERFLOW;
    } else {
        return OK;
    }
}