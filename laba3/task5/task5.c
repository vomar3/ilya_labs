#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

typedef enum ERRORS{
    OK,
    INVALID_INPUT,
    MEMORY_ERROR,
    PROBLEMS_WITH_FILE,
    NOT_FOUND,
} error;

typedef struct Students{
    unsigned int id;
    char name[BUFSIZ];
    char surname[BUFSIZ];
    char group[BUFSIZ];
    unsigned char *marks;
    double average_marks;
} Students;

#define EPS 1e-7

void print_menu();

void free_all(Students *student, int size);

size_t my_strlen(const char *str);

int compare_names(const void *a, const void *b);

int compare_id(const void *a, const void *b);

int compare_groups(const void *a, const void *b);

int compare_surnames(const void *a, const void *b);

error read_file_write_struct(FILE *input, Students **student, int *count, int *size, double *average);

error print_student_in_file(const Students *student, FILE *output);

void print_student_in_console(const Students *student);

error print_FI_in_file(const Students *student, FILE *output);

error search_by_id(const Students *student, int size, unsigned int id);

error search_by_NSG(const Students *student, int size, unsigned int id, char nsg, const char *key);

error search_by_id_with_file(const Students *student, int size, unsigned int id, FILE *out);

int main(int argc, char *argv[]) {

    int struct_size = 2, count = 0, i;
    unsigned int search_id;
    short user_answer;
    char input_path[PATH_MAX], output_path[PATH_MAX], scanf_info[BUFSIZ];
    FILE *input = NULL, *output = NULL;
    double average_students_marks;

    if (argc != 3 || argv[0] == NULL || argv[1] == NULL || argv[2] == NULL) return INVALID_INPUT;

    if (!realpath(argv[1], input_path) || !realpath(argv[2], output_path)) {
        perror("Realpath");
        return PROBLEMS_WITH_FILE;
    }

    if (strcmp(input_path, output_path) == 0) {
        printf("Identical files\n");
        return PROBLEMS_WITH_FILE;
    }

    input = fopen(argv[1], "r");
    output = NULL;

    if (input == NULL) {
        perror("fopen");
        return PROBLEMS_WITH_FILE;
    }

    Students *student = (Students *) malloc(struct_size * sizeof(Students));
    if (student == NULL) {
        perror("student");
        fclose(input);
        //fcloseall(); // Спросить (?)
        return MEMORY_ERROR;
    }

    if (read_file_write_struct(input, &student, &count, &struct_size, &average_students_marks) != OK) {
        printf("Big problems...\n");
        return MEMORY_ERROR;
    }

    double real_average = average_students_marks / (count * 5.);

    print_menu();
    scanf("%hd", &user_answer);
    while (user_answer != 0) {
        switch (user_answer) {
            case 0:
                printf("Bye!\n");
                user_answer = 0;
                break;
            case 1:
                printf("Enter the id\n");
                scanf("%u", &search_id);
                printf("Search by id.. \n");
                if (search_by_id(student, count, search_id) == NOT_FOUND) {
                    printf("No students with this id\n");
                }
                print_menu();
                scanf("%hd", &user_answer);
                break;
            case 2:
                printf("Enter the surname (not longer 512): \n");
                scanf("%s", scanf_info);
                printf("Search by surname.. \n");
                if (search_by_NSG(student, count, search_id, 's', scanf_info) == NOT_FOUND) {
                    printf("No students with this surname\n");
                }
                print_menu();
                scanf("%hd", &user_answer);
                break;
            case 3:
                printf("Enter the name (not longer 512): \n");
                scanf("%s", scanf_info);
                printf("Search by group.. \n");
                if (search_by_NSG(student, count, search_id, 'n', scanf_info) == NOT_FOUND) {
                    printf("No students with this name\n");
                }
                print_menu();
                scanf("%hd", &user_answer);
                break;
            case 4:
                printf("Enter the group (not longer 512): \n");
                scanf("%s", scanf_info);
                printf("Search by group.. \n");
                if (search_by_NSG(student, count, search_id, 'g', scanf_info) == NOT_FOUND) {
                    printf("No students with this group\n");
                }
                print_menu();
                scanf("%hd", &user_answer);
                break;
            case 5:
                printf("Sort by id..\n");
                qsort(student, count, sizeof(Students), compare_id);
                printf("The sorting was completed successfully\n");

                for (i = 0; i < count; ++i) {
                    print_student_in_console(&student[i]);
                }

                print_menu();
                scanf("%hd", &user_answer);
                break;
            case 6:
                printf("Sort by surname..\n");
                qsort(student, count, sizeof(Students), compare_surnames);
                printf("The sorting was completed successfully\n");

                for (i = 0; i < count; ++i) {
                    print_student_in_console(&student[i]);
                }

                print_menu();
                scanf("%hd", &user_answer);
                break;
            case 7:
                printf("Sort by name..\n");
                qsort(student, count, sizeof(Students), compare_names);
                printf("The sorting was completed successfully\n");

                for (i = 0; i < count; ++i) {
                    print_student_in_console(&student[i]);
                }

                print_menu();
                scanf("%hd", &user_answer);
                break;
            case 8:
                printf("Sort by group..\n");
                qsort(student, count, sizeof(Students), compare_groups);
                printf("The sorting was completed successfully\n");

                for (i = 0; i < count; ++i) {
                    print_student_in_console(&student[i]);
                }

                print_menu();
                scanf("%hd", &user_answer);
                break;
            case 9:
                output = fopen(argv[2], "a+");
                if (output == NULL) {
                    free_all(student, count);
                    printf("Error with open file\n");
                    return PROBLEMS_WITH_FILE;
                }

                // printf("%lf", real_average); Для теста

                for (i = 0; i < count; ++i) {
                    if (student[i].average_marks - real_average > EPS) {
                        if (print_FI_in_file(&student[i], output) != OK) {
                            printf("Error with file\n");
                            free_all(student, count);
                            fclose(output);
                            return PROBLEMS_WITH_FILE;
                        }
                    }
                }

                printf("Written to a file\n");
                fclose(output);

                print_menu();
                scanf("%hd", &user_answer);
                break;
            case 10:
                output = fopen(argv[2], "a+");
                if (output == NULL) {
                    free_all(student, count);
                    printf("Error with open file\n");
                    return PROBLEMS_WITH_FILE;
                }

                printf("Enter the id\n");
                scanf("%u", &search_id);
                printf("Search by id.. \n");
                if (search_by_id_with_file(student, count, search_id, output) != OK) {
                    printf("No students with this id\n");
                } else {
                    printf("Written to a file\n");
                }

                fclose(output);

                print_menu();
                scanf("%hd", &user_answer);
                break;
            default:
                printf("Incorrect input, try again\n");
                print_menu();
                if (scanf("%hd", &user_answer) != 1) {
                    printf("Why don't you respect me and my menu?\n");
                    free_all(student, count);
                    return INVALID_INPUT;
                }
                break;
        }
    }

    free_all(student, count);

    return OK;
}

