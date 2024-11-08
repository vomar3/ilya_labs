#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

typedef enum ERRORS{
    OK,
    INVALID_INPUT,
    PROBLEMS_WITH_FILE,
} error;

error printf_tree(FILE *input, FILE *output);

int main(int args, char *argv[]) {

    int symbol;
    char input_path[PATH_MAX], output_path[PATH_MAX];
    FILE *input = NULL, *output = NULL;
    const char *in_path, *out_path;

    if (args != 3 || argv[0] == NULL || argv[1] == NULL || argv[2] == NULL) {
        printf("Invalid input\n");
        return INVALID_INPUT;
    }

    in_path = (const char *) realpath(argv[1], input_path);
    out_path = (const char *) realpath(argv[2], output_path);

    if (!in_path || !out_path) {
        perror("realpath");
        return PROBLEMS_WITH_FILE;
    }

    if (strcmp((in_path), out_path) == 0) {
        printf("The files are the same\n");
        return PROBLEMS_WITH_FILE;
    }

    input = fopen(argv[1], "r");
    output = fopen(argv[2], "w");
    if (input == NULL || output == NULL) {
        if (input != NULL) fclose(input);
        if (output != NULL) fclose(output);
        printf("File == NULL\n");
        return PROBLEMS_WITH_FILE;
    }

    int count = 1;
    while (!feof(input)) {
        fprintf(output, "Tree with number %d:\n\n", count);
        if (printf_tree(input, output) != OK) {
            printf("Error with input\n");
            fclose(input);
            fclose(output);
            return PROBLEMS_WITH_FILE;
        }

        fprintf(output, "\n");
        count++;
    }

    fprintf(output, "End of input trees\n");

    fclose(input);
    fclose(output);

    return OK;
}

error printf_tree(FILE *input, FILE *output) {
    if (!input || !output) {
        return PROBLEMS_WITH_FILE;
    }

    char symbol = fgetc(input);
    int count = 0, i;

    while (symbol != '\n' && symbol != EOF) {
        if (symbol == '(') {
            ++count;
        } else if (symbol == ')') {
            if (count != 0) {
                --count;
            } else {
                return INVALID_INPUT;
            }
        }
        else if (symbol == ',' || symbol == ' ') {
            symbol = fgetc(input);
            continue;
        } else {
            for (i = 0; i < count; ++i) {
                fprintf(output, "\t");
            }

            if (!isspace(symbol)) {
                fprintf(output, "->%c\n", symbol);
            }
        }

        symbol = fgetc(input);
    }

    return OK;
}