asm ("ldr r13, =$0x10000");


int main(int argc, char **argv) {
    int a = 12;
    int b = 144;

    int c = b + a;

    return c;
}