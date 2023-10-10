#include "nsstdlib.h"

int main() {
    int i = 0x10000;
    for (;;i++) {
        int a = *(int*)i;

        if (a == 0x12345678) break;
    }

    println("Ended at: ");
    puthexln(i);

    return 0;
}
