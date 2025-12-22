#include "mathlib.h"

int add(int lhs, int rhs) noexcept {
    return lhs + rhs;
}

int sub(int lhs, int rhs) noexcept {
    return lhs - rhs;
}

int mul(int lhs, int rhs) noexcept {
    return lhs * rhs;
}

int divide(int lhs, int rhs, int *error) noexcept {
    if (rhs == 0) {
        *error = 1;
        return 0;
    }

    *error = 0;
    return lhs / rhs;
}

int pow(int base, int exp) noexcept {
    int result = 1;

    for (int i = 0; i < exp; ++i) {
        result *= base;
    }

    return result;
}

int fact(int n) noexcept {
    if (n <= 1) {
        return 1;
    }

    return n * fact(n - 1);
}