void print_menu() {
    printf("----------------------------------------------------------\n");
    printf("Menu for print: \n");
    printf("0 for exit\n");
    printf("1 for search by id\n");
    printf("2 for search by surname\n");
    printf("3 for search by name\n");
    printf("4 for search by group\n");
    printf("5 for sort by id\n");
    printf("6 for sort by surname\n");
    printf("7 for sort by name\n");
    printf("8 for sort by group\n");
    printf("9 print more then average to trace\n");
    printf("10 print search by id to trace\n");
    printf("----------------------------------------------------------\n");
}

size_t my_strlen(const char *str) {
    char const *end = str;
    while (*end++);
    return end - str - 1;
}

int compare_id(const void *a, const void *b) {
    const Students *first = (const Students *)a;
    const Students *second = (const Students *)b;

    return first->id - second->id;
}

int compare_names(const void *a, const void *b) {
    const Students *first = (const Students *)a;
    const Students *second = (const Students *)b;

    return strcmp(first->name, second->name);
}

int compare_surnames(const void *a, const void *b) {
    const Students *first = (const Students *)a;
    const Students *second = (const Students *)b;

    return strcmp(first->surname, second->surname);
}

int compare_groups(const void *a, const void *b) {
    const Students *first = (const Students *)a;
    const Students *second = (const Students *)b;

    return strcmp(first->group, second->group);
}

