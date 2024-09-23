#include <stdio.h>
#include <ctype.h>

enum ERRORS{
    OK,
    INPUT_ERROR,
    FILE_ERROR,
};

int main(int args, char *argv[]) {

    if (args > 4 || args < 3) {
        printf("input error\n");
        return INPUT_ERROR;
    }

    if(!(argv[1][0] == '-' || argv[1][0] == '/')) {
        printf("input error\n");
        return INPUT_ERROR;
    }

    FILE *input = fopen(argv[2], "r");
    FILE *output = fopen("output.txt", "w");

    if (input == NULL || output == NULL) {
        printf("Problems with open file\n");
        if (input != NULL) {
            fclose(input);
        }
        if (output != NULL) {
            fclose(output);
        }

        return FILE_ERROR;
    }

    int symbol;

    if (argv[1][1] == 'n') {
        if ((argv[1][2] != 'd' && argv[1][2] != 'i' && argv[1][2] != 's' && argv[1][2] != 'a') || argv[1][3] != '\0') {
            printf("input error\n");
            return INPUT_ERROR;
        } else {
            argv[1][1] = argv[1][2];

        }
    }


    switch (argv[1][1]) {

        case 'n':
            if (argv[1][2] != 'd' && argv[1][2] != 'i' && argv[1][2] != 's' && argv[1][2] != 'a') {
                printf("input error\n");
                return INPUT_ERROR;
            }

            if (argv[1][3] != '\0') {
                printf("input error\n");
                return INPUT_ERROR;
            }

            break;
        case 'd':

            if (argv[1][2] != '\0') {
                printf("input error\n");
                return INPUT_ERROR;
            }

            while (!feof(input)) {
                symbol = fgetc(input);

                if (symbol != EOF) {

                    if ((char)symbol >= '0' && (char)symbol <= '9') continue;
                    else {
                        fprintf(output,"%c", (char)symbol);
                    }
                }
            }

            break;
        case 'i':
            if (argv[1][2] != '\0') {
                printf("input error\n");
                return INPUT_ERROR;
            }

            int count_letter = 0;
            while (!feof(input)) {
                symbol = fgetc(input);

                if (symbol != EOF) {

                    if (((char)symbol >= 'a' && (char)symbol <= 'z') || ((char)symbol >= 'A' && (char)symbol <= 'Z')) {
                        count_letter++;
                    }
                    else if (symbol == '\n'){
                        fprintf(output,"%d\n", count_letter);
                        count_letter = 0;
                    }
                } else {
                    fprintf(output,"%d\n", count_letter);
                    count_letter = 0;
                }
            }

            break;
        case 's':

            if (argv[1][2] != '\0') {
                printf("input error\n");
                return INPUT_ERROR;
            }

            int count = 0;
            while (!feof(input)) {
                symbol = fgetc(input);

                if (symbol != EOF) {

                    if (!(isalpha(symbol) || isdigit(symbol) || symbol == ' ' || symbol == '\n')) {
                        count++;
                    }

                    if (symbol == '\n'){
                        fprintf(output,"%d\n", count);
                        count = 0;
                    }
                } else {
                    fprintf(output,"%d\n", count);
                    count = 0;
                }
            }

            break;
        case 'a':
            if (argv[1][2] != '\0') {
                printf("input error\n");
                return INPUT_ERROR;
            }

            while (!feof(input)) {
                symbol = fgetc(input);

                if (symbol != EOF) {
                    if ((isdigit(symbol))) {
                        fprintf(output,"%c", (char)symbol);
                    } else {
                        fprintf(output, "%X", symbol);
                    }
                }
            }
            break;
    }

    fclose(input);
    fclose(output);

    return OK;
}