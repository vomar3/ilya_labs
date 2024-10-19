#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef enum ERRORS {
    OK,
    INVALID_INPUT,
    MEMORY_ERROR,
    OVERFLOW,
} error;

int my_strlen(const char *str);

error str_to_Uint(const char *str, unsigned int *number, int system);

void my_reverse(const char *str, char *new_line);

void upper_case(const char *str, char *answer);

void str_numbers_symbols_others(const char *str, char *answer);

void free_all(char *line_r, char *line_u, char *line_n, char *line_c);

error concatenation(const char *str, char *line_c, int *my_len_for_c, size_t *buf);

error function(int num, char *argv[], size_t buf, char *line_c);

int main(int args, char *argv[]) {

    if (args < 3) {
        printf("Invalid input\n");
        return INVALID_INPUT;
    }

    if (argv[1][0] != '-' || argv[1][2] != '\0') {
        printf("Invalid input\n");
        return INVALID_INPUT;
    }
    size_t buf = BUFSIZ;
    unsigned int number_for_random;
    int num = args - 3;
    char *line_r = (char *) malloc((my_strlen(argv[2]) + 1) * sizeof(char));
    char *line_u = (char *) malloc((my_strlen(argv[2]) + 1) * sizeof(char));
    char *line_n = (char *) malloc((my_strlen(argv[2]) + 1) * sizeof(char));
    char *line_c = (char *) malloc(buf * sizeof(char));
    if (line_r == NULL || line_u == NULL || line_n == NULL || line_c == NULL) {
        printf("Error with malloc\n");
        free_all(line_r, line_u, line_n, line_c);
        return MEMORY_ERROR;
    }

    switch (argv[1][1]) {
        case 'l':
            if (args != 3) {
                free_all(line_r, line_u, line_n, line_c);
                printf("Invalid input\n");
                return INVALID_INPUT;
            }

            printf("The length of line: %d\n", my_strlen(argv[2]));
            break;
        case 'r':
            if (args != 3) {
                free_all(line_r, line_u, line_n, line_c);
                printf("Invalid input\n");
                return INVALID_INPUT;
            }

            printf("Original line: %s\n", argv[2]);
            my_reverse(argv[2], line_r);
            printf("Reversed line: %s\n", line_r);
            break;

        case 'u':
            if (args != 3) {
                free_all(line_r, line_u, line_n, line_c);
                printf("Invalid input\n");
                return INVALID_INPUT;
            }

            printf("Original line: %s\n", argv[2]);
            upper_case(argv[2], line_u);
            printf("Line with upper: %s\n", line_u);
            break;

        case 'n':
            if (args != 3) {
                free_all(line_r, line_u, line_n, line_c);
                printf("Invalid input\n");
                return INVALID_INPUT;
            }

            printf("Original line: %s\n", argv[2]);
            str_numbers_symbols_others(argv[2], line_n);
            printf("Line with sequence: numbers, letters, others: %s\n", line_n);
            break;

        case 'c':

            if (args < 5) {
                free_all(line_r, line_u, line_n, line_c);
                printf("Invalid input\n");
                return INVALID_INPUT;
            }

            if (str_to_Uint(argv[2], &number_for_random, 10) == INVALID_INPUT) {
                printf("Invalid input\n");
                free_all(line_r, line_u, line_n, line_c);
                return INVALID_INPUT;
            } else if (str_to_Uint(argv[2], &number_for_random, 10) == OVERFLOW) {
                printf("Number is overflow\n");
                free_all(line_r, line_u, line_n, line_c);
                return OVERFLOW;
            }

            srand(number_for_random);
            if (function(num, argv, buf, line_c) == OK) {
                printf("New line: %s\n", line_c);
            } else {
                printf("Memory error\n");
                free_all(line_r, line_u, line_n, line_c);
                return MEMORY_ERROR;
            }

            break;

        default:
            printf("Invalid input\n");
            free_all(line_r, line_u, line_n, line_c);
            return INVALID_INPUT;
    }

    free_all(line_r, line_u, line_n, line_c);

    return OK;
}

