# Embedded System Programming with Arduino - Assessment Portfolio

## **Final Project:** Moncky 1 emulator (MonckyUno)
[Project Proposal](/assets/project-proposal.md) ([pdf](/assets/project-proposal.pdf)) | Inspired by: [The Moncky Project by Kris Demuynck](https://gitlab.com/big-bat/moncky)

**TASK OVERVIEW**
- [x] Proposal approved
- [x] Implement project
    - [x] Representation in memory
        - [x] CPU
        - [x] DATARAM
            - [x] Dynamic allocation
        - [x] BUTTON1: display all registers
    - [x] Implement instructions
        - [x] Halt
        - [x] Load immediate
        - [x] ALU
        - [x] Load & Store
        - [x] Jump
        - [x] Conditional jump
    - [x] Implement clock using Timer
        - [x] BUTTON2: start/stop timer
        - [x] Potentiometer: adjust clock speed
    - [x] Map display to R14 and R15
- [x] BONUS: modify MonckyCode assembler to add MonckyUno export tab
- [x] Document project

### **How to use**
#### **Setup**
Once you have cloned [this](https://github.com/bdev42/infra2-arduino-portfolio.git) git repository,
open the `MonckyUno` directory in _VS Code_ with **PlatformIO**.

#### **Loading a program**
The PROGRAM must be defined in the `program.h` header file,
which is located in the **include** subfolder of the project.
Use the **MonckyUno** tab of the **_MonckyCode assembler_** to
generate the machine code in the correct format. Make sure to
comment out any other define statements for PROGRAM in this header file.
Then simply press the "Upload" button of PlatformIO to upload to your Arduino.

#### **The modified MonckyCode assembler**
The assembler has an additional **MonckyUno** tab to export
your program in a format that can be directly pasted inside
the `program.h` header file. To use the assembler simply
open the `MonckyCode.html` file with your preferred browser
which is located inside the `moncky_assembler` subfolder of
the project.

### **Additional information**
For details about the instruction set and other technical information please refer to the project proposal above,
the rest of the information can be found below:
#### **MonckyUno Settings**
The following emulator settings may be defined in `program.h`
to customize some of the behaviour of the emulator.

##### **DEBUG**
If defined, debugging is enabled, and information such us initial
and final state of the whole processor (and memory) will be printed
as well as the current instructions that are being executed.
The downside of it is that it can slow down the CPU and that
a lot of memory will be used up (so only about 1-2 blocks of
memory can be allocated).

**Usage:** `#define DEBUG`
##### **DEBUG_DONT_PRINT_INSTRUCTIONS**
If defined, disables the printing of the instructions
being executed, which is the main cause of slow performance.
So only errors, warnings, and the initial and final state will be printed.

**Usage:** `#define DEBUG_DONT_PRINT_INSTRUCTIONS`
##### **CLEAR_GARBAGE_ON_ALLOC**
If defined, all values in the new block will be cleared (set to 0),
when a new block is allocated.

**Usage:** `#define CLEAR_GARBAGE_ON_ALLOC`
##### **BUTTON1_DURATION_PER_REGISTER**
Defines the time (in ms) a single register's number and 
value will be shown for when button1 is pressed.

**Usage:** `#define BUTTON1_DURATION_PER_REGISTER <duration-in-ms>`
##### **BUTTON1_SKIP_REGS_WITH_ZERO**
If defined, the registers that posess a value of 0 will
not be shown (skipped), this makes displaying all the
registers take less time, without losing any information.

**Usage:** `#define BUTTON1_SKIP_REGS_WITH_ZERO`
##### **ERROR_BUZZER_ENABLE**
If defined, the buzzer will beep if any errors,
such as a memoryfault occured. (aka when any of
the flags inside the HALTED_WITH_ERROR_MASK are set)

**Usage:** `#define ERROR_BUZZER_ENABLE`

## Weekly Projects
### [Lunar Lander](LunarLander/)

### [My Synthesizer](Synthesizer/)

### [Stopwatch](Stopwatch/)
