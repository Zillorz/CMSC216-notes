# Lecture 12 - 3/10/26

### Disassembling Binaries
- Binaries are hard to read on their own
- Many tools exist to work with the, notably `objdump`
- Can **disassemble** binary: show "readable" version of contents


### leaX: Load Effective Address
- Memory address must often be loaded into registers
- Often done with a leaX, usually leaQ on 64-bit platforms
- Soft of like "address-of" op & in C but a bit more general

> [!NOTE]
> Often the compiler uses leaX to optimize multiplication, makes it faster but less readable

### FLAGS: Condition Codes Registers
- Most CPUs have a special register with "flags" or various conditions
- In x86-64 this register goes by the following name FLAGS, EFLAGS, RFLAGS
- Bits in FLAGS register are automatically set based on results of other operations

| Bit | Abbrev| Name | Description |
| --------------- | --------------- | --------------- | --------------- |
| 0 | aaaa | Item3.1 | Item4.1 |
| 6 | Item2.2 | Item3.2 | Item4.2 |
| 7 | Item2.3 | Item3.3 | Item4.3 |
| | Item2.4 | Item3.4 | Item4.4 |

### Comparisons and Tests
- `cmpX B, A` compute the flags based on A - B
- `testX A, B` compute the flags based on A & B

- Immediates like $2 must not be the first argument B
- B, A are NOT altered with cmp/test instrctions
- EFLAGA register IS changed by cmp/test to incdicate less than, greater than, 0, etc.

### Jump instructions
- `jmp` always jumps
- `j*` jumps on a condition based on flags, usually intuitive

### Logical And / Or in Assembly
Logical boolean operators like `a && b` and `x || y` translate to sequences of compare/test instructions followed by conditional jumps.

### Procedure calls
- `call function`
- `ret`
- Callee save registers must be restored by a function
- Caller save registers are not perserved by a function

- `call` instructions always tranfder control to start of function
- `ret` instruction must transfer to different locations sometimes
- When this is necessary, 
