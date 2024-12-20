#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

typedef enum ERRORS{
    OK,
    INVALID_INPUT,
    MEMORY_ERROR,
    NUMBER_OVERFLOW,
    ZERO_DIVISION,
    PROBLEMS_WITH_FILE,
} error;

typedef struct String {
    char *arr;
    int capacity;
    int len;
} String;

typedef struct MemoryCell {
    String name;
    int value;
} MemoryCell;

typedef struct Variables {
    int capacity;
    int size;
    MemoryCell *array;
} Variables;

error string_to_int(const char *str, int *result);

error string_init(String *string);

error delete_string_content(String *string);

int string_compare(const String *str_1, const String *str_2);

int string_equivalence_relation(const String *str_1, const String *str_2);

String *string_destroy(String *str);

error add_char_to_string(String *str, char ch);

error read_word(FILE *input, String *str);

error read_string(FILE *input, String *str);

MemoryCell *search_variable(Variables *vars, const char *name);

void delete_memory_cell(MemoryCell *mc);

void delete_variables(Variables *vars);

error execute_operation(int var_1, int var_2, char op, int *res);

int compare(const void *a, const void *b);

error add_variable(Variables *vars, MemoryCell *var);

error execute_expression(String *expression, Variables *vars);

error init_variables(Variables *vars);

error print_var(const char *var_name, Variables *vars);

error print_all(Variables *vars);

int main(int argc, char *argv[]) {
    Variables vars;
    FILE *input;
    String line;
    error err;
    int number_of_string = 0, i;
    char input_path[PATH_MAX];
    const char *in_path;

    if (argc != 2 || argv[0] == NULL || argv[1] == NULL) {
        printf("Invalid input\n");
        return INVALID_INPUT;
    }

    in_path = (const char *) realpath(argv[1], input_path);

    if (!in_path) {
        perror("realpath");
        return PROBLEMS_WITH_FILE;
    }

    input = fopen(argv[1], "r");
    if (!input) {
        printf("File open error\n");
        return PROBLEMS_WITH_FILE;
    }

    if (init_variables(&vars) == MEMORY_ERROR) {
        printf("Memory error\n");
        return MEMORY_ERROR;
    }

    while (!feof(input)) {

        number_of_string++;
        if (err = read_string(input, &line))
            break;

        if (line.len == 0)
            break;

        if (!strcmp(line.arr, "print")) {
            printf("print all: ");
            print_all(&vars);
            putchar('\n');
        } else if (strstr(line.arr, "print")) {

            String var_name;
            if (err = string_init(&var_name))
                break;

            for (i = 5; i < line.len; ++i) {
                if (err = add_char_to_string(&var_name, line.arr[i])) {
                    delete_string_content(&var_name);
                    break;
                }
            }

            if (err) {
                delete_string_content(&var_name);
                break;
            }

            if (err = print_var(var_name.arr, &vars)) {
                delete_string_content(&var_name);
                break;
            }
            delete_string_content(&var_name);
        } else {
            if (err = execute_expression(&line, &vars))
                break;
        }

        delete_string_content(&line);
    }

    delete_string_content(&line);
    delete_variables(&vars);
    fclose(input);

    switch (err) {
        case MEMORY_ERROR:
            printf("Memory error. Line: %u \n", number_of_string);
            return MEMORY_ERROR;
        case INVALID_INPUT:
            printf("Input error. Line: %u \n", number_of_string);
            return INVALID_INPUT;
        case NUMBER_OVERFLOW:
            printf("Overflow error. Line: %u \n", number_of_string);
            return MEMORY_ERROR;
        case ZERO_DIVISION:
            printf("Division by zero. Line: %u \n", number_of_string);
            return ZERO_DIVISION;
        default:
            return OK;
    }

    return OK;
}

error string_to_int(const char *str, int *result) {
    if (!str || !result)
        return MEMORY_ERROR;

    char *endinp;
    long res;
    res = strtol(str, &endinp, 10);

    if (res > INT_MAX || res < INT_MIN)
        return INVALID_INPUT;
    if (*endinp != '\0')
        return INVALID_INPUT;

    *result = res;

    return OK;
}

error string_init(String *string) {
    if (!string)
        return MEMORY_ERROR;

    string->capacity = 1;
    string->len = 0;
    if (!(string->arr = (char *) malloc(sizeof(char))))
        return MEMORY_ERROR;

    string->arr[0] = '\0';

    return OK;
}

error delete_string_content(String *string) {
    if (!string)
        return MEMORY_ERROR;

    free(string->arr);
    string->arr = NULL;
    string->len = 0;
    string->capacity = 0;

    return OK;
}

int string_compare(const String *str_1, const String *str_2) {
    if (str_1->len != str_2->len)
        return str_1->len - str_2->len;

    return strcmp(str_1->arr, str_2->arr);
}

