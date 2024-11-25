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

error sum_two_nums(int *count_answers, unsigned int system, char **answer, size_t *buf_size, const char *new_line);

error sum_all_numbers(int *count_answers, char **answer, unsigned int system, unsigned int count, ...);

int main() {
    unsigned int system = 10, count = 2, count_for_answer = 0;
    int count_answers = 0, i;
    size_t answer_size = 1;
    char *answer = (char *) malloc((answer_size + 1) * sizeof(char));
    if (!answer) {
        printf("Memory error\n");
        return MEMORY_ERROR;
    }

    switch (sum_all_numbers(&count_answers, &answer, system, count,
                            "999999999999999999999992131111111111143242342342342343242341111111111111111111114324324234324324324435546546111111111111143",
                            "5211113423423423432423423423423423419999999999999999999999999991111111111111111111")) {
        case OK:
            for (i = count_answers - 1; i >= 0; --i) {
                if (count_for_answer == 0 && answer[i] == '0')
                    continue;
                if (answer[i] <= '9') {
                    ++count_for_answer;
                    printf("%c", answer[i]);
                } else {
                    printf("%c", (answer[i] - '0' + 'A' - 10));
                    ++count_for_answer;
                }
            }
            printf("\n");
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
    const char *end = str;
    while (*end++);
    return end - str - 1;
}

void reverse(char *str) {
    if (!str)
        return;

    size_t len = strlen(str);
    size_t i;
    char temp;

    for (i = 0; i < len / 2; i++) {
        temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

error sum_all_numbers(int *count_answers, char **answer, unsigned int system, unsigned int count, ...) {
    if (system < 2 || system > 36 || count < 1)
        return INVALID_INPUT;

    va_list args;
    va_start(args, count);
    size_t buf_size = 1;
    int i;

    for (i = 0; i < count; ++i) {

        const char *input_line = va_arg(args, const char *);
        size_t input_len = my_strlen(input_line);

        char *number_line = (char *) malloc((input_len + 1) * sizeof(char));
        if (!number_line) {
            va_end(args);
            return MEMORY_ERROR;
        }

        strcpy(number_line, input_line);
        reverse(number_line);

        if (sum_two_nums(count_answers, system, answer, &buf_size, number_line) != OK) {
            free(number_line);
            va_end(args);
            return INVALID_INPUT;
        }

        free(number_line);
    }

    va_end(args);

    return OK;
}

error sum_two_nums(int *count_answers, unsigned int system, char **answer, size_t *buf_size, const char *new_line) {
    if (!answer)
        return MEMORY_ERROR;

    int shift = 0, i, num_new_line, num_answer, sum;
    size_t len = my_strlen(new_line);
    size_t temp_buf_size;

    if (*count_answers > len) {
        temp_buf_size = *count_answers;
    } else {
        temp_buf_size = len;
    }

    char *temp_buffer = (char *) malloc((temp_buf_size + 1) * sizeof(char));
    if (!temp_buffer)
        return MEMORY_ERROR;

    for (i = 0; i < len || i < *count_answers; ++i) {

        if (i < len) {
            if (new_line[i] >= '0' && new_line[i] <= '9') {
                num_new_line = new_line[i] - '0';
            } else {
                num_new_line = new_line[i] - 'A' + 10;
            }
        } else {
            num_new_line = 0;
        }

        if (i < *count_answers && (*answer)[i] != '\0') {
            num_answer = (*answer)[i] - '0';
        } else {
            num_answer = 0;
        }

        if (num_new_line >= system) {
            free(temp_buffer);
            return INVALID_INPUT;
        }

        sum = num_new_line + num_answer + shift;
        if (sum >= system) {
            shift = 1;
            sum -= system;
        } else {
            shift = 0;
        }

        temp_buffer[i] = sum + '0';
    }

    if (shift != 0) {
        if (temp_buf_size + 1 > *buf_size) {
            *buf_size *= 2;
            char *new_answer = (char *) realloc(*answer, *buf_size * sizeof(char));
            if (!new_answer) {
                free(temp_buffer);
                return MEMORY_ERROR;
            }
            *answer = new_answer;
        }
        temp_buffer[temp_buf_size] = '1';
        ++temp_buf_size;
    }

    if (temp_buf_size > *buf_size) {
        *buf_size = temp_buf_size;
        char *new_answer = (char *) realloc(*answer, (*buf_size + 1) * sizeof(char));
        if (!new_answer) {
            free(temp_buffer);
            return MEMORY_ERROR;
        }
        *answer = new_answer;
    }
    memcpy(*answer, temp_buffer, temp_buf_size);
    *count_answers = temp_buf_size;

    free(temp_buffer);
    return OK;
}
