#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum ERRORS{
    OK,
    INPUT_ERROR,
    FILE_ERROR,
};

void str_to_others(int number, int system, FILE *my_File);

int main(int args, char *argv[]) {

    if (args < 5) {
        printf("Invalid input\n");
        return INPUT_ERROR;
    }

    if (! ((argv[1][0] == '/' || argv[1][0] == '-') && (argv[1][2] == '\0'))) {
        printf("Invalid input\n");
        return INPUT_ERROR;
    }

    FILE *file1 = fopen(argv[2], "r");
    FILE *file2 = fopen(argv[3], "r");
    FILE *output = fopen(argv[args - 1], "w");

    if (file1 == NULL || file2 == NULL || output == NULL) {
        printf("Invalid input\n");
        if (file1 != NULL) fclose(file1);
        if (file2 != NULL) fclose(file2);
        if (output != NULL) fclose(output);

        return FILE_ERROR;
    }

    int count_r = 1, symbol, last = 0, f_eof = 0, s_eof = 0;
    int count_a = 1, check_last_space = -1;
    switch (argv[1][1]) {
        case 'r':

            while (!feof(file1) || !feof(file2)) {
                if (count_r % 2 == 0) {
                    symbol = fgetc(file2);

                    if (symbol == EOF) {
                        if (f_eof == 0){
                            f_eof++;
                            fprintf(output, " ");
                        }
                        count_r++;
                        continue;
                    }

                    if (!isspace(symbol)) {
                        fprintf(output, "%c", (char)symbol);
                        last = -1;
                    } else {
                        if (last == -1) {
                            count_r++;
                            fprintf(output, " ");
                            last = 0;
                        }
                    }
                } else if (count_r % 2 != 0) {
                    symbol = fgetc(file1);

                    if (symbol == EOF) {
                        if (s_eof == 0) {
                            s_eof++;
                            fprintf(output, " ");
                        }
                        count_r++;
                        continue;
                    }

                    if (!isspace(symbol)) {
                        fprintf(output, "%c", (char)symbol);
                        last = -1;
                    } else {
                        if (last == -1) {
                            count_r++;
                            fprintf(output, " ");
                            last = 0;
                        }
                    }
                }
            }

            break;
        case 'a':

            while (!feof(file2)) {
                symbol = fgetc(file2);

                if (symbol == EOF) continue;

                if (!isspace(symbol)){
                    if (count_a % 10 == 0) {
                        if (isalpha(symbol) && (symbol >= 65 && symbol <= 90)) symbol += 32;

                        str_to_others(symbol, 4, output);
                        check_last_space = 0;
                    } else if (count_a % 2 == 0) {
                        if (isalpha(symbol) && (symbol >= 65 && symbol <= 90)) symbol += 32;
                        fprintf(output, "%c", symbol);
                        check_last_space = 0;
                    } else if (count_a % 5 == 0) {
                        str_to_others(symbol, 8, output);
                        check_last_space = 0;
                    } else {
                        fprintf(output, "%c", symbol);
                        check_last_space = 0;
                    }
                } else {
                    if (check_last_space == 0) {
                        ++count_a;
                        fprintf(output, " ");
                        check_last_space = -1;
                    }
                }
            }

            break;
        default:
            printf("Invalid input\n");
            return INPUT_ERROR;
    }

    fclose(file1);
    fclose(file2);
    fclose(output);

    return OK;
}

void str_to_others(int number, int system, FILE *my_File) {

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
            free(array);
            flag = 1;
            number = 0;
        }

        array = new_check;
    }

    if (flag == 0) {
        for (int i = count - 1; i >= 0; --i) {
            fprintf(my_File, "%c", array[i]);
        }
    }

    free(array);
}