int string_equivalence_relation(const String *str_1, const String *str_2) {
    return strcmp(str_1->arr, str_2->arr) == 0;
}

error string_dup(String *dest, const String *sourse) {
    if (!dest || !sourse)
        return MEMORY_ERROR;

    dest->capacity = sourse->capacity;
    dest->len = sourse->len;

    if (!sourse->arr) {
        dest->arr = NULL;
        return OK;
    }

    dest->arr = (char *) malloc(sourse->capacity * sizeof(char));
    if (!dest->arr)
        return MEMORY_ERROR;
    strcpy(dest->arr, sourse->arr);

    return OK;
}

String *string_destroy(String *str) {
    if (!str)
        return NULL;

    free(str->arr);
    free(str);

    return NULL;
}

error add_char_to_string(String *str, char ch) {
    if (!str)
        return MEMORY_ERROR;

    char *for_realloc;

    if (str->capacity == 0) {
        if (string_init(str))
            return MEMORY_ERROR;
    }

    if (str->capacity == str->len + 1) {
        str->capacity *= 2;
        for_realloc = (char *) realloc(str->arr, str->capacity * sizeof(char));
        if (!for_realloc)
            return MEMORY_ERROR;

        str->arr = for_realloc;
    }

    str->arr[str->len++] = ch;
    str->arr[str->len] = '\0';

    return OK;
}

error read_word(FILE *input, String *str) {
    if (!input || !str)
        return MEMORY_ERROR;

    char ch;

    if (string_init(str) == MEMORY_ERROR)
        return MEMORY_ERROR;

    ch = getc(input);
    while (ch != EOF && isalnum(ch)) {
        if (add_char_to_string(str, ch))
            return MEMORY_ERROR;

        ch = getc(input);
    }

    return OK;
}

error read_string(FILE *input, String *str) {
    if (!input || !str)
        return MEMORY_ERROR;

    char ch;

    if (string_init(str) == MEMORY_ERROR)
        return MEMORY_ERROR;

    ch = getc(input);

    while (ch != EOF && ch != ';') {
        if (!isspace(ch)) {
            if (add_char_to_string(str, ch))
                return MEMORY_ERROR;
        }
        ch = getc(input);
    }

    return OK;
}

