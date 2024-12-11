#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef enum ERRORS{
    OK,
    INVALID_INPUT,
    PROBLEM_WITH_FILE,
    MEMORY_ERROR,
} error;

typedef struct Found {
    char *file_name;
    int n_line;
    int n_char;
    struct Found *next;
} found;

int my_strlen(const char *str);

error add_new_found(found **head, char *file_name, int len_filename, int n_line, int n_char);

error search_str_in_files(char *substring, int count_files, found **result, ...);

int main() {
    found *result, *current;
    char flag = '1';

    error error_code = search_str_in_files("dd\ndimon", 1, &result, "input.txt");

    switch (error_code) {
        case PROBLEM_WITH_FILE:
            printf("Some files could not be opened\n");
            return PROBLEM_WITH_FILE;
        case MEMORY_ERROR:
            printf("Memory error\n");
            return MEMORY_ERROR;
        case INVALID_INPUT:
            printf("Input error\n");
            return INVALID_INPUT;
        default:
            current = result;
            while (current) {
                found *next = current->next;
                flag = 0;
                printf("File: %s line: %d, char: %d;\n", current->file_name, current->n_line, current->n_char);
                free(current->file_name);
                free(current);
                current = next;
            }

            if (flag) {
                printf("Substring not found\n");
            }
    }

    return OK;
}

int my_strlen(const char *str) {
    const char *end = str;
    while (*end++);
    return end - str - 1;
}

error add_new_found(found **head, char *file_name, int len_filename, int n_line, int n_char) {
    if (!head || !file_name)
        return INVALID_INPUT;

    int i;
    found *new_elem = (found *) malloc(sizeof(found));
    if (!new_elem)
        return MEMORY_ERROR;

    new_elem->file_name = (char *) malloc((len_filename + 1) * sizeof(char));
    if (!new_elem->file_name) {
        free(new_elem);
        return MEMORY_ERROR;
    }

    for (i = 0; i <= len_filename; i++) {
        new_elem->file_name[i] = file_name[i];
    }

    new_elem->n_char = n_char;
    new_elem->n_line = n_line;
    new_elem->next = NULL;

    if (!(*head)) {
        *head = new_elem;
    } else {
        found *current = (*head);
        while (current->next) {
            current = current->next;
        }
        current->next = new_elem;
    }
    return OK;
}

error search_str_in_files(char *substring, int count_files, found **result, ...) {
    if (!substring || !result)
        return INVALID_INPUT;

    int len_substr = my_strlen(substring), i, len_filename, j;
    char c, *name_file;
    FILE *file;
    int idx_substr, n_line, n_char, n_line_answ, n_char_answ, count_new_str;

    *result = NULL;
    if (count_files < 1 || !len_substr)
        return INVALID_INPUT;

    va_list args;
    va_start(args, result);

    for (i = 0; i < count_files; i++) {
        n_line = 1, n_char = 0;
        name_file = va_arg(args, char *);
        len_filename = my_strlen(name_file);
        if (!(file = fopen(name_file, "r"))) {
            va_end(args);
            return PROBLEM_WITH_FILE;
        }

        idx_substr = 0;
        while ((c = getc(file)) != EOF) {
            n_char++;
            if (c == '\n') {
                n_line++;
            }
            if (c == substring[idx_substr]) {
                idx_substr++;
                if (idx_substr == 1) {
                    n_line_answ = n_line;
                    n_char_answ = n_char;
                }
                if (idx_substr == len_substr) {
                    if (add_new_found(result, name_file, len_filename, n_line_answ, n_char_answ) == MEMORY_ERROR)
                        return MEMORY_ERROR;

                    fseek(file, -idx_substr + 1, SEEK_CUR);
                    count_new_str = 0;
                    for (j = 0; j < idx_substr; j++) {
                        if (substring[j] == '\n')
                            count_new_str++;
                    }
                    n_char -= (idx_substr - 1);
                    n_line -= count_new_str;
                    idx_substr = 0;
                }
            } else if (idx_substr) {
                fseek(file, -idx_substr + 1, SEEK_CUR);
                count_new_str = 0;
                for (j = 0; j < idx_substr; j++) {
                    if (substring[j] == '\n')
                        count_new_str++;
                }
                if (c == '\n')
                    count_new_str++;
                n_char -= (idx_substr - 1);
                idx_substr = 0;
                n_line -= count_new_str;
            }
            if (c == '\n') {
                n_char = 0;
            }
        }
        fclose(file);
    }

    va_end(args);
    return OK;
}