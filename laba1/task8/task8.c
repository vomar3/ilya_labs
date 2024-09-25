#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long int str_to_llint(char* str, long long int* number, int system);

int main(int args, char* argv[]) {
    if (args != 3) {
        printf("Input error\n");
        return -1;
    }

    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

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
    char* number = (char*)calloc(100, sizeof(char));

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
            if (symbol == '0' && count == 0)
                continue;

            number[count] = (char)(symbol);
            count += 1;

            if ((char)symbol >= '0' && (char)symbol <= '9' &&
                symbol - 47 > max_system) {
                max_system = symbol - 47;
            } else if ((char)symbol >= 'a' && (char)symbol <= 'z' &&
                       symbol - 86 > max_system) {
                max_system = symbol - 86;
            } else if ((char)symbol < '0' || (char)symbol > 'z') {
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

            number[count] = '\0';

        } else {
            if (count != 0 && max_system != -1 && number != NULL) {
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

            count = 0;
            max_system = -1;
        }
    }

    fclose(input);
    fclose(output);
    free(number);

    return 0;
}

long long int str_to_llint(char* str, long long int* number, int system) {
    char* end = NULL;
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