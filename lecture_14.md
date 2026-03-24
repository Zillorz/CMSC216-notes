# Lecture 14 - 3/24/26

### Stack Alignment
- According to the strict x86-64 ABI, must align `rsp` (stack pointer) to 16 byte boundaries when calling functions
- Will often see arbitrary pushes or subtractions to align
    - Functions called with 16-byte alignment
    - `call` pushes 8-byte Return Address on the stack
    - At minimum, must grow stack by 8 bytes to `call` again
- `rsp` changes must be undone prior to return

```asm
main:
    subq    $8, %rsp
    ...
    call    sum_range
    ...
    addq    $8, %rsp
```

### History: rbp
- 32-bit x86 / IA32 assembly used rbp and rsp to describe stack frames
- All function args pushed onto the stack when calling, changes both Stack and Base pointer
- x86-64: optimizies %rbp for general purpose use, points at stack only in special situations

> [!WARNING]
> Some examples (like those on Wikipedia) may include this legacy calling method
> Do not use this in your written assembly, you WILL lose points

### Global variables in assembly
- Global data can be set up in assembly in .data sections with labels and assembler directives like .int and .short
- Modern access to globals
    - Uses `%rip` relative addressing
    - Default in gcc as it plays nice with OS security features like Address Space Randomization
    - May discuss again later during Linking/ELF coverage

```asm
.data
an_int:
    .int 12

some_ints:
    .int 20
    .int 23

.text
    movl an_int(%rip), $eax
    leaq some_ints(%rip), $rdx
```

> [!NOTE]
> Most of today's lecture was on a single example about calling functions and the stack
> Those things will be important
