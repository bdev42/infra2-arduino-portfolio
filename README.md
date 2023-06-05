# Embedded System Programming with Arduino - Assessment Portfolio

## **Final Project:** Moncky 1 emulator (MonckyUno)
[Project Proposal](https://docs.google.com/document/d/14MxjyNdxrFIr5g0-lHUJzJUsimFi6WuBhsvv9xIky0E/edit?usp=share_link)

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
- [ ] Document project

### **How to use**
#### **Setup**
Once you have cloned [this](https://gitlab.com/kdg-ti/infrastructure2/2022-2023/boldizsar-olajos) git repository,
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
the project. For details about the instruction set and other
technical details please refer to the project proposal above.

## Weekly Projects
### [Lunar Lander](LunarLander/)

### [My Synthesizer](Synthesizer/)

### [Stopwatch](Stopwatch/)
