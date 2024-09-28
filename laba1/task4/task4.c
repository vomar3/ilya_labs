#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

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
    //FILE *output = fopen("output.txt", "w");
    FILE *output = NULL;

    if (input == NULL) {
        printf("Problems with open file\n");
        fclose(input);
        return FILE_ERROR;
    }

    int symbol;

    if (argv[1][1] == 'n') {
        if ((argv[1][2] != 'd' && argv[1][2] != 'i' && argv[1][2] != 's' && argv[1][2] != 'a') || argv[1][3] != '\0') {
            printf("input error\n");
            return INPUT_ERROR;
        } else {
            argv[1][1] = argv[1][2];
            argv[1][2] = '\0';
            output = fopen(argv[3], "w");

            if (output == NULL) {
                printf("Problems with open file\n");
                fclose(input);
                return FILE_ERROR;
            }
        }
    } else {
        char *output_path = (char *) malloc((strlen(argv[2]) + 5) * sizeof(char));
        if (output_path == NULL) {
            printf("input error\n");
            if (input != NULL) fclose(input);
            free(output_path);
            return INPUT_ERROR;
        }

        char *last = strrchr(argv[2], '/');
        if (last == NULL) {
            strcpy(output_path, "out_");
            strcat(output_path, argv[2]);
            output_path[strlen(argv[2]) + 4] = '\0';
        } else {
            last++;
            unsigned long long int len_without_last = strlen(argv[2]) - strlen(last);
            strncpy(output_path, argv[2], len_without_last);
            strcat(output_path, "out_");
            strcat(output_path, last);
            output_path[strlen(argv[2]) + 4] = '\0';
        }

        output = fopen(output_path, "w");
        if (output == NULL) {
            printf("Problems with open file\n");
            if (input != NULL) fclose(input);
            free(output_path);
            return FILE_ERROR;
        }

        free(output_path);
    }

    switch (argv[1][1]) {

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
        default:
            printf("input error\n");
            if (output != NULL) fclose(output);
            if (input != NULL) fclose(input);
            return INPUT_ERROR;
    }

    fclose(input);
    fclose(output);

    return OK;
}