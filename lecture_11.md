# Lecture 11 - 3/5/26

> [!NOTE]
> IEEE 754 lecture on canvas, might be useful for bonus points, but not necessary.


### Register Size and Data Movement
- `%rax` is a register, `%eax` is its lower 32 bits
- Data movement involving small registers **may NOT overwrite** higher bits in extended register
- Moving data to low 32-bit regs automatically zeros high 32-bits
```asm
movabsq $0x1122334455667788, %rax
movl $0xAABBCCDD, %eax
## Now, %rax is 0x00000000AABBCCDD
```
- Moving data to other small regs **DOES NOT ALTER** high bits

```asm
movabsq $0x1122334455667788, %rax
movw $0xAABB, %eax
## Now, %rax is 0x112233445566AABB

```

Gives rise to two other families of movement instructions for 
moving little registers (X) to big registers (Y)

### addX: A Quintessential ALU instruction
- Addition represents most 2-operand ALU instructions well
- Second operand A is modified by first operand B, No change to B
- Variety of register, memory, constant, combinations honored
- addX has variants for each register addq, addl, addw, addb

```asm
addq %rdx, %rcx # rcx += rdx
addl %eax, %ebx # ebx += eax
addq $42, %rdx # rdx += 42
addl (%rsi),%edi # edi += *rsi
addw %ax, (%rbx) # *rbx += ax
addq $55, ($rbx) # *rbx += 55

# NO mem+mem or con+con
```

### The Other ALU Instructions
- Most ALU instructions follow the same patter as addX: two operands, second gets changed
- Some one operand instructions as well
- addX, subX, imulX, andX, orX, xorX, salX, shlX, sarX, shrX
- incX, decX, negX, notX - one parameter
- Most are pretty self-explanatory
- sa(l|r) and sh(l|r), these swift right and left, the 'a' variants keep the sign, while the h variants do not

### Division
Division is painful

- idivX operation has some special rules
- Dividend must be in the rax/eax/ax register
- Sign extend to rdx/edx/dx register with cqto
- idivX takes on register argument which is the divisor
- At completion
    - rax/eax/ax holds quotient
    - rdx/edx/dx holds the remainder
- cwtl, cltq, cqto > convert (word/long/quad) to (long/quad/octo)
- When performing 8-bit or 16-bit quantities, use instructions to sign extend small reg to all rax register

## Control Flow in Assembly

### The instruction pointer
- %rip: **special register** referred to as the **instruction pointer** or program counter
- %rip contains main memory address of next assembly instruction to execute
- After executing an instruction, %rip automatically updates to the subsequent instruction
- OR in a Jump instruction, %rip changes non-sequentially
- **Do not** add/subtract with %rip via addq/subq: %rip automatically updates after each instruction

### Jump instructions
- **Labels** in assembly indicate jump targets like .LOOP
- **Unconditional Jump**: always jump to a new location changing %rip non-sequentially 
- **Comparison/TEST** Instruction, sets EFLAGS bit indicating relation between registers/value
- **Conditional Jump**: Jumps to a new location if certain bits of EFLAGS are set by changing %rip non-sequentially, otherwise continues sequential execution
