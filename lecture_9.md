# Lecture 9 - 2/26/26

### Showing bits
- `printf()`
    - `%d` is Decimal
    - `%x` is Hexdeciaml
    - `%o` is Octal
    - `%c` is Character
- No binary specifier
- Can construct one with bitwise operator
- Usually involves shifting and &

```c standalone
void showbits(int b) {
    for(int i = 31; i >= 0; i--) {
        if ((1 << i) & b) {
            printf("1");
        } else {
            printf("0");
        }
    }
}

int main() {
    showbits(3292);
    return 0;
}
```

### Bit Masking
- Semi-common for funtcions to accept bit patterns which indicate true/false operations
- Frequently makes use of bit **masks** which are constants associated with specific bits
- Ex) Unix permissions
- Use them to create options to C functions

```c
#define ADMIN 0b1000
#define CAN_COMMENT 0b0100
#define CAN_WATCH 0b0010
#define CAN_EDIT 0b0001

void do(int perm);

int permissions = do(CAN_COMMENT | CAN_WATCH);
```

### Unix permissions with octal
- Octal arises with **Unix file permissions**
- Every file has 3 permissions for 3 entites
- Permissions are true/false so a single bit will suffice
- `ls -l` shows permissions
- `chmod` 777 is an octal permission
- also honors letters, u=rw, x = execute
- 777 = u+x

### Endiananess: Byte ordering in Memory
- Single bytes like ASCII characters lay out sequentially in memory in increasing address
- Multi-byte entries like 4-byte ints require decisions on byte ordering
- We think of a 32-bit int like this (MOST SIGNIFICANT <------> LEAST SIGNIFICANT)
- There are two options for ordering multi-byte data
    - **Little Endian**: Least significant byte at low address (how we think of it)
    - **Big Endian**: Most significant byte at low address 

### Little Endian vs. Big Endian
- Most modern machines use **Little Endian** ordering by default, (not M86K tho)
- Some processor (ARM, PowerPC) support both Little / Big Endian BUT one is chosen at startup and used until turned off
- Both Big and Little Endian have (minor) engineering trade-offs
- At one time, this was debated heavily among hardware folks: "Gulliver's Travels"
- **Intel Chips** were little endian and x86 dominated computing, which set the precedent
- Big endian byte order is common in **network programming**, as sending bytes over network is done in big endian


## ASM

### The **Many** Assembly Languages
- Most **microprocessors** are created to understand a **binary machine language**
- Machine Language provides means to manipulate internal memory, perform arithmetic, and more

**MOS Technology 6502**
- 8-bit operations, limited addressable memory, **1 general purpose register**, powered notable gaming systems in the 1980s
- Apple lle, Atari 2600, Commodore
- NES/Famicom

**IBM Cell microprocessors**
- Developed in early 2000s, 64-bit, many cores, many registers (32), large addressable space, fast multimedia performance, is a **pain** to program
- PlayStation 3 and Blue Gene Supercomputer

### Our focus: The x86-64 Assembly Language
- x86-64 Targets Intel/AMD chips with 64-bit word size
- Lineage of x86 family
    - 1970s: 16-bit systems like Intel 8086
    - 1990s: IA32 (Intel 32-bit systems like 80386 80486)
    - 2000s: x86-64 (64-bit extension by AMD)
- x86-64 is backwards compatible, consequently much cruft
    - Can run compiled code from the 70's / 80's on modern processors
    - But this means 50 year old instructions must be preserved
    - x86-64 is not an assembly language someone would design from scratch, but is what exists now
    - RISC-V is a new assembly language that is "clean", and has no history to support

>[!WARNING]
> A lot of information available on the web for Intel assembly programming is very OLD, and for IA32 systems, which won't work on 64-bit systems. 

- We are going to use AT&T Syntax

### x86-64 "General Purpose" Registers
- Many general purpose registers have special purposes and conventions associated with them
    - Return Value: `%rax` / `%eax` / `%ax`
    - Function args 1 to 6: `%rdi` / `%rsi`, `%rdx`, `%rcx`, `%r8`, `%r9`
    - Stack pointer (top of stack): `%rap`
    - Old Code Base Pointer: `%rbp`, historically start of current stack frame but is not used that way in modern codes

> [!NOTE]
> There are also Special Registers like `%rip` and `%eflags` which we will discuss later

SEE TABLE

Caller Save: Restore after calling func
Callee Save: Restore before returning
