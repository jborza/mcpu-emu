#include "DigiKeyboard.h"

// MCPU - A Minimal 8Bit CPU Emulator (CPU design by Tim Boescke, cpldcpu@opencores.org)

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "mcpu.h"

#define MATCH_NOR 0 //opcode: 00AAAAAA
#define MATCH_ADD 1 // opcode: 01AAAAAA
#define MATCH_STA 2 // opcode: 10AAAAAA
#define MATCH_JCC 3 // opcode: 11AAAAAA

// configuration
#define DISASSEMBLE
#define PRINT_STATUS
//#define DEMO_SIMPLE
#define DEMO_PRIME
// configuration end

#ifdef DISASSEMBLE
char* mnemonics[] = {"NOP","ADD","STA","JCC"};

void disassemble(mcpu_state* state) {
  uint8_t opcode = state->memory[state->pc];
  uint8_t immediate = opcode & 0x3f;
  DigiKeyboard.print(mnemonics[opcode >> 6]);
  DigiKeyboard.print(" $");
  DigiKeyboard.print((int)immediate);
  DigiKeyboard.print(" [");
  DigiKeyboard.print((int)state->memory[immediate]);
  DigiKeyboard.print("] ");
}
#endif

void emulate_instruction(mcpu_state* state) {
  uint8_t opcode = state->memory[state->pc++];
  uint8_t immediate = opcode & 0x3f;
  uint16_t result = (state->accu + state->memory[immediate]);
  switch(opcode >> 6){
    case MATCH_NOR:
    //Acc = Acc NOR mem[AAAAAA]
      state->accu = (state->accu | state->memory[opcode]) ^ 0xFF;
      break;
    case MATCH_ADD:
      //Acc = Acc +  mem[AAAAAA], update carry      
      state->carry = result >> 8;
      state->accu = result;
      break;
    case MATCH_STA:
      //mem[AAAAAA] = Acc
      state->memory[immediate] = state->accu;
      break;    
    case MATCH_JCC:
    //PC = AAAAAA if carry = 0, clear carry
      if (opcode == 0xFF) //output pseudo-instruction 0xFF
        DigiKeyboard.println((int)state->accu);
      else if (state->carry == 0) {
        state->pc = immediate;
      }
      state->carry = 0;
      break;
  }
}

mcpu_state* state;

#ifdef DEMO_SIMPLE
void load_binary(){
  state->memory[0] = 0x3e;
  state->memory[1] = 0x45;
  state->memory[2] = 0x7f;
  state->memory[3] = 0xc2;
  state->memory[4] = 0xc4;
  state->memory[5] = 0xfa;
  state->memory[0x3e] = 0xff;
  state->memory[0x3f] = 0x01;
 }
#endif

#ifdef DEMO_PRIME
 void load_binary(){
  int binary[37] = { 0x3e,0x62,0xff,0x3e,0x7e,0x7e,0xa3,0x3e,0x64,0x63,0xcc,0xc9,0x3d,0x63,0x7f,0x7e,0xdc,0x3e,0x63,0x7e,0xa3,0x64,0x7e,0xd9,0xc7,0x3e,0x64,0xff,0x3e,0x64,0x62,0xa4,0xc3,0xc3,0x02,0x00,0x03};
  for(int i = 0; i < MEM_SIZE; i++)
    state->memory[i] = binary[i];
  state->memory[0x3e] = 0xff;
  state->memory[0x3f] = 0x01;
 }
#endif

void setup() {
  state = (mcpu_state *)malloc(sizeof(mcpu_state));
  memset(state, 0, sizeof(mcpu_state));
  load_binary();
  pinMode(0, OUTPUT); //buzzer
  pinMode(1, OUTPUT); //LED 
  DigiKeyboard.delay(3000);
}

int beep = 0;

void loop() {
  #ifdef DISASSEMBLE
  disassemble(state);
  #endif
    
  #ifdef PRINT_STATUS
  DigiKeyboard.print("I,A,C,PC: ");
  DigiKeyboard.print((int)state->memory[state->pc]);
  #endif
  
  emulate_instruction(state);
  
  #ifdef PRINT_STATUS
  DigiKeyboard.print(",");
  DigiKeyboard.print((int)state->accu);
  DigiKeyboard.print(", ");
  DigiKeyboard.print((int)state->carry);
  DigiKeyboard.print(", ");
  DigiKeyboard.println((int)state->pc);
  #endif
  
  digitalWrite(1, beep ? HIGH : LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(0, beep ? HIGH : LOW);   // turn the LED on (HIGH is the voltage level)
  beep = !beep;
}
