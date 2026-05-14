# Lecture 12 - 3/10/26

### Disassembling Binaries
- Binaries are hard to read on their own
- Many tools exist to work with the, notably `objdump`
- Can **disassemble** binary: show "readable" version of contents


### leaX: Load Effective Address
- Memory address must often be loaded into registers
- Often done with a leaX, usually leaq on 64-bit platforms
- Soft of like "address-of" op & in C but a bit more general

> [!NOTE]
> Often the compiler uses leaX to optimize multiplication, makes it faster but less readable

### FLAGS: Condition Codes Registers
- Most CPUs have a special register with "flags" or various conditions
- In x86-64 this register goes by the following name FLAGS, EFLAGS, RFLAGS
- Bits in FLAGS register are automatically set based on results of other operations

| Bit | Abbrev| Name | Description |
| --------------- | --------------- | --------------- | --------------- |
| 0  | CF | Carry Flag | |
| 6  | ZF | Zero Flag| |
| 7  | SF | Sign Flag | |
| 8  | TF | Trap Flag | |
| 9  | IF | Interrupt Flag | |
| 11 | OF | Overflow Flag | |

### Comparisons and Tests
- `cmpX B, A` compute the flags based on A - B
- `testX B, A` compute the flags based on A & B

- Immediate values like $2 must not be the first argument B
- B, A are NOT altered by the cmp/test instructions
- EFLAGS register IS changed by cmp/test to indicate less than, greater than, 0, etc.

### Jump instructions
- `jmp` always jumps
- `j*` jumps on a condition based on flags, usually intuitive

### Logical And / Or in Assembly
Logical boolean operators like `a && b` and `x || y` translate to sequences of compare/test instructions followed by conditional jumps.

### Procedure calls
- `call function`
- `ret`
- Callee save registers must be restored by a function
- Caller save registers are not preserved by a function

- `call` instructions always transfer control to start of function
- `ret` instruction must transfer to different locations sometimes
