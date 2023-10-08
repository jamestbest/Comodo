asm ("ldr r13, =$0x10000");

int main(int argc,  char **argv) {
	int arr[] = {1,2,3,4,5};

	int tot = 0;
	int len = sizeof(arr) / sizeof(int);
	for (int i = 0; i < len; i++) {
		tot += arr[i] << 1;
		tot -= i % 2 == 0 ? 1 : 0;
	}

	for (int i = 0; i < len; i++) {
		arr[i] = tot * arr[i] + 1 << i;
	}

	return arr[0];
}