error str_to_Uint(const char *str, unsigned int *number, int system) {
    char *end;
    *number = strtoul(str, &end, system);

    if (*number == ULONG_MAX) {
        return OVERFLOW;
    } else if (end == str && *number == 0) {
        return INVALID_INPUT;
    } else if (*end != '\0') {
        return INVALID_INPUT;
    } else {
        return OK;
    }
}

int my_strlen(const char *str) {
    /*char const *end = str;
    int count = 0;
    while (*end != '\0') {
        count++;
        end++;
    }

    return count;*/
    //Моя запись

    char const *end = str;
    while (*end++);
    return end - str - 1;
    // Более компактная запись с лаб
}

void my_reverse(const char *str, char *new_line) {
    int len = my_strlen(str);

    if (len > 0) {
        for (int i = 0; i < len; ++i) {
            new_line[len - i - 1] = str[i];
        }
    }

    new_line[len] = '\0';
}

void upper_case(const char *str, char *answer) {
    int len = my_strlen(str);

    for (int i = 0; i < len; ++i) {
        if ((i & 1) != 0) {
            if (str[i] >= 'a' && str[i] <= 'z') {
                answer[i] = str[i] - 32;
            } else {
                answer[i] = str[i];
            }
        } else {
            answer[i] = str[i];
        }
    }

    answer[len] = '\0';
}

void str_numbers_symbols_others(const char *str, char *answer) {
    int len = my_strlen(str);
    int count = 0;

    for (int i = 0; i < len; ++i) {
        if (str[i] >= '0' && str[i] <= '9') {
            answer[count] = str[i];
            ++count;
        }
    }

    for (int i = 0; i < len; ++i) {
        if ((str[i] >= 'a' && str[i] <= 'z') ||
            (str[i] >= 'A' && str[i] <= 'Z')) {
            answer[count] = str[i];
            ++count;
        }
    }

    for (int i = 0; i < len; ++i) {
        if (!((str[i] >= 'a' && str[i] <= 'z') ||
              (str[i] >= 'A' && str[i] <= 'Z') ||
              (str[i] >= '0' && str[i] <= '9'))) {
            answer[count] = str[i];
            ++count;
        }
    }

    answer[len] = '\0';
}

void free_all(char *line_r, char *line_u, char *line_n, char *line_c) {
    if (line_n != NULL) {
        free(line_n);
        line_n = NULL;
    }
    if (line_u != NULL) {
        free(line_u);
        line_u = NULL;
    }
    if (line_r != NULL) {
        free(line_r);
        line_r = NULL;
    }
    if (line_c != NULL) {
        free(line_c);
        line_c = NULL;
    }
}

error concatenation(const char *str, char *line_c, int *my_len_for_c, size_t *buf) {
    if (line_c == NULL) { return MEMORY_ERROR; }

    int size = my_strlen(str);

    if (*buf < *my_len_for_c + size + 1) {
        *buf *= 2;
        char *new = (char *) realloc(line_c, *buf);
        if (new == NULL) { return MEMORY_ERROR; }
        line_c = new;
    }

    for (int i = 0; i < size; ++i) {
        line_c[*my_len_for_c] = str[i];
        (*my_len_for_c)++;
    }

    line_c[*my_len_for_c] = '\0';

    return OK;
}

error function(int num, char *argv[], size_t buf, char *line_c) {
    int how_random_str;
    int my_len_for_c = 0;
    int strings[num];
    for (int i = 0; i < num; ++i) {
        strings[i] = 0;
    }

    how_random_str = num;
    int random;
    int count = 0;

    while (count != num) {
        random = 3 + rand() % how_random_str;

        if (strings[random - 3] != 1) {
            if (concatenation(argv[random], line_c, &my_len_for_c, &buf) != OK) {
                return MEMORY_ERROR;
            } else {
                ++count;
                strings[random - 3] = 1;
            }
        }
    }

    return OK;
}