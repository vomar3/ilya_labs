#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

long long int str_to_llint(const char *str, long long int *number, int system);

void ten_to_nine(long long int number);

void ten_to_others(long long int number, int system);

int main (int args, char* argv[]) {

    int system;
    char *num = malloc(100 * sizeof(char));
    memset(num, 0, 100 * sizeof(char));
    long long int number;
    long long int max = LLONG_MIN;

    if (2 != scanf("%d %s", &system, num) || system < 2 || system > 36) {
        printf("Invalid input\n");
        return -1;
    }

    while (strcmp(num, "Stop") != 0) {
        if (str_to_llint(num, &number, system) == -2) {
            return -2;
        }

        if (llabs(number) > max) {
            max = llabs(number);
        }

        scanf("%s", num);
    }

    free(num);

    ten_to_others(max, system);
    ten_to_nine(max);
    //printf("10cc number: %lli\n", max);
    ten_to_others(max, 18);
    ten_to_others(max, 27);
    ten_to_others(max, 36);

    return 0;
}

long long int str_to_llint(const char *str, long long int *number, int system) {
    char *end = NULL;
    *number = strtoll(str, &end, system);

    if (*number == LLONG_MIN || *number == LLONG_MAX) {
        printf("the problem with the element\n");
        return -2;
    } else if (end == str && *number == 0) {
        printf("the problem with the element\n");
        return -2;
    } else if (*end != '\0') {
        printf("the problem with the element\n");
        return -2;
    }

    return 0;
}

void ten_to_nine(long long int number) {

    int *array = malloc(1 * sizeof(int));
    int flag = 0;
    int count = 0;

    while (number > 0) {
        array[count] = (int)(number % 9);
        number = number / 9;
        count++;

        int *new_check = realloc(array, (sizeof(array) + 1) * sizeof(int));
        if (new_check == NULL) {
            printf("Memory error\n");
            free(array);
            flag = 1;
            number = 0;
        }

        array = new_check;
    }

    if (flag == 0) {
        printf("9cc number: ");
        for (int i = count - 1; i >= 0; --i) {
            printf("%d", array[i]);
        }

        printf("\n");
    }

    free(array);
}

void ten_to_others(long long int number, int system) {

    char *array = malloc(1 * sizeof(char));
    int flag = 0;
    int count = 0;

    while (number > 0) {

        if (number % system <= 9) {
            array[count] = (char)(number % system + '0');
        } else {
            array[count] = (char)((number % system) + 87);
        }

        number = number / system;
        count++;

        char *new_check = realloc(array, (sizeof(array) + 1) * sizeof(char));
        if (new_check == NULL) {
            printf("Memory error\n");
            free(array);
            flag = 1;
            number = 0;
        }

        array = new_check;
    }

    if (flag == 0) {
        printf("%dcc number: ", system);
        for (int i = count - 1; i >= 0; --i) {
            printf("%c", array[i]);
        }

        printf("\n");
    }

    free(array);
}
