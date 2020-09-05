// MCPU - A Minimal 8Bit CPU Emulator (CPU design by Tim Boescke, cpldcpu@opencores.org)

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
    // PC has a width o 6 bits, allows addressing 64 bytes of memory (shared between code and data)
    uint8_t pc : 6;

    uint8_t memory[MEM_SIZE];
} mcpu_state;

// instruction set: NOR, ADD, STA, JCC

// opcode: 00AAAAAA
void nor(mcpu_state *state, uint8_t opcode);
// opcode: 01AAAAAA
void add(mcpu_state *state, uint8_t opcode);
// opcode: 10AAAAAA
void sta(mcpu_state *state, uint8_t opcode);
// opcode: 11AAAAAA
void jcc(mcpu_state *state, uint8_t opcode);


int emulate_instruction(mcpu_state *state){
    uint8_t* opcode = &state->memory[state->pc++];    
    switch(*opcode){
        //all opcodes
        INS_MATCH(MATCH_NOR, nor);
        INS_MATCH(MATCH_ADD, add);
        INS_MATCH(MATCH_STA, sta);
        INS_MATCH(MATCH_JCC, jcc);
    }
}

int main(int argc, char* argv[]){
    //TODO zero-initialize the state
    mcpu_state *state = mallocz(sizeof(mcpu_state));
    //start execution at PC 0
    while(1){
        emulate_instruction(state);
    }
}