error read_file_write_struct(FILE *input, Students **student, int *count, int *size, double *average) {

    int i, sum = 0, num, check_how_read;

    if (input == NULL)
        return PROBLEMS_WITH_FILE;

    if (student == NULL)
        return MEMORY_ERROR;

    while (!feof(input)) {

        if (*count == *size) {
            *size *= 2;
            Students *memory = (Students *) realloc(*student, *size * sizeof(Students));
            if (memory == NULL) {
                free_all(*student, *count);
                fclose(input);
                return MEMORY_ERROR;
            }
            *student = memory;
        }

        (*student)[*count].marks = (unsigned char *) malloc(5 * sizeof(unsigned char));
        if ((*student)[*count].marks == NULL) {
            free_all(*student, *count);
            fclose(input);
            return MEMORY_ERROR;
        }

        if ((check_how_read = fscanf(input, "%d %49s %49s %49s %c %c %c %c %c",
                                         &(*student)[*count].id, (*student)[*count].name, (*student)[*count].surname,
                                         (*student)[*count].group, &(*student)[*count].marks[0],
                                         &(*student)[*count].marks[1],
                                         &(*student)[*count].marks[2], &(*student)[*count].marks[3],
                                         &(*student)[*count].marks[4])) == 9) {

            size_t len = my_strlen((*student)[*count].surname);
            if ((*student)[*count].id <= 0 || len == 0 || my_strlen((*student)[*count].name) == 0
            || my_strlen((*student)[*count].group) == 0) {
                free_all(*student, *count);
                fclose(input);
                return INVALID_INPUT;
            }

            for (i = 0; i < len; ++i) {
                if (!isalpha((*student)[*count].surname[i])) {
                    free_all(*student, *count);
                    fclose(input);
                    return INVALID_INPUT;
                }
            }

            len = my_strlen((*student)[*count].name);
            for (i = 0; i < len; ++i) {
                if (!isalpha((*student)[*count].name[i])) {
                    free_all(*student, *count);
                    fclose(input);
                    return INVALID_INPUT;
                }
            }

            for (i = 0; i < 5; ++i) {
                num = ((*student)[*count].marks[i] - '0');
                if (num > 5 || num < 2) {
                    free_all(*student, *count);
                    fclose(input);
                    return INVALID_INPUT;
                }
                sum += num;
            }
            *average += sum;
            (*student)[*count].average_marks = (double) (sum) / 5;
            sum = 0;

            (*count)++;
        } else if (check_how_read > 0 && check_how_read < 9){
            printf("%d", check_how_read);
            free_all(*student, *count);
            fclose(input);
            return INVALID_INPUT;
        }
    }

    fclose(input);
    return OK;
}

void print_student_in_console(const Students *student) {
    printf("ID: %u\n", student->id);
    printf("surname: %s\n", student->surname);
    printf("name: %s\n", student->name);
    printf("group: %s\n", student->group);
    printf("marks: %c %c %c %c %c\n", student->marks[0], student->marks[1], student->marks[2],
           student->marks[3], student->marks[4]);
}

error print_student_in_file(const Students *student, FILE *output) {
    if (!output) return PROBLEMS_WITH_FILE;

    fprintf(output, "ID: %u\n", student->id);
    fprintf(output, "surname: %s\n", student->surname);
    fprintf(output, "name: %s\n", student->name);
    fprintf(output, "group: %s\n", student->group);
    fprintf(output, "average: %lf\n", student->average_marks);
    return OK;
}

error print_FI_in_file(const Students *student, FILE *output) {
    if (!output) return PROBLEMS_WITH_FILE;

    fprintf(output, "%s ", student->surname);
    fprintf(output, "%s\n", student->name);
    //fprintf(output, "%lf\n", student->average_marks); для теста
    return OK;
}

error search_by_id(const Students *student, int size, unsigned int id) {
    int i;
    if (!student)
        return MEMORY_ERROR;

    for (i = 0; i < size; ++i) {
        if (student[i].id == id) {
            print_student_in_console(&student[i]);
            return OK;
        }
    }

    return NOT_FOUND;
}

error search_by_NSG(const Students *student, int size, unsigned int id, char nsg, const char *key) {
    int i, count = 0;
    if (!student) return MEMORY_ERROR;

    if (nsg != 'n' && nsg != 's' && nsg != 'g') return INVALID_INPUT;

    for (i = 0; i < size; ++i) {
        if ((strcmp(student[i].surname, key) == 0 && nsg == 's')
            || (strcmp(student[i].name, key) == 0 && nsg == 'n')
            || (strcmp(student[i].group, key) == 0 && nsg == 'g')) {
            ++count;
            print_student_in_console(&student[i]);
        }
    }

    if (count != 0)
        return OK;

    return NOT_FOUND;
}

void free_all(Students *student, int size) {
    int i;
    for (i = 0; i <= size; ++i) {
        free(student[i].marks);
        student[i].marks = NULL;
    }
    free(student);
    student = NULL;
}

error search_by_id_with_file(const Students *student, int size, unsigned int id, FILE *out) {
    int i;
    if (!student || !out)
        return MEMORY_ERROR;

    for (i = 0; i < size; ++i) {
        if (student[i].id == id) {
            print_student_in_file(&student[i], out);
            return OK;
        }
    }

    return NOT_FOUND;
}