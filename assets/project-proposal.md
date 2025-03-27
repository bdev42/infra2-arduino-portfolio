# Moncky 1 processor emulator

The Moncky 1 is a simple RISC (Reduced Instruction Set) 16 bit processor. My goal is to be able to run Moncky 1 machine code on the Arduino by  emulating the Moncky 1 processor.

## **Phase 1: Representing the CPU and memory**

The state of the CPU is stored in a **struct**, this contains **all flags** and **registers**.  
**FLAGS: carry, zero, sign, overflow** (Always shown by the LEDs)  
**REGISTERS: 16x  16 bit registers (R0 \-\> R15)**  
**PROGRAM COUNTER:** points to the currently executed instruction in the program memory (NOT DIRECTLY ACCESSIBLE)

The dataram is an array of 16 bit integers that is dynamically allocated, (or a struct with “blocks” of memory that are dynamically allocated).

The program memory is static, and can therefore be stored by an array of 16 bit integers.  
(The program can be defined in **“program.h”** otherwise a default program is defined and executed).

**BUTTON1 interrupt**: display values of all the registers (R0 to R15).

## **Phase 2: Implementing the instruction set**

Implement a function that simulates a single processor/clock cycle using the current state of the processor (executing the current  instruction at the program counter).

### INSTRUCTION SET

| machine language | assembly | meaning | example |
| :---- | :---- | :---- | :---- |
| 0000 xxxx xxxx xxxx | halt | the processor stops | halt |
| 00x1 iiii iiii rrrr | li r, i | reg\[r\] \= i | li r3, 42 |
| 01xx AAAA rrrr ssss | ALU r, s | reg\[r\] \= reg\[r\] ALU reg\[s\] | add r3, r4 |
| 100x xxxx rrrr ssss | ld r, (s) | reg\[r\] \= DATARAM\[reg\[s\]\] | ld r2, (r15) |
| 101x xxxx rrrr ssss | st r, (s) | DATARAM\[reg\[s\]\] \= reg\[r\] | st r1, (r15) |
| 110x xxxx xxxx rrrr | jp r | PC \= reg\[r\] | add r3, r4 |
| 1111 xxxx xccc rrrr | jpc r | if(c) then PC \= reg\[r\] | jpz r1 |

**iiii iiii** 8 bit number immediate  
**rrrr** register nr 0-15  
**ssss** register nr 0-15

#### ALU INSTRUCTIONS

| AAAA | assembly | meaning |
| :---- | :---- | :---- |
| 0000 | NOP | No operation: the result is equal to the second input of the ALU |
| 0001 | OR | bitwise OR |
| 0010 | AND | bitwise AND |
| 0011 | XOR | bitwise XOR |
| 0100 | ADD | addition |
| 0101 | SUB | subtraction |
| 0110 | SHL | shift the first number left by the second number |
| 0111 | SHR | shift the first number right by the second number |
| 1000 | ASHR | SHR but maintain sign bit |
| 1001 | NOT | logic inverse: the result is the 1-complement of the second input |
| 1010 | NEG | negative: the result is the 2-complement of the second input |

#### CONDITIONS (FLAGS)

| ccc | flag | meaning |
| :---- | :---- | :---- |
| 000 | c | if the carry flag is equal to 1 (the calculation resulted in 1 extra bit being discarded) |
| 001 | nc | if the carry flag is equal to 0 (the calculation did not result in an extra bit) |
| 010 | z | if the zero flag is equal to 1 (the result of the calculation was 0\) |
| 011 | nz | if the zero flag is equal to 0 (the result of the calculation was not equal to 0\) |
| 100 | s | if the sign flag is equal to 1 (the result of the calculation was negative) |
| 101 | ns | if the sign flag is equal to 0 (the result of the calculation was not negative) |
| 110 | o | if the overflow flag is equal to 1 (the calculation resulted in an overflow) |
| 111 | no | if the overflow flag is equal to 0 (the calculation did not result in an overflow) |

**BUTTON3 Interrupt:** Execute a single instruction / clock cycle by calling the function implemented above

## **Phase 3: Implementing the processor clock using a Timer**

To make the processor run on its own, we must perform clock cycles automatically, to do this we will use a Timer. The goal is to be able to adjust the clock speed using the **Potentiometer.** And to be able to stop/pause the automatic clock using the **BUTTON2 interrupt.**

## **Phase 4: Map R14 and R15 to the display**

R14 and R15  is connected to 4x 8-segment displays (7seg \+ dot)

Each 8 bits correspond to one display, the bit order is:

| R14 |  | R15 |  |
| :---: | :---: | :---: | :---: |
| DISPLAY1 | DISPLAY2 | DISPLAY3 | DISPLAY4 |
| PCDE GFAB | PCDE GFAB | PCDE GFAB | PCDE GFAB |

(P is the **dot or DP**)  
![][image1]

## **Extra: Map buzzer to a register or make it signal halt or overflow**

[image1]: <data:image/png;base64,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>