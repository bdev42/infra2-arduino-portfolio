#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include <buttonlib.h>

#define DEBUG
#ifdef DEBUG
#include <usart.h>
#endif

#define CLEAR_GARBAGE_ON_ALLOC

#include "program.h"

const uint16_t program[] = PROGRAM;

typedef struct
{
  uint16_t reg[16];
  uint16_t PC;
  uint8_t flags;
} CPU;

CPU cpuState = {};

#define FL_CARRY    0
#define FL_ZERO     1
#define FL_SIGN     2
#define FL_OVERFLOW 3
#define FL_MEMORYF  6
#define FL_HALT     7

#define FL_CARRY_MSK    _BV(FL_CARRY)
#define FL_ZERO_MSK     _BV(FL_ZERO)
#define FL_SIGN_MSK     _BV(FL_SIGN)
#define FL_OVERFLOW_MSK _BV(FL_OVERFLOW)
#define FL_MEMORYF_MSK  _BV(FL_MEMORYF)
#define FL_HALT_MSK     _BV(FL_HALT)

typedef struct
{
  uint16_t values[256];
} MEMBLOCK;

typedef struct
{
  uint8_t num_blocks;
  MEMBLOCK *blocks;
} MEMORY;

MEMORY dataram = {};

#ifdef DEBUG
void printCPUState(CPU *cpuState) {
  printString("---=== MonckyUno State ===---\nPC: ");
  printWord(cpuState->PC);
  printString("\n");
  for(uint8_t i = 0; i < 16; i++) {
    printf("R%d: 0x%X ", i, cpuState->reg[i]);
  }
  printString("\n-----------------------------\n");
}
void printMemoryBlock(MEMBLOCK *block) {
  printString("\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  MEMORY BLOCK - START  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n    ");
  for(uint8_t lsi = 0; lsi < 16; lsi++) {
    printf("   %x ", lsi);
  }
  printString("\n");
  for(uint8_t msi = 0; msi < 16; msi++) {
    printf("\n0x%x  ", msi);
    for(uint8_t lsi = 0; lsi < 16; lsi++) {
      uint8_t i = (msi << 4) | lsi;
      printHexByte((block->values[i] & 0xff00) >> 8);
      printHexByte(block->values[i] & 0x00ff);
      printString(" ");
    }
  }
  printString("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   MEMORY BLOCK - END   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
}
void printState(CPU *cpuState) {
  printCPUState(cpuState);
  printString("MEMORY STATISTICS: \n");
  printf("Blocks Allocated: %u\n", dataram.num_blocks);
  if(dataram.num_blocks >= 1) {
    printString("First Block: ");
    printMemoryBlock(dataram.blocks);
  }
  printString("=============================\n");
}
#endif

int writeMemory(uint16_t address, uint16_t value) {
  uint8_t block_index = (address & 0xff00) >> 8;
  if(block_index >= dataram.num_blocks) {
    dataram.num_blocks = block_index + 1;
    #ifdef DEBUG
    printf("\t\tAllocating memory: (blocks: %d;TO_WRITE: adress: 0x%x, block: 0x%x, value: 0x%X)\n", dataram.num_blocks, address, block_index, value);
    #endif
    dataram.blocks = realloc(dataram.blocks, sizeof(MEMBLOCK)*(dataram.num_blocks));
    if(dataram.blocks==NULL) {
      #ifdef DEBUG
      printf("Memory Allocation FAILED: (blocks: %d;TO_WRITE: adress: 0x%x, block: 0x%x, value: 0x%X)\n", dataram.num_blocks, address, block_index, value);
      #endif
      return 0;
    }
    #ifdef CLEAR_GARBAGE_ON_ALLOC
    dataram.blocks[block_index].values[0] = 0;
    for(uint8_t i = 0; i < 255; i++) {
      dataram.blocks[block_index].values[i+1] = 0;
    }
    #endif
  }
  dataram.blocks[block_index].values[address & 0x00ff] = value;
  return 1;
}

uint16_t readMemory(uint16_t address) {
  uint8_t block_index = (address & 0xff00) >> 8;
  if(block_index >= dataram.num_blocks) {
    return 0; //uninitilaized memory blocks are assumed to have 0 as value
  }
  return dataram.blocks[block_index].values[address & 0x00ff];
}

void ALU(CPU *cpuState, uint16_t inst) {
  // 01xx AAAA rrrr ssss
  const uint8_t operation = (inst & 0x0f00) >> 8;
  uint32_t result = cpuState->reg[(inst & 0x00f0) >> 4]; //also operand A
  const uint16_t B = cpuState->reg[inst & 0x000f];
  #ifdef DEBUG
  printf("\tALU CALL>> op: %u A:", operation);
  printWord(cpuState->reg[(inst & 0x00f0) >> 4]);
  printString(" B: ");
  printWord(cpuState->reg[inst & 0x000f]);
  #endif
  if(operation == 0) // NOP
  {
    result = B;
  }else if(operation == 1) // OR
  {
    result |= B;
  }else if(operation == 2) // AND
  {
    result &= B;
  }else if(operation == 3) // XOR
  {
    result = result ^ B;
  }else if(operation == 4) // ADD
  {
    result += B;
  }else if(operation == 5) // SUB
  {
    result -= B;
  }else if(operation == 6) // SHIFT LEFT
  {
    result = (result << B) & 0xffff;
  }else if(operation == 7) // SHIFT RIGHT
  {
    result = result >> B;
  }else if(operation == 8) // ARITHMETIC SHIFT RIGHT
  {
    //Right shift for signed values are implementation dependent in C, so we do it on our own:
    uint8_t sign = result & 0x8000;
    for(uint8_t i = 0; i < B; i++) {
      result = sign | (result >> 1);
    }
  }else if(operation == 9) // NOT
  {
    result = ~B;
  }else if(operation == 10) // NEG
  {
    // 2's complement of the second input
    // must ignore overflow
    result = (1 + (~B)) & 0xffff;
  }else {
    #ifdef DEBUG
    printf("/!\\ Unknown/invalid ALU operation '%d' at PC: %d\n", operation, cpuState->PC);
    #endif
  }
  //--= UPDATE CPU FLAGS =--//
  //carry flag (unsigned overflow)
  if(result & 0x10000) {
    cpuState->flags |= FL_CARRY_MSK;
  }else {
    cpuState->flags &= ~FL_CARRY_MSK;
  }
  //zero flag
  if((result & 0xffff) == 0) {
    cpuState->flags |= FL_ZERO_MSK;
  }else {
    cpuState->flags &= ~FL_ZERO_MSK;
  }
  //sign flag
  if(result & 0x8000) {
    cpuState->flags |= FL_SIGN_MSK;
  }else {
    cpuState->flags &= ~FL_SIGN_MSK;
  }
  // overflow flag
  // When two signed 2's complement numbers are added, overflow is detected if:
  // both operands are positive and the result is negative, 
  // OR both operands are negative and the result is positive.
  uint8_t operandANegative = (cpuState->reg[(inst & 0x00f0) >> 4] & 0x8000) != 0;
  if(((!operandANegative) && ((B & 0x8000)==0) && ((result & 0x8000)!=0)) || ((operandANegative) && ((B & 0x8000)!=0) && ((result & 0x8000)==0))) {
    cpuState->flags |= FL_OVERFLOW_MSK;
  }else {
    cpuState->flags &= ~FL_OVERFLOW_MSK;
  }

  #ifdef DEBUG
  printString(" result: ");
  printWord(result);
  printString("\n");
  #endif
  // reg[r] = reg[r] ALU reg[s]
  cpuState->reg[(inst & 0x00f0) >> 4] = (uint16_t) result;
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
    #ifdef DEBUG
      printf("\tli r%u, 0x%X\n", inst & 0x000f, (inst >> 4) & 0x00ff);
    #endif
    cpuState->reg[inst & 0x000f] = (inst >> 4) & 0x00ff;
    cpuState->PC++;
  }else if( (inst & 0x8000) == 0 && (inst & 0x4000)) // ALU 
  {
    // 01xx AAAA rrrr ssss
    #ifdef DEBUG
      printf("\tALU r%u, r%u", (inst & 0x00f0) >> 4, inst & 0x000f);
    #endif
    ALU(cpuState, inst);
    cpuState->PC++;
  }else if( (inst & 0x8000) && (inst & 0x6000) == 0 ) // LOAD
  {
    // 100x xxxx rrrr ssss
    #ifdef DEBUG
      printf("\tld r%u, (r%u)\n", (inst & 0x00f0) >> 4, inst & 0x000f);
    #endif
    cpuState->reg[(inst & 0x00f0) >> 4] = readMemory(cpuState->reg[inst & 0x000f]);
    cpuState->PC++;
  }else if((inst & 0xA000) == 0xA000 && (inst & 0x4000) == 0) // STORE
  {
    // 101x xxxx rrrr ssss
    #ifdef DEBUG
      printf("\tst r%u, (r%u)\n", (inst & 0x00f0) >> 4, inst & 0x000f);
    #endif
    if(!writeMemory(cpuState->reg[inst & 0x000f], cpuState->reg[(inst & 0x00f0) >> 4])) {
      #ifdef DEBUG
      printf("/!\\ Moncky 1 HALTING DUE TO MEMORYFAULT... (PC: %d)\n", cpuState->PC);
      #endif
      cpuState->flags |= FL_HALT_MSK | FL_MEMORYF_MSK;
    }
    cpuState->PC++;
  }else if( (inst & 0xC000) == 0xC000 && (inst & 0x2000) == 0) // JUMP
  {
    // 110x xxxx xxxx rrrr
    #ifdef DEBUG
      printf("\tjp r%u\n", inst & 0x000f);
    #endif
    cpuState->PC = cpuState->reg[inst & 0x000f];
  }else if( (inst & 0xf000) == 0xf000 ) //CONDITIONAL JUMP
  {
    // 1111 xxxx xccN rrrr (N: negate condition if 1)
    #ifdef DEBUG
      printf("\tjpC r%u  ; condition = %u\n", inst & 0x000f, (inst & 0x0060) >> 5);
    #endif
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

ISR(PCINT1_vect) {
  if(bit_is_clear(BUTTONS_PIN, BUTTON1)) {
    printState(&cpuState);
  } else if(bit_is_clear(BUTTONS_PIN, BUTTON3)) {
    performInstructionCycle(&cpuState);
  }
}

int main() {

  enableButtonInterrupts();

  #ifdef DEBUG
  initUSART();
  printState(&cpuState);
  printString("Moncky 1 STARTED!\n");
  #endif

  while((cpuState.flags & FL_HALT_MSK) == 0) {
    performInstructionCycle(&cpuState);
  }
  printState(&cpuState);
}
