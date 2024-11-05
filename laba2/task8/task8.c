#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

typedef enum ERRORS {
    OK,
    INVALID_INPUT,
    MEMORY_ERROR,
} error;

size_t my_strlen(const char *str);

void reverse(char *str);

error sum_two_nums(int *count_answers, unsigned int system, char *answer, int *buf, char *new_line);

error sum_all_numbers(int *count_answers, char *answer, unsigned int system, unsigned int count, ...);

int main() {
    unsigned int system = 36, count = 4, count_for_answer = 0;
    int count_answers = 0;
    char *answer = (char *) malloc(BUFSIZ * sizeof(char));
    if (answer == NULL) {
        printf("Memory error\n");
        return MEMORY_ERROR;
    }
    memset(answer, '\0', BUFSIZ);

    switch (sum_all_numbers(&count_answers, answer, system, count,
                            "MMMMMMMMMMMMMMMMMMMMMM",
                            "ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ", "000000000", "1")) {
        case OK:
            for (int i = count_answers - 1; i >= 0; --i) {
                if (count_for_answer == 0 && answer[i] == '0') continue;
                if (answer[i] <= '9') {
                    ++count_for_answer;
                    printf("%c", answer[i]);
                } else {
                    printf("%c", (answer[i] - '0' + 'A' - 10));
                    ++count_for_answer;
                }
            }
            break;
        case INVALID_INPUT:
            printf("Invalid input\n");
            free(answer);
            return INVALID_INPUT;
        default:
            printf("Memory error\n");
            free(answer);
            return MEMORY_ERROR;
    }

    free(answer);
    return OK;
}

size_t my_strlen(const char *str) {
    char const *end = str;
    while (*end++);
    return end - str - 1;
}

error sum_all_numbers(int *count_answers, char *answer, unsigned int system, unsigned int count, ...) {
    if (system < 2 || system > 36 || count < 1) return INVALID_INPUT;

    va_list args;
    int buf = 512;
    va_start(args, count);

    for (int i = 0; i < count; ++i) {
        char number_line[BUFSIZ * 10];
        strcpy(number_line, va_arg(args, char *));
        reverse(number_line);

        if (sum_two_nums(count_answers, system, answer, &buf, number_line) != OK) {
            va_end(args);
            return INVALID_INPUT;
        }
    }

    va_end(args);

    return OK;
}

void reverse(char *str) {
    size_t len = strlen(str);

    char symbol;
    for (int i = 0; i < len / 2; i++) {
        symbol = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = symbol;
    }
}

error sum_two_nums(int *count_answers, unsigned int system, char *answer, int *buf, char *new_line) {
    size_t len = my_strlen(new_line);
    if (*count_answers < len) *count_answers = len;

    if (*count_answers > *buf) {
        *buf *= 2;
        char *realloc_str = (char *) realloc(new_line, *buf);
        if (realloc_str == NULL) {
            return MEMORY_ERROR;
        }
        new_line = realloc_str;
    }

    if (new_line[0] == '-') return INVALID_INPUT;

    int flag = 0;
    for (int i = 0; i < len; ++i) {
        int number;
        if (new_line[i] >= '0' && new_line[i] <= '9') {
            number = new_line[i] - '0';
        } else {
            number = new_line[i] - 'A' + 10;
        }

        if (number >= system) return INVALID_INPUT;

        int current;
        if (answer[i] != '\0') {
            current = answer[i] - '0';
        } else {
            current = 0;
        }
        int sum = current + number + flag;

        if (sum >= system) {
            answer[i] = (sum - system) + '0';
            flag = 1;
        } else {
            answer[i] = sum + '0';
            flag = 0;
        }
    }

    int count = len, current;
    while (flag == 1) {
        if (answer[count] != '\0') {
            current = answer[count] - '0';
        } else {
            current = 0;
        }
        if (current == system - 1) {
            answer[count] = '0';
            ++count;
            if (*count_answers < count + 1) {
                *count_answers = count + 1;
            }
        } else {
            answer[count] = current + 1 + '0';
            flag = 0;
            if (*count_answers < count + 1) {
                *count_answers = count + 1;
            }
        }
    }

    return OK;
}
