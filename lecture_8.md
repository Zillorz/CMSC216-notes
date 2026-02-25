# Lecture 8 - 2/24/26

## Character Coding Conventions
- Would be hard to share words if bits didn't have a standard conversion 
- **ASCII**: American Standard Code for Information Interchange
- An old standard for bit/character correspondence
- 7 bits per character, includes upper, lower case and punctuation

### Unicode
- ASCII only has American symbols and alphabet
- The rest of the world doesn't have representation
- Unicode uses 1-4 bytes per character to represent 1,112,064 codepoints
- ASCII only uses 7 bits, so the 8th bit in unicode indictates extension for multiple bytes
- Requires software to understand coding convention allowing broader language support
- ASCII is a proper subset of UTF-8, so it's backwards compatible
- Really annoying to support in C, so we'll ignore it

## Integers

### Binary Adders 
- Adding/subtracting in binary is the same as in decimal
- Carry on values on 2 instead of 10 (when borrowing from above in subtraction)
- Digital circuits can implement arithmetic

### Signed integers

- To represent negative integers, must choose a different coding system
- The **Two's Complement Encoding** is the most common coding system for signed numbers so we will study it
- Alternatives exist
    - Signed magnitude: leading bit pos (0), neg (1)
    - One's complement: invert to go between positive and negative
- Great advantage of two's complement: ==signed and unsigned arithmetic are identical==
- Hardware folks only need to make one set of units for both unsigned nad signed arithmetic


### Two's Complement Integers:

- The most significant bit is a negative power of two.
- For a byte, the msb is multiplied by -2^7.
- Ignore overflow to switch sign. 
- Also adds wrapping mechanics.
- Finally, when positive, representation is same as unsigned

- If there's a leading one, subtract 256 to convert from signed to unsigned, otherwise, leave it the same
- To negate a value in two's complement: invert bits and add 1
    - For max negative value, (e.g. -128), you'll get the max again

### Overflow
- Sums that exceed the representation of the bits associated with the integral type **overflow**
- Excess significant bits are **dropped**
- Addition can result in a sum smaller than the summands, even for two positive numbers
- Integer arithmetic in fixed bits is a mathematical **ring**

### Underflow
- Underflow occurs in unsigned arithmetic when values go below 0
- Pretend that there is an extra significant bi to carry out subtraction
- Subtracting a positive integer from a positive integer may result in a larger positive integer
- Integer arithmetic is fixed in a mathematical **ring**

For example
```c
char c = -128;

printf("-128 * -1 = %hhd\n", -1 * c);

unsigned char g = 255;
printf("255 + 1 = %hhu\n", g + 1);

unsigned char p = 23;
printf("23 - 34 = %hhu\n", p - 34);
```

- Underflow and overflow do not cause runtime errors
- Good for hashing and cryptography
- Bad for other applications, should check in system critical operations
- Ariane Rocket Crash
- Assembly provides condition codes indicating when overflow occurs but checking in C is hard

### Integer Ops and Speed
- Along with Addition and Subtraction, Multiplication and Division can also be done in binary
- Algorithms are the same as base 10 but more painful to do by hand
- The pain is reflected in the hardware speed of these operations
- The Arithmetic and Logic Unit (ALU) does integer ops
- A clock ticks in the machine (cpu) at some rate like 5Ghz (5 billion times per second)
- Under ideal circumstances typical ALU Op speeds are

| Operation   | Cycles   |
|--------------- | --------------- |
| Addition   | 1   |
| Logical   | 1   |
| Shifts   | 1   |
| Subtraction   | 1   |
| Multiplication | 3   |
| Division   | >30   |

- Due to disparity, it is worth knwoing the relation between multiplication, division, and bitwise operations
- Compiler often uses these tricks: shift rather than multiply by divide

Bitwise ops evaluate on a per-bit level


### Bitwise shifts
- Shift operations move bits within a field of bits
- `a << k` shift right k bits
- `a >> k` shift left k bits
- All integral types can use shifts
- Doesn't work on not integer types (floats and ptrs)
- Right shifts respect signed bits

```c
printf("76 * 2 = %d\n", 76 << 1);
printf("76 / 2 = %d\n", 76 >> 1);

printf("-128 / 4 = %d\n", -128 >> 2);
```

Tricks:
Multiply by 2: `a * 2 == a << 1`
Divide by 2: `a / 2 == a >> 1`

- Use `-O3` flag to make the compiler do this for you
- Sometimes programmers will do this, make sure you comment this if you do do it manually.
