#include <stdio.h>

extern void compile(char *);

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Syntax error");
		return 0;
	}
	compile(argv[1]);
	printf("\n\nThanh cong\n\n");
	return 0;
}
