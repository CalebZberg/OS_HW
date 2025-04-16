// Caleb Zierenberg
// CS-3060-001 Spring 2025
// M1 Program 0 - "Programming in C on a Linux system"


#include <stdio.h>

int main(int argc, char * argv[]) {
	printf("Caleb Zieneberg - Programming in C on a Linux system\n");
	int i = 0;
	for (i = 0; i < argc; i++) {
		printf("Argument #%d: %s\n", i, argv[i]);
	}
}
