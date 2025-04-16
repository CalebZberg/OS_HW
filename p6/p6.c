#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]){

	if (argc != 2) {
		printf("Invalid input, must be one hex address\n");
		return 1;
	}

	unsigned int address = (unsigned int)strtoul(argv[1], NULL, 16);

	unsigned int offset = address & 0xFFF;
	unsigned int pageIndex = address >> 12;

	printf("Logical Addr:0x%08X - Page Index:0x%08X - Offset:0x%08X\n", address, pageIndex, offset);

	return 0;
}
