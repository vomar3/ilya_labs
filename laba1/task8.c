#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <ctype.h>

enum ERRORS{
    OK,
    INPUT_ERROR,
    OVERFLOW,
    FILE_ERROR,
};

long long int str_to_llint(const char *str, long long int *number, int system);

int main(int args, char* argv[]) {

    if (args != 3) {
        printf("Input error\n");
        return -1;
    }

    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    if (input == NULL || output == NULL) {
        printf("Problems with open file\n");
        if (input != NULL) {
            fclose(input);
        }
        if (output != NULL) {
            fclose(output);
        }

        return -3;
    }

    int symbol;
    int count = 0;
    int max_system = -1;
    char *number = malloc(10 * sizeof(char));

    if (number == NULL) {
        printf("Memory allocation error\n");
        free(number);

        if (input != NULL) {
            fclose(input);
        }
        if (output != NULL) {
            fclose(output);
        }
        return -4;
    }

    while (!feof(input)) {
        symbol = fgetc(input);

        if (!isspace(symbol) && symbol != EOF) {

            if (symbol == '0' && count == 0) continue;

            number[count] = (char)(symbol);
            count += 1;

            if ((char)symbol >= '0' && (char)symbol <= '9' && symbol - 47 > max_system) {
                max_system = symbol - 47;
            } else if ((char)symbol >= 'a' && (char)symbol <= 'z' && symbol - 86 > max_system) {
                max_system = symbol - 86;
            } else if ((char)symbol < '0' || (char)symbol > 'z'){
                printf("%c %d\n", (char)symbol, symbol);
                printf("Input error\n");

                if (input != NULL) {
                    fclose(input);
                }
                if (output != NULL) {
                    fclose(output);
                }
                free(number);
                return -1;
            }

            char *new_number = realloc(number, (sizeof(number) + 1) * sizeof(char));
            if (new_number == NULL) {
                printf("Memory error\n");
                if (input != NULL) {
                    fclose(input);
                }
                if (output != NULL) {
                    fclose(output);
                }
                free(number);
                return -5;
            }

            number = new_number;

        } else {
            if (count != 0 && max_system != -1) {

                long long int answer = 0;

                if (str_to_llint(number, &answer, max_system) != -2) {
                    fprintf(output, "%s %d %lli\n", number, max_system, answer);
                } else {
                    free(number);
                    if (input != NULL) {
                        fclose(input);
                    }
                    if (output != NULL) {
                        fclose(output);
                    }
                    return -2;
                }
            }

            free(number);
            number = calloc(0, 100);
            count = 0;
            max_system = -1;
        }
    }

    fclose(input);
    fclose(output);

    free(number);

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