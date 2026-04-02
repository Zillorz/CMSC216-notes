
| Name | Must save? | Notes |
| ---- | ---- | ---- |
| `%*ax` |  | Return Val |
| `%*bx` | Y |  |
| `%*cx` |  | Arg 4 |
| `%*dx` |  | Arg 3 |
| `%*si` |  | Arg 2 |
| `%*di` |  | Arg 1 |
| `%rsp` | Y | Stack Ptr |
| `%rbp` | Y | Base Ptr? |
| `%r8*` |  | Arg 5 |
| `%r9*` |  | Arg 6 |
| `%r10*` |  |  |
| `%r11*` |  |   |
| `%r12*` | Y |   |
| `%r13*` | Y |   |
| `%r14*` | Y |   |
| `%r15*` | Y |   |

Caller Save: Restore after calling func
Callee Save (Y): Restore before returning

**ALWAYS USE MOVQ FOR ADDRESSES**, anything else is undefined behavior and likely a segfault

WE WORK IN **LITTLE ENDIAN**
- least significant at low memory/address
- moving a value into memory is right to left->low to high

**THIS DOESN'T APPLY TO LEAQ**

| Style | C-like | Notes |
| --------------- | --------------- | --------------- |
| $21, $0xD2 | 21 | constant |
| %rax | rax | register content |
| (%rax) | `*rax` | deref |
| 8(%rax) | `*(rax + 2)`, `rax->field` | base plus const  |
| (%rax, %rbx) | `*(rax + rbx)`, `arr[rbx]` | no sizeof |
| (%rax, %rbx, 4) | `*(rax + rbx * 4)` | sizeof |
| 1024 | `(void*)(1024)` | Absolute memory address|


Often structs are packed into registers
```c
struct Pack {
    int a;
    short c;
    short b;
}
```
would be passed in just %rdi, with the first 32 bits being a, next 16 being c, and last 16 being b
