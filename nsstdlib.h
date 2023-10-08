char getchar();

void putchar (char a);
void putcharln (char a);
void putint (int a);
void putintln (int a);
void print (char *str);
void println (char *str);

void reset(int err);

void getstring(char terminator, int maxSize, char* buff);
int streq(char *stra, char *strb);
void strcat(char *stra, char *strb, char *strc);
int str2int(char *str);
int len(char *str);

int min(int a, int b);
int max(int a, int b);
int pow(int base, int expo);
int ror(int a, int rep);

void heapCreate();
void* malloc(unsigned int bytes);
void* malloc_debug(unsigned int bytes);
int free(void* ptr);