#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

typedef enum ERRORS{
    OK,
    INVALID_INPUT,
    PROBLEMS_WITH_FILE,
    MEMORY_ERROR,
} error;

typedef struct Employee{
    unsigned int id;
    char name[100];
    char surname[100];
    double salary;
} Employee;

#define EPS 1e-7

error read_Employers(int *size_struct, Employee **employers, FILE *input, int *count);

error check_dates_struct(Employee info);

int compare_for_a(const void *a, const void *b);

int main(int args, char *argv[]){
    if (args != 4 || argv[2][2] != '\0' || (argv[2][0] != '/' && argv[2][0] != '-')) {
        printf("Invalid input\n");
        return INVALID_INPUT;
    }

    char input_path[PATH_MAX], output_path[PATH_MAX];
    if (!realpath(argv[1], input_path) || !realpath(argv[3], output_path)) {
        perror("realpath");
        return PROBLEMS_WITH_FILE;
    }

    if (strcmp(input_path, output_path) == 0) {
        printf("Identical files\n");
        return PROBLEMS_WITH_FILE;
    }

    int size = 2;
    int count = 0;
    Employee *employers = (Employee *) malloc(size * sizeof(Employee));
    if (employers == NULL) {
        perror("employers");
        return MEMORY_ERROR;
    }

    FILE *input = fopen(input_path, "r");
    FILE *output = fopen(output_path, "w");

    if (input == NULL || output == NULL) {
        perror("fopen");
        if (input != NULL) fclose(input);
        if (output != NULL) fclose(output);
        free(employers);
        return PROBLEMS_WITH_FILE;
    }

    switch (argv[2][1]) {
        case 'a':
            if (read_Employers(&size, &employers, input, &count) == OK) {
                qsort(employers, count, sizeof(Employee), compare_for_a);
                for (int i = 0; i < count; ++i) {
                    fprintf(output, "%u %s %s %lf\n"
                            , employers[i].id, employers[i].name, employers[i].surname, employers[i].salary);
                }
            } else {
                printf("Invalid input\n");
                fclose(input);
                fclose(output);
                free(employers);
                return INVALID_INPUT;
            }
            break;
        case 'd':
            if (read_Employers(&size, &employers, input, &count) == OK) {
                qsort(employers, count, sizeof(Employee), compare_for_a);
                for (int i = count - 1; i >= 0; --i) {
                    fprintf(output, "%u %s %s %lf\n"
                            , employers[i].id, employers[i].name, employers[i].surname, employers[i].salary);
                }
            } else {
                printf("Invalid input\n");
                fclose(input);
                fclose(output);
                free(employers);
                return INVALID_INPUT;
            }
            break;
        default:
            printf("Invalid input\n");
            break;
    }

    fclose(input);
    fclose(output);
    free(employers);

    return OK;
}

error read_Employers(int *size_struct, Employee **employers, FILE *input, int *count) {
    int check_reads_file;

    while ((check_reads_file = fscanf(input, "%u%s%s%lf\n", &(*employers)[*count].id,
                                      (*employers)[*count].name, (*employers)[*count].surname,
                                      &(*employers)[*count].salary)) == 4) {

        if (check_dates_struct((*employers)[*count]) != OK)
            return INVALID_INPUT;

        ++(*count);
        if (*count == *size_struct) {
            *size_struct *= 2;
            Employee *new_struct = (Employee *)realloc(*employers, *size_struct * sizeof(Employee));
            if (new_struct == NULL) {
                free(*employers);
                return MEMORY_ERROR;
            }
            *employers = new_struct;
        }
    }

    if (check_reads_file < 4 && check_reads_file > 0) return INVALID_INPUT;
    return OK;
}

error check_dates_struct(Employee info) {
    if (info.salary < 0) return INVALID_INPUT;

    int size = strlen(info.name);
    for (int i = 0; i < size; ++i) {
        if (info.name[i] < 'A' || info.name[i] > 'z' || (info.name[i] > 'Z' && info.name[i] < 'a'))
            return INVALID_INPUT;
    }

    size = strlen(info.surname);
    for (int i = 0; i < size; ++i) {
        if (info.surname[i] < 'A' || info.surname[i] > 'z' || (info.surname[i] > 'Z' && info.surname[i] < 'a'))
            return INVALID_INPUT;
    }

    return OK;
}

int compare_for_a(const void *a, const void *b) {
    const Employee *first = (const Employee *)a;
    const Employee *second = (const Employee *)b;

    if (first->salary - second->salary > EPS)
        return 1;
    else if (second->salary - first->salary > EPS)
        return -1;

    int answer = strcmp(first->surname, second->surname);
    if (answer > 0)
        return 1;
    else if(answer < 0)
        return -1;

    answer = strcmp(first->name, second->name);
    if (answer > 0)
        return 1;
    else if(answer < 0)
        return -1;

    return first->id - second->id;
}