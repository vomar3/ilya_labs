#include <stdio.h>

int main(int args, char *argv[]) {

    if (args > 4 || args < 3) {
        printf("input error\n");
        return -1;
    }

    if(!(argv[1][0] == '-' || argv[1][0] == '/')) {
        printf("input error\n");
        return -1;
    }

    switch (argv[1][1]) {
        case 'n':
            if (argv[1][2] != 'd' && argv[1][2] != 'i' && argv[1][2] != 's' && argv[1][2] != 'a') {
                printf("input error\n");
                return -1;
            }

            if (argv[1][3] != '\0') {
                printf("input error\n");
                return -1;
            }

            break;
        case 'd':
            if (argv[1][2] != '\0') {
                printf("input error\n");
                return -1;
            }
            break;
        case 'i':
            if (argv[1][2] != '\0') {
                printf("input error\n");
                return -1;
            }

            break;
        case 's':
            if (argv[1][2] != '\0') {
                printf("input error\n");
                return -1;
            }

            break;
        case 'a':
            if (argv[1][2] != '\0') {
                printf("input error\n");
                return -1;
            }

            break;
    }

    return 0;
}