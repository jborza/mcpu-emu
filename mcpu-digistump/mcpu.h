#define MEM_BITS 6
#define MEM_SIZE 1 << MEM_BITS

typedef struct mcpu_state {
  // CPU is accumulator based, accumulator has a width of 8 bits + carry flag
  uint8_t accu : 8;

  uint8_t carry : 1;
  // PC has a width o 6 bits, allows addressing 64 bytes of memory (shared between code and data)
  uint8_t pc : 6;

  uint8_t memory[MEM_SIZE];
} mcpu_state;
