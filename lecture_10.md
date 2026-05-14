# Lecture 10 - 3/3/26 

> [!NOTE]
> You can use gdb on ASM code too!

To run an assembly file, let's use an assembler, which is included with gcc/clang!
`gcc hi.asm` compiles the assembly!

To compile a c file to assembly, run `gcc -S hi.c`, 
use the -Og flag to make it more readable.

> [!WARNING]
> We use AT&T syntax, intel syntax will result in a ZERO on exam/lab/projects

### Basic Instruction Classes
- Remember: Goal is to understand assembly as a target for higher level languages
- Means we won't hit all 4,834 pages of the x86-64 manual
- Brown University's x64 cheat sheet is a good overview
- x86 assembly from Yale is also good, but limited to 32bit

### Data Movement: mov instruction

#### Overview
- Moves data
    - Reg to Reg
    - Mem to Reg
    - Reg to Mem
    - Imm to ...
- Reg: register
- Mem: main memory
- Imm: "immediate value" (constant), must start with $

#### Variations
- movq, movl, movw, movb
- b -> byte, w -> word, l -> long (2) word, q -> quad word
- b = 8bit, w = 16bit, l = 32bit, q = 64bit


### Operands and Addressing Modes

| Style | Address Mode | C-like | Notes |
| --------------- | --------------- | --------------- | --------------- |
| $21, $0xD2 | Immediate | 21 | value of constant |
| %rax | register | rax | to/from register contents |
| (%rax) | indirect | `*rax` | deref |
| 8(%rax) | displaced | `*(rax + 2)`, `rax->field` | base plus const  |
| (%rax, %rbx) | indexed | `*(rax + rbx)`, `arr[rbx]` | no sizeof |
| (%rax, %rbx, 4) | scaled index | `*(rax + rbx * 4)` | sizeof |
| 1024 | absolute | `(void*)(1024)` | Absolute memory address|

> [!WARNING]
> ALWAYS USE **MOVQ** to copy memory addresses, as movl will not correctly copy, and will cause segfault
