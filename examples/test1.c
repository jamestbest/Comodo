struct test {
    int t1;
    int t2;
} test;

int main() {
    int a = 12; int b = 14;

    for (int i = 0; i < 10; i++) {
        a = a + b;
        b = b << 1;
    }

    struct test astruct = {1,3};

    int c = astruct.t1 + astruct.t2;

    char* stra = "Hello\0";
    char* strb = "World\0";

    return 0;
}
