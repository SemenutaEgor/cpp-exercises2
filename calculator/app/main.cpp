#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "../mathlib/mathlib.h"

void print_help() {
    printf("Usage:\n");
    printf("    calcucator <a> <op> <b>\n");
    printf("    or\n");
    printf("    calculator <n> !\n");
    printf("Operations:\n");
    printf("    + addition\n");
    printf("    - subtraction\n");
    printf("    x multiplication\n");
    printf("    / division\n");
    printf("    ^ power\n");
    printf("    ! factorial\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_help();
        return 1;
    }

    if (strcmp(argv[1], "--help") == 0) {
        print_help();
        return 0;
    }

    if (argc != 3 && argc != 4) {
        print_help();
        return 1;
    }

    const char *op = argv[2];
    int result{};

    if (argc == 4) {
        int a = atoi(argv[1]);
        int b = atoi(argv[3]);

        if (strcmp(op, "+") == 0) {
            result = add(a, b);
        } else if (strcmp(op, "-") == 0) {
            result = sub(a, b);
        } else if (strcmp(op, "x") == 0) {
            result = mul(a, b);
        } else if (strcmp(op, "/") == 0) {
            int error{};
            result = divide(a, b, &error);
            if (error) {
                printf("Error: division by zero\n");
                return 1;
            }
        } else if (strcmp(op, "^") == 0) {
            result = pow(a, b);
        } else {
            printf("Unknown operation\n");
            return 1;
        }
        printf("%d\n", result);
        return 0;
    }

    if (argc == 3) {
        int n = atoi(argv[1]);

        if (strcmp(op, "!") != 0) {
            printf("Unknown operation\n");
            return 1;
        }

        result = fact(n);
        printf("%d\n", result);
    }

    return 0;
}