MemoryCell *search_variable(Variables *vars, const char *name) {
    if (!vars || !name)
        return NULL;

    int left = 0, mid, res;
    int right = vars->size - 1;

    while (left <= right) {
        mid = left + (right - left) / 2;
        res = strcmp(vars->array[mid].name.arr, name);
        if (res == 0) {
            return &vars->array[mid];
        }

        if (res < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return NULL;
}

void delete_memory_cell(MemoryCell *mc) {
    if (!mc)
        return;

    delete_string_content(&mc->name);
    mc->value = 0;
}

void delete_variables(Variables *vars) {
    if (!vars)
        return;

    int i;

    for (i = 0; i < vars->size; ++i) {
        delete_memory_cell(&vars->array[i]);
    }

    free(vars->array);
    vars->size = 0;
    vars->capacity = 0;
    vars->array = NULL;
}

error execute_operation(int var_1, int var_2, char op, int *res) {
    if (!res)
        return MEMORY_ERROR;

    if (op == '+') {
        if (var_1 > 0 && INT_MAX - var_1 < var_2)
            return NUMBER_OVERFLOW;
        else if (var_1 < 0 && INT_MIN - var_1 > var_2)
            return NUMBER_OVERFLOW;

        *res = var_1 + var_2;
    } else if (op == '-') {
        var_2 *= -1;
        if (var_1 > 0 && INT_MAX - var_1 < var_2)
            return NUMBER_OVERFLOW;
        else if (var_1 < 0 && INT_MIN - var_1 > var_2)
            return NUMBER_OVERFLOW;

        *res = var_1 + var_2;
    } else if (op == '*') {
        *res = var_1 * var_2;
        if (var_1 != 0 && *res / var_1 != var_2)
            return NUMBER_OVERFLOW;
    } else if (op == '/') {
        if (var_2 == 0)
            return ZERO_DIVISION;

        *res = var_1 / var_2;
    } else if (op == '%'){
        if (var_2 == 0)
            return ZERO_DIVISION;
        *res = var_1 % var_2;
    }

    return OK;
}

int compare(const void *a, const void *b) {
    MemoryCell *mc1 = (MemoryCell *)a;
    MemoryCell *mc2 = (MemoryCell *)b;
    return strcmp(mc1->name.arr, mc2->name.arr);
}

error add_variable(Variables *vars, MemoryCell *var) {
    if (!vars || !var)
        return MEMORY_ERROR;

    if (vars->capacity == vars->size) {
        MemoryCell *for_realloc;
        vars->capacity *= 2;
        for_realloc = (MemoryCell *)realloc(vars->array, vars->capacity * sizeof(MemoryCell));
        if (!for_realloc)
            return MEMORY_ERROR;

        vars->array = for_realloc;
    }

    if (string_dup(&vars->array[vars->size].name, &var->name))
        return MEMORY_ERROR;
    vars->array[vars->size++].value = var->value;

    qsort(vars->array, vars->size, sizeof(MemoryCell), compare);

    return OK;
}

error execute_expression(String *expression, Variables *vars) {
    if (!expression || !vars)
        return MEMORY_ERROR;

    String var_1, var_2;
    MemoryCell result, *mc_var1, *mc_var2;
    int idx = 0;
    error state;
    int var_1_num, var_2_num;
    char operation, flag_second_var;
    var_2.arr = NULL;

    if (string_init(&result.name))
        return MEMORY_ERROR;

    while (isalpha(expression->arr[idx])) {
        if (add_char_to_string(&result.name, expression->arr[idx++])) {
            delete_string_content(&result.name);
            delete_memory_cell(&result);
            return MEMORY_ERROR;
        }
    }

    idx++;

    if (string_init(&var_1)) {
        delete_string_content(&result.name);
        delete_memory_cell(&result);
        return MEMORY_ERROR;
    }

    while (isalnum(expression->arr[idx])) {
        if (add_char_to_string(&var_1, expression->arr[idx++])) {
            delete_string_content(&var_1);
            delete_memory_cell(&result);
            return MEMORY_ERROR;
        }
    }

    if (isdigit(var_1.arr[0])) {
        if (state = string_to_int(var_1.arr, &var_1_num)) {
            delete_memory_cell(&result);
            delete_string_content(&var_1);
            return state;
        }
    } else {
        mc_var1 = search_variable(vars, var_1.arr);
        if (!mc_var1) {
            delete_string_content(&var_1);
            delete_memory_cell(&result);
            return INVALID_INPUT;
        }
        var_1_num = mc_var1->value;
    }
    result.value = var_1_num;

    if (expression->arr[idx] != '\0') {
        flag_second_var = 1;
        operation = expression->arr[idx++];

        if (string_init(&var_2))
        {
            delete_memory_cell(&result);
            delete_string_content(&var_1);
            return MEMORY_ERROR;
        }

        while (isalnum(expression->arr[idx])){
            if (add_char_to_string(&var_2, expression->arr[idx++])) {
                delete_memory_cell(&result);
                delete_string_content(&var_1);
                delete_string_content(&var_2);
                return MEMORY_ERROR;
            }
        }

        if (isdigit(var_2.arr[0])) {
            if (state = string_to_int(var_2.arr, &var_2_num)) {
                delete_memory_cell(&result);
                delete_string_content(&var_1);
                delete_string_content(&var_2);
                return state;
            }
        } else {
            mc_var2 = search_variable(vars, var_2.arr);
            if (!mc_var2) {
                delete_string_content(&var_1);
                delete_string_content(&var_2);
                delete_memory_cell(&result);
                return INVALID_INPUT;
            }
            var_2_num = mc_var2->value;
        }

        if ((state = execute_operation(var_1_num, var_2_num, operation, &result.value)) != OK) {
            delete_string_content(&var_1);
            delete_string_content(&var_2);
            delete_memory_cell(&result);
            return state;
        }
    }

    mc_var1 = search_variable(vars, result.name.arr);

    if (!mc_var1) {
        if (add_variable(vars, &result)) {
            delete_string_content(&var_1);
            delete_string_content(&var_2);
            delete_memory_cell(&result);
            return MEMORY_ERROR;
        }
    } else {
        mc_var1->value = result.value;
    }

    delete_memory_cell(&result);
    delete_string_content(&var_1);
    delete_string_content(&var_2);

    return OK;
}

error init_variables(Variables *vars) {
    if (!vars)
        return MEMORY_ERROR;

    vars->array = (MemoryCell *)malloc(sizeof(MemoryCell) * 2);
    if (!vars->array)
        return MEMORY_ERROR;

    vars->capacity = 2;
    vars->size = 0;

    return OK;
}

error print_var(const char *var_name, Variables *vars) {
    if (!vars || !var_name)
        return MEMORY_ERROR;

    MemoryCell *var;

    var = search_variable(vars, var_name);
    if (!var)
        return INVALID_INPUT;
    printf("%d\n", var->value);

    return OK;
}

error print_all(Variables *vars) {
    if (!vars)
        return MEMORY_ERROR;

    int i;

    for (i = 0; i < vars->size; ++i) {
        printf("%s = %d; ", vars->array[i].name.arr, vars->array[i].value);
    }

    return OK;
}