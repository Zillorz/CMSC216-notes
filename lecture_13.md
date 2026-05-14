# Lecture 13 - 3/12/26

### The test instruction
- `testl %eax %eax` uses bitwise AND to examine a register
- Selected by compiler to check for zero, NULL, negativity, etc.
- Followed by je/jz/jne/jnz/js/jns

Guess: %eax = 26

### call / ret with Return Address in Stack
- `call` instruction
    1. Push the "caller" **Return Address** onto the stack, return address if for instruction after call
    2. Change rip to first instruction of the "callee" function
- `ret` instruction
    1. Set rip to Return Address at top of stack
    2. Pop the Return Address off to shrink stack

This is actually why a function calling itself leads to a stack overflow!

Continuing off of this, this means that the `call` instruction modifies %rsp.
So doing a poor job restoring %rsp can be detrimental to execution.

### Warning: %rsp is important for returns
- When a function is about to return %rsp MUST refer to the memory location of the return address (same place it was at the start)
- `ret` uses `(%rsp)` as return address
- Segmentation faults **WILL** occur if %rsp is wrong
- Stack is often to used to store local variables, stack pointer %rsp is manipulated via push/subq

### Pushing and Popping the Stack
- If local variables or callee save regs are needed on the stack, can use `push/pop` for these
- Push and Pop Instructions are compound: manipulate %rsp and move data in a single instruction
- The stack is a **stack**, so it's FILO
- This can be used to use callee save registers, as you can push them onto the stack, modify them, and then restore then before returning to the caller
