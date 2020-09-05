#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MATCH_NOR 0
#define MATCH_ADD 1
#define MATCH_STA 2
#define MATCH_JCC 3

#define MEM_BITS 6
#define MEM_SIZE 1 << MEM_BITS

void read_program(char *name, uint8_t *buffer)
{
#define BIN_SIZE 64
	FILE *file = fopen(name, "rb");
	if (!file)
	{
		printf("Couldn't load binary %s!", name);
		exit(1);
	}
	//static uint8_t buffer[BIN_SIZE];
	int read = fread(buffer, sizeof(uint8_t), BIN_SIZE, file);
	fclose(file);
}

void disassemble(uint8_t address, uint8_t opcode, char *str)
{
	uint8_t immediate = opcode & 0x3f;
	str += sprintf(str, "%2X:\t%2X\t", address, opcode);
	switch (opcode >> 6)
	{
	case MATCH_NOR:
		sprintf(str, "NOR $%2X",  immediate);
		break;
	case MATCH_ADD:
		sprintf(str, "ADD $%2X",  immediate);
		break;
	case MATCH_STA:
		sprintf(str, "STA $%2X", immediate);
		break;
	case MATCH_JCC:
		sprintf(str, "JCC $%2X",  immediate);
		break;
	}
}

int main(int argc, char *argv[])
{
	uint8_t memory[MEM_SIZE];
	read_program(argv[1], memory);
	char dasm_buffer[128];
	for (int i = 0; i < MEM_SIZE; i++)
	{
		disassemble(i, memory[i], dasm_buffer);
		printf("%s\n", dasm_buffer);
	}
}