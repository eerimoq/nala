#include "foo.h"

int square(int x) { return x * x; }

int myfunction(int x, int y) {
    return square(x) + square(y);
}
