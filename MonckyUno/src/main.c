#include <avr/io.h>

#define DEBUG
#ifdef DEBUG
#include <usart.h>
#endif

#include "program.h"

const uint16_t program[] = PROGRAM;

typedef struct
{
  uint16_t reg[16];
  uint16_t PC;
  uint8_t flags;
} CPU;

#define FL_CARRY    0
#define FL_ZERO     1
#define FL_SIGN     2
#define FL_OVERFLOW 3
#define FL_HALT     7

#define FL_CARRY_MSK    _BV(FL_CARRY)
#define FL_ZERO_MSK     _BV(FL_ZERO)
#define FL_SIGN_MSK     _BV(FL_SIGN)
#define FL_OVERFLOW_MSK _BV(FL_OVERFLOW)
#define FL_HALT_MSK     _BV(FL_HALT)

void writeMemory(uint16_t adress, uint16_t value) {

}

uint16_t readMemory(uint16_t adress) {

}

void ALU(CPU *cpuState, uint16_t inst) {
  // 01xx AAAA rrrr ssss
  uint8_t operation = (inst & 0x0f00) >> 8;
  uint32_t result = 0;
  if(operation == 0) // NOP
  {

  }else if(operation == 1) // OR
  {

  }else if(operation == 2) // AND
  {

  }else if(operation == 3) // XOR
  {

  }else if(operation == 4) // ADD
  {

  }else if(operation == 5) // SUB
  {

  }else if(operation == 6) // SHIFT LEFT
  {

  }else if(operation == 7) // SHIFT RIGHT
  {

  }else if(operation == 8) // ARITHMETIC SHIFT RIGHT
  {

  }else if(operation == 9) // NOT
  {

  }else if(operation == 10) // NEG
  {

  }else {
    #ifdef DEBUG
    printf("/!\\ Unknown/invalid ALU operation '%d' at PC: %d\n", operation, cpuState->PC);
    #endif
  }
}

void performInstructionCycle(CPU *cpuState) {
  if(cpuState->flags & FL_HALT_MSK) return; // Processor halted, do not perform instruction cycle

  uint16_t inst = program[cpuState->PC];
  if( (inst & 0xf000) == 0 ) // HALT
  {
    // 0000 xxxx xxxx xxxx
    cpuState->flags |= FL_HALT_MSK;
    #ifdef DEBUG
    printf("Moncky 1 HALTED (PC: %d)\n", cpuState->PC);
    #endif
    return;
  }else if( (inst & 0xC000) == 0 && (inst & 0x1000) ) // LOAD IMMEDIATE
  {
    // 00x1 iiii iiii rrrr
    cpuState->reg[inst & 0x000f] = (inst >> 4) & 0x00ff;
    cpuState->PC++;
  }else if( (inst & 0x8000) == 0 && (inst & 0x4000)) // ALU 
  {
    // 01xx AAAA rrrr ssss
    ALU(&cpuState, inst);
    cpuState->PC++;
  }else if( (inst & 0x8000) && (inst & 0x6000) == 0 ) // LOAD
  {
    // 100x xxxx rrrr ssss
    cpuState->reg[inst & 0x00f0] = readMemory(cpuState->reg[inst & 0x000f]);
    cpuState->PC++;
  }else if((inst & 0xA000) == 0xA000 && (inst & 0x4000) == 0) // STORE
  {
    // 101x xxxx rrrr ssss
    writeMemory(cpuState->reg[inst & 0x000f], cpuState->reg[inst & 0x00f0]);
    cpuState->PC++;
  }else if( (inst & 0xC000) == 0xC000 && (inst & 0x2000) == 0) // JUMP
  {
    // 110x xxxx xxxx rrrr
    cpuState->PC = cpuState->reg[inst & 0x000f];
  }else if( (inst & 0xf000) == 0xf000 ) //CONDITIONAL JUMP
  {
    // 1111 xxxx xccN rrrr (N: negate condition if 1)
    uint8_t condition = (cpuState->flags & _BV((inst & 0x0060) >> 5)) != 0;
    if(condition != ((inst & 0x0010) >> 4)) {
      cpuState->PC = cpuState->reg[inst & 0x000f];
    }else {
      cpuState->PC++;
    }
  }else
  {
    #ifdef DEBUG
    printf("/!\\ Unknown/invalid instruction '0x%X' at PC: %d\n", inst, cpuState->PC);
    #endif
    cpuState->PC++; // Skip unknown instruction
  }
}

int main() {
  CPU cpuState = {};

  #ifdef DEBUG
  initUSART();
  printString("Moncky 1 started!\n");
  #endif

  while((cpuState.flags & FL_HALT_MSK) == 0) {
    performInstructionCycle(&cpuState);
  }

}