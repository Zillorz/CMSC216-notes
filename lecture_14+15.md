# Lecture 14 - 3/24/26 + Lecture 15 - 3/26/26

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
- x86-64: optimizes %rbp for general purpose use, points at stack only in special situations

> [!WARNING]
> Some examples (like those on Wikipedia) may include this legacy calling method
> Do not use this in your written assembly, you WILL lose points

### Global Variables in Assembly
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

### Globals in Disassembled Code
- Global variables are in the "Global" area of memory
- Position Independent Executables (PIE) have the Text Memory Region at an arbitrary position BUT place the Globals Region at a fixed offset 
- Instruction Point (RIP) points to a instruction in Text, references the memory location in Globals by adding a offset

### Aggregate Data in Assembly (Arrays + Structs)
- Arrays
    - Usually: base + index * size
    - `movl (%rdi, %rcx, 4)`
    - Array starting address often held in reg
    - Array index held in reg
    - Compiler inserts appropriate size (usually only 1, 2, 4, 8)
- Structs
    - Usually base+offset
    - `f->s = 4` = `movw $4, 4(%rax)`

### Packed Structures as Procedure Arguments
- Passing pointers to structs is 'normal': registers contain addresses to main memory
- Passing actual structs may result in *packed structs* where several fields are in a single register
- Assembly must *unpack* these through **shifts and masking**

### Large Packed Structs
- Structs that don't fit into single registers may be packed across several argument registers
- At a certain size, the compiler stores Very Large packed structs in the stack and passes pointers to functions

### Security Risks in C
- Buffer Overflow Attacks
    - No default bounds checking in C: Performance favored over safety
    - Allows classic security flaws
    - For data larger than buf, begin overwriting other parts of the stack
        - Clobber return addresses
        - Insert executable code and run it
- Counter-measures
    - **Stack protection** is default in gcc in the modern era
    - Inserts "canary" values on the stack near return address
    - Prior to function return, checks that canaries are unchanged
    - **Stack / Text Section** start randomized by kernel, return address and function addresses difficult to predict AOT
    - Kernel may also vary virtual memory address as well
    - Disabling protections is risky


### Floating Point Operations
- Original Intel 8086 Processor didn't do floating point ops
- Had to buy a co-processor (Intel 8087) to enable FP ops
- Most modern CPUs support FP ops but they feel separate from the integer ops: FPU verses ALU
- Can be used as "scalars" - single values, but
- `xmmI` is 128 bits long
    - hold 2 doubles or 4 floats
- `ymmI` and `zmmI` extend further
- Operates on single values or "vectors" of packed values
- 3-operands common in more "modern" assembly languages

> [!NOTE]
> Most of today's lecture was on a single example about calling functions and the stack
> Those things will be important
> These notes were also continued on 3/26/26
