// MCPU - A Minimal 8Bit CPU Emulator (CPU design by Tim Boescke, cpldcpu@opencores.org)

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MEM_BITS 6
#define MEM_SIZE 1 << MEM_BITS

#define INS_MATCH(MATCH,HANDLER) if ((*opcode >> 6) == MATCH) { HANDLER(state, *opcode);	}

#define MATCH_NOR 0
#define MATCH_ADD 1
#define MATCH_STA 2
#define MATCH_JCC 3

void* mallocz(size_t size)
{
	void* ptr;
	ptr = malloc(size);
	if (!ptr)
		return NULL;
	memset(ptr, 0, size);
	return ptr;
}

typedef struct mcpu_state {
	// CPU is accumulator based, accumulator has a width of 8 bits + carry flag
	uint8_t accu : 8;

	uint8_t carry : 1;
	// PC has a width o 6 bits, allows addressing 64 bytes of memory (shared between code and data)
	uint8_t pc : 6;

	uint8_t memory[MEM_SIZE];
} mcpu_state;

// instruction set: NOR, ADD, STA, JCC

// opcode: 00AAAAAA
void nor(mcpu_state* state, uint8_t opcode) {
	//Acc = Acc NOR mem[AAAAAA]
	state->accu = (state->accu | state->memory[opcode]) ^ 0xFF;
}
// opcode: 01AAAAAA
void add(mcpu_state* state, uint8_t opcode) {
	uint8_t immediate = opcode & 0x3f;
	//update carry first
	uint16_t result = (state->accu + state->memory[immediate]);
	state->carry = result >> 8;
	//Acc = Acc +  mem[AAAAAA], update carry
	state->accu = result;
}
// opcode: 10AAAAAA
void sta(mcpu_state* state, uint8_t opcode) {
	uint8_t immediate = opcode & 0x3f;
	//mem[AAAAAA] = Acc
	state->memory[immediate] = state->accu;

}
// opcode: 11AAAAAA
void jcc(mcpu_state* state, uint8_t opcode) {
	//PC = AAAAAA if carry = 0, clear carry
	uint8_t immediate = opcode & 0x3f;
	if (state->carry == 0) {
		state->pc = immediate;
	}
	state->carry = 0;
}

void disassemble(mcpu_state *state, char* str) {
	uint8_t opcode = state->memory[state->pc];
	uint8_t immediate = opcode & 0x3f;
	switch (opcode >> 6) {
	case MATCH_NOR:
		sprintf(str, "NOR $%2X [%2X]", immediate, state->memory[immediate]);
		break;
	case MATCH_ADD:
		sprintf(str, "ADD $%2X [%2X]", immediate, state->memory[immediate]);
		break;
	case MATCH_STA:
		sprintf(str, "STA $%2X", immediate);
		break;
	case MATCH_JCC:
		sprintf(str, "JCC $%2X", immediate);
		break;
	}
}

void emulate_instruction(mcpu_state* state) {
	uint8_t* opcode = &state->memory[state->pc++];
	//all opcodes
	INS_MATCH(MATCH_NOR, nor);
	INS_MATCH(MATCH_ADD, add);
	INS_MATCH(MATCH_STA, sta);
	INS_MATCH(MATCH_JCC, jcc);
}

void read_program(char* name, uint8_t *buffer) {
#define BIN_SIZE 64
	FILE* file = fopen(name, "rb");
	if (!file) {
		int err = errno;
		printf("Couldn't load binary %s!", name);
		exit(1);
	}
	//static uint8_t buffer[BIN_SIZE];
	int read = fread(buffer, sizeof(uint8_t), BIN_SIZE, file);
	fclose(file);
}

int main(int argc, char* argv[]) {
	mcpu_state* state = mallocz(sizeof(mcpu_state));
	//read the program
	read_program("programs/prog_test.bin", state->memory);
	//start execution at PC 0
	while (1) {
		char dasm_buffer[128];
		disassemble(state, dasm_buffer);
		emulate_instruction(state);
		printf("%-20s  A:%02X C:%01X PC:%02X\n", dasm_buffer, state->accu, state->carry, state->pc);
	}
}