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
Callee Save: Restore before returning
