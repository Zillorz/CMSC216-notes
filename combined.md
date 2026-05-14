If you don't want to use the lecture_xx.md files, this is a collection of all of them in order.

Lectures 1 & 2 were online and therefore skipped. The content in those lectures is somewhat trivial and you should be fine wo them.

This file can also be useful for searching, but I would recommend using grep or rg instead.

# Lecture 3

### Traditional data types

| Bytes [^1]| Name | Range |
| ------------- | ------------- | -------------- |
|   | INTEGRAL |   |
| 1 | char | 0 to 255 [^3] |
| 2 | short | -32,768 to 32,767 |
| 4 | int | -2^31 to 2^31 - 1 |
| 8 | long | -2^63 to 2^63 - 1 |
| ------------- | ------------- | -------------- |
|   | FLOATING |   |
| 4 | float | +- 3.4e+38 |
| 8 | double | +- 1.8e+308 |
| ------------- | ------------- | -------------- |
|   | POINTER |   |
| 8 [^2]  | pointer  | Either 64 bits    |
|   | array | type of pointer |

[^1]: These sizes are not standard, portable code cannot assume any given size
[^2]: We run 64 bit computers, so we will assume 8 bytes for every pointer
[^3]: Fun fact about characters, these aren't always guaranteed to be signed. This is also implementation dependent and portable code cannot assume signed or unsigned chars. On x86_64 machines, these are signed. On ARM64, it depends on the machine, where MacOS/IOS sign chars, but Windows, Linux, and Android do not.

### More about pointers
- Pointers are written with a *, e.g. 
- `double *d` `int *p`, `char *s`, `void *p`
- An array is a pointer to a fixed location
- Array indexing is a pointer deference
- `array[12]` = `*(array + 12)`
- Strings are represented by `char*` or `char[]`, there isn't a dedicated string type 
- `void` normally means nothing, but a `void*` is just a pointer to a memory address that stores anything

### Recall: Pointers, Address, and Dereferences
`type *ptr` Declares a pointer variable
`type* ptr` Declares a pointer variable
`*ptr` Deferences a pointer

```c
int *iptr;
int x = 7;
iptr = &x;
int y = *iptr;
*iptr = 9;

double z = 1.23;
double *dptr = &z;
*dptr = 4.56;

printf("%f", z);

// At the end of this code
// x = 9, y = address of x, z = 4.56
```


### Pointers are typed
> [!CAUTION]
> The following code doesn't compile
> ```c
> int a = 10;
> int *aptr = &a;
> double b = 4.56;
> aptr = &b; // error
> ```

A pointer cannot contain another type without a cast. But this is C, we can do whatever we want lmao.

```c
int a = 5;
double x = 1.2345;
void *ptr;

ptr = &a;
// we set b to the dereferenced value of ptr, after casting ptr to an int pointer
int b = *((int *) ptr); 
// b = 5

ptr = &x;
// we set y to the dereferenced value of ptr, after casting ptr to a double pointer
double y = *((double *) ptr);
// y = 1.2345

// SAFETY: ptr points to a double, but we casted it to an int??
int c = *((int *) ptr);
printf("%d", c); // 309237645
```
'c' will have the same bits as 'x' (at least the first 32 bits of x), but the alternate formats and size cause 'c' to be a completely gibberish number.

### Void pointers
- A pointer which points to ==anything==
- Useful to store an arbitrary memory address
- Remove compilers ability to type check, so introduces risk
- Sometimes this can be useful, quake 3 fast inv sqrt, but that kind of optimization is way beyond the scope of a normal program


**Byte level picture, see slides ig**
- you should understand ts


### More Types?
There are actually more types

| Name   | Range   |
|--------------- | --------------- |
| unsigned char   | 0 to 255   |
| unsigned short   | 0 to 65,535   |
| unsigned int   | 0 to 2^32 - 1   |
| unsigned long   | 0 to 2^64 - 1   |

stdint.h defines int64, uint64 and other int types with guaranteed sizes


## Arrays
- A continuous block of homogeneous data
- Automatically allocated by compiler with a fixed size [^4]
- Uses \[ \] syntax
- Refer to element with arr\[3\]
- Bare name 'arr' is the memory address where array starts
- This actually means arrays are also pointers!
- And array indexing is pointer dereferenced, `array[3]` == `*(array + 3)`

[^4]: This is referring to fixed size arrays, not heap allocated arrays made with malloc/calloc

# Lecture 4 - 2/5/26

Note: the compiler does not do array bounds checking 
> [!WARNING]
> The following code exhibits undefined behavior
> ```c
> int a[3] = { 1, 2, 3 };
> printf("%d", a[3]);
> printf("%d", a[4]);
> ```
These out of bounds accesses work, but return garbage, and may cause segfaults. 

### Pointers and Arrays are related

| Property | Pointer | Array |
| --------------- | --------------- | --------------- |
| Declaration | int *p = &x; | int a\[2\] = {1, 2}; |
| Refers to | Memory location | Memory location |
| Location | Assigned by you | Assigned by compiler |
| # items | Many | Many |
| Brace index? | Yes: p\[0\] | Yes: a\[0\] |
| Dereference | Yes: *p | No |
| Arithmetic | Yes: ++p | No |
| Assign to array? | *p = a | No |
| Interchangeable | Yes | Yes |
| Has length propert | No | No |

```c standalone
void print_arr(int a[]) {
    printf("Array - %p: %d\n", a, a[0]);
}

void print_ptr(int *p) {
    printf("Pointer - %p: %d\n", p, *p);
}

int main() {
    int *p = NULL;
    printf("Null pointer - %p: %p\n", &p, p);

    int x = 21;
    p = &x;

    // interchangeable
    print_arr(p);

    int a[] = { 5, 10, 15 };

    // interchangeable
    print_ptr(a);

    // a stores the memory address of a[0], therefore a == &a[0]
    // this means a is a valid ptr
    p = a; 
    print_ptr(a);
    return 0;
}
```
This also means that `a[1]` == `*(a + 1)`

> [!NOTE]
> My answers to exercise
> ```
> #4936: 5
> #4940: 10
> #4944: 15
> #4952: ?
> ```

> [!WARNING]
> This code causes a segmentation fault (exit code: -1073741819)
> ```c
> printf("Hello!\n");
> int x = 1;
> int *p = &x;
> p += 923723; // Pointer now points to the wrong place
> printf("%d", *p); // Seg fault
> ```

### Pointer Arithmetic Alternatives
Pointer arithmetic causes problems, so we should try to avoid it.

Example #1
```c

double arr[5] = {};

printf("enter 5 numbers\n");
for(int i = 0; i < 5; i++) {
    // Don't use this
    // scanf("%lf", arr + i);
    scanf("%lf", &arr[i]); 
}

// Don't use this
// printf("Your last number was %lf \n", *(arr + 4));
printf("Your last number was %lf \n", arr[4]);
```

Sometimes pointer manipulation is useful
```c
char name[128];
printf("First name: ");
scanf(" %s", name); // reads into name

int len = strlen(name);
name[len] = ' '; // replaces '\0' with space ' '

printf("Last name: ");
scanf(" %s", name + len + 1); // makes scanf fill after the first name + ' '

printf("Hello %s", name); // now name is 'firstname lastname\0'
```

C strings end with the null terminator '\0' and have no length property.
Accessing the length with a function like `strlen` is actually `O(n)`

Another thing the null terminator, not having it in a string can cause undefined behavior. 
For example, calling `printf` or `strlen` would cause problems.

`scanf` can also cause a buffer overflow, if the user inputs more than 128 characters, which is also undefined behavior! You can try this with the above example!

### Strings are character arrays
- Convention is to use `char*` as strings
- Strings end with the null terminator '\0'
- Character arrays need a terminating '\0' at the end
- Null termination is done automatically for constants
- Null termination is also done by standard functions like `scanf`

> [!WARNING]
> When manipulating strings, make sure you don't remove/modify the null terminator!
> The built-in `fread()` does not append nulls on binary data


#### String Library
- Include with `#include <string.h>`   
- Null termination expected
- `strlen(s)` - length of string
- `strcpy(dst, src)` - copies string from src to dst
- `strcmp(s1, s2)` - compares string lexicographically, 0 if equal

# Lecture 5 - 2/10/26

## Dynamic memory allocation

- Most C data has a fixed size (known at compile time)
- `malloc(nbytes)` is used to allocate memory
    - one argument: the number of bytes
    - frequently used with `sizeof()` operator, which returns the amount of bytes a type is
    - returns a `void*` to bytes, or `NULL` if there was not enough space
    - `free()` is used to release memory

You can use this for arrays with sizes that are determined at runtime
```c
int len;

printf("How many ints: ");
scanf("%d", &len);

int *nums = malloc(sizeof(int) * len);

printf("Type your numbers:\n");

for(int i = 0; i < len; i++) {
    scanf("%d", &nums[i]);
}

printf("Your numbers are: ");
for(int i = 0; i < len; i++) {
    printf("%d ", nums[i]);
}
printf("\n");
free(nums); // Don't forget to free!
```

## Compile and Runtime vs Memory Management

### Compile time sizes
- Some sizes are known at **Compile Time**
- Compiler can calculate sizes of fixed variables, arrays, and sizes of stack frames
- These are automatically managed, don't require the use of malloc/free

### Run Time Sizes 
- Compiler can't predict the future, at **Run Time** programs must react
    - Typed user input, size of file to be read
    - Elements to be added to a data structure
    - Memory allocated in one function and return to another
- As these things are determined, `malloc()` is used to allocate memory in the **heap**
- When it is finished, `free()` it
- This is similar to the `new` keyword in Java, but these aren't automatically freed by a GC

### The 4 logical regions of program memory
A running program has 4 regions of memory
1. **Stack**: automatic, push/pop with function calls
2. **Heap**: `malloc()` / `free()`
3. **Global**: variables outside of functions, `static` vars
4. **Text**: Program instructions in binary

- Stack grows towards the heap, if they collide, we get a Stack Overflow error. [^5]
- Global and Text regions are usually fixed in size.
- "Logical Regions" for humans to organize their programs; no physical differences for regions[^6]

[^5]: Code block below demonstrates stack overflow (code: -1073741571), on GitHub might be somewhere else
```c
int expand_stack() {
    expand_stack();
}

expand_stack();
```

[^6]: Technically memory can have different physical locations (CPU cache vs RAM) but these aren't covered until WAY later in the course

Common misconception: `sizeof(thing)`

- `sizeof(thing)` returns a **Compile Time Size** of thing from C source
- Useful when `malloc()`'ing stuff like `int *arr = malloc(count * sizeof(int));`
- **NOT USEFUL** for sizes of arrays or strings
```c
int *arr = malloc(32 * sizeof(int)); // good use of sizeof
int nelems = sizeof(arr); // BAD USAGE!
printf("Num elements: %d", nelems); // This always prints 8!
```
- For array size, search for sentinel or be given size
- For string size, use `strlen`

### Memory Tools on Linux
Valgrind: Suite of tools including Memcheck
- Catches most memory errors
    - Use of uninitialized memory
    - Reading/writing freed memory
    - Reading/writing before/after malloc block
    - Memory leaks
- Source line where problem arose (but not its cause)
- Super easy to use: `valgrind ./program`
- Slows execution of program down

# Lecture 6 - 2/12/26

> [!WARNING]
> The following code has undefined behavior
> If you don't see undefined behavior, try running with `-O2` or `-O3`
```c standalone
#include <stdio.h>
#include <stdlib.h>

int *ones_array(int len) {
  int *arr = malloc(sizeof(int) * len);
  for(int i = 0; i < len; i++) {
    arr[i] = 1;
  }
  free(arr); // This is bad place to free, as the caller wants to use this int*
  return arr;
}

int main() {
  int *ones = ones_array(5);

  for(int i = 0; i < 5; i++) {
    printf("ones[%d] = %d\n", i, ones[i]); // this is a use after free, trash data
  }

  free(ones); // this is a double-free
  return 0;
}
```

- Once a malloc area is freed, it is no longer valid
- Don't free the return value of a function 
- Never `free()` twice

## Structs
Heterogeneous Groupings of Data
- Arrays are homogeneous; all elements the same type
- `structs` are C's way of defining heterogeneous data
- Each **field** can be a different kind/type
- One instance of a `struct` has all fields
- Access elements with 'dot' or '->' notation
- Several syntaxes to declare, but we will use the modern approach
- Convention: types have _t suffix
- Similar to classes in Java: but have no constructors, methods, variables, access modifiers, or anything else

```c
typedef struct {
    int a;
    double b;
    char first_letter;
    char letters[6];
} thing_t;

thing_t at;
at.a = 12;


thing_t bt = { .a = 2, .b = 2.2, .first_letter = 'c' };
thing_t ct = { .first_letter = 'j', .letters = "james" };
```

### Structs in memory
- Structs allocated in memory are laid out compactly
- Compiler may *pad* fields to place them at nice alignments
    - The compiler likes items starting at multiples of 4/8

```c lib
typedef struct {
    double x;
    int y;
    char nm[4];
} small_t;
```

### Structs: Dots vs Arrows
```c
small_t small; // 16 bytes
small_t *sptr; // 8 bytes, this is a ptr

small.x = 1.23; // use the '.' when you have a struct
sptr->y = 4; // use the '->' when you have a ptr to a struct
```

The 'a->b' is syntatic sugar for '(*a).b'
- Use '.' on struct 
- Use '->' on struct ptr

### Structs in arrays
Extra notes (not in lecture):
```c
printf("%d", sizeof(small_t)); // Prints out 16
small_t *small_things = malloc(sizeof(small_t) * 12); // use sizeof

small_things[10].x = 1.4444; // use [] indexing and . to modify
                             // no need for arrow as [] does the dereference

small_things->x = 3.222; // BAD!
// this is equal to small_things[0].x = 3.222, but much less readable;
```


### File Input and Output

- Standard C I/O functions for reading/writing file data.
- Work with text data; formatted for human reading;

- `FILE` is a type that represents a file, mostly used through `FILE*`
- Standard functions
    - `fopen`, `fscanf`, `fclose`, `rewind`, `fprintf`, `fread`
    - Use man for what these do, it's pretty well defined
    - 'r' for reading, 'w' for writing, 'a' to append (write without clearing)
    - `fscanf` and `fprintf` work the same as `scanf` and `printf`, but with a `FILE*` as the first argument (which is read from or written to instead of stdin/stdout)
    - `fread` reads one char from file (returns int)
    - `fclose` frees a `FILE*`
    - `rewind` goes back to start of `FILE*`, allows for re-reading
    - if `fscanf` or `fread` returns `EOF`, file is over, EOF = End of File
    - `fgetc` gets one char, returns `EOF` when file is over
    - `fgets` reads one line

# Lecture 7 - 2/17/26

### A little more on file input
- `fread()`, this reads RAW bytes from a file
- `fwrite()`, this writes RAW bytes to a file

Use to store and read struct
```c 
typedef struct {
    double x; int y; char nm[4];
} small_t;

small_t sarr[2] = {
    { .x = 5.4, .y = 4, .nm = "jam" },
    { .x = 4.3, .y = 2, .nm = "ra" }
};

FILE *fout = fopen("small.m", "wb");
int wc = fwrite(&sarr, sizeof(small_t), 2, fout);

printf("Wrote %d items.\n", wc);
fclose(fout);

small_t* fs = malloc(sizeof(small_t) * 2);

FILE *fin = fopen("small.m", "rb");
int rc = fread(fs, sizeof(small_t), 2, fin);

printf("Read %d items.\n", rc);
fclose(fin);

// This will print 'jam'
printf("%s\n", fs[0].nm);
free(fs);
```

- Binary files usually smaller than text and can be directly read into memory
- Text data is easier to read

### Common C Operators
- Arithmetic `+ - * / %`
- Comparison `== > < <= >= !=`
- Logical `&& || !`
- Memory `& *`
- Compound `+= -= *= /=`
- Bitwise `^ | & ~ << >>`
- Conditional `? :`

### Common control structures
- While loops, for loops, do while loops, breaks, continues
- if, else if, else, goto [^7]
- labels:
- Very basic, same as Java

[^7]: DO NOT USE GOTO, you need permission from Kauffman to not lose ALL style points with a goto :/

`switch()/case`: The **worst** control structure

Kauffman really, really hates switch statements. NEVER use them unless you're writing a emulator?

### A program is born
- Write some C code in `program.c`
- Compile it with a C compiler
    - `gcc -o program program.c`
    - `clang -o program program.c`
- Compilation is a multi-step process
    - Check syntax for correctness/errors
    - Perform optimizations on the code if possible
    - Translate result to **ASM** for specific target e.g. ARM64, AMD64
    - Assemble code into object code
    - Link required libraries
- Result: executable program
- Running a program requires a loader

**END OF EXAM 1 CONTENT**

### Binary encoding
- Unsigned integers are always positive (recall [lecture 3](/lecture_03.md))
- To understand binary encoding, first recall base 10 
- For any base, numbers are represented as follows
- C\_0 + C\_1 * B^1 + C\_2 * B^2 + C\_3 * B^3 ...
- Where 0 <= C_N < B
- And where B is the Base

Example (Base 2):
- Numbers are represented as 
- C\_0 + 2 * C\_1 + 4 * C\_2 ....
- For example, the number 01101
- 1 + 2 * 0 + 4 * 1 + 8 * 1 + 16 * 0 = 13

- Going backwards is more complicated...
- A very simple algorithm

```c standalone
char* reverse(char* inp) {
    int len = strlen(inp);
    char* rev = malloc(sizeof(char) * len + 1);

    for(int i = 0; len > i; i++) {
        rev[i] = inp[len - i - 1];
    }

    rev[len] = '\0';
    free(inp); // bad practice to free an argument
    return rev;
}

char* dec_to_binary(unsigned int i) {
    // 32 bytes for int(32 bit) and 1 for \0
    char* c = malloc(sizeof(char) * 33);
    int p = 0;

    while (i != 0) {
        if (i % 2) {
            c[p++] = '1';
        } else {
            c[p++] = '0';
        }
        i /= 2;
    }


    c[p] = '\0';
    return c;
}

int main() {
    unsigned int d;
    printf("Enter your number: ");
    scanf("%ud", &d);

    char* res = reverse(dec_to_binary(d));
    printf("That is %s in binary", res);
    free(res);
}
```

### Other bases (OCTAL and HEXADECIMAL)
- Printing them out
- "%d": Decimal
- "%o": Unsigned octal
- "%u": Unsigned decimal 
- "%x": Unsigned hexadecimal
- Octal is 0 through 7, base 8
- Hexadecimal is 0 to F (switches to letters after 9)

- A trick to convert these, go to binary, then to them!
    - Group in 4 (as 2^4 = 16) for hex
    - Group in 3 (as 2^3 = 8) for octal

# Lecture 8 - 2/24/26

## Character Coding Conventions
- Would be hard to share words if bits didn't have a standard conversion 
- **ASCII**: American Standard Code for Information Interchange
- An old standard for bit/character correspondence
- 7 bits per character, includes upper, lower case and punctuation

### Unicode
- ASCII only has American symbols and alphabet
- The rest of the world doesn't have representation
- Unicode uses 1-4 bytes per character to represent 1,112,064 code points
- ASCII only uses 7 bits, so the 8th bit in unicode indicates extension for multiple bytes
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
- A clock ticks in the machine (CPU) at some rate like 5GHZ (5 billion times per second)
- Under ideal circumstances typical ALU Op speeds are

| Operation   | Cycles   |
|--------------- | --------------- |
| Addition   | 1   |
| Logical   | 1   |
| Shifts   | 1   |
| Subtraction   | 1   |
| Multiplication | 3   |
| Division   | >30   |

- Due to disparity, it is worth knowing the relation between multiplication, division, and bitwise operations
- Compiler often uses these tricks: shift rather than multiply by divide

Bitwise ops evaluate on a per-bit level


### Bitwise shifts
- Shift operations move bits within a field of bits
- `a << k` shift left k bits
- `a >> k` shift right k bits
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

# Lecture 9 - 2/26/26

### Showing bits
- `printf()`
    - `%d` is Decimal
    - `%x` is Hexadecimal
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
    int n;
    printf("Number: ");
    scanf("%d", &n);
    showbits(n);
    return 0;
}
```

### Bit Masking
- Semi-common for functions to accept bit patterns which indicate true/false operations
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
- Every file has 3 permissions for 3 entities
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


| 64-bit | 32-bit | 16-bit | 8-bit | Saved By | Notes |
| ---- | ---- | ---- | ---- | ---- | ---- |
| `%rax` | `%eax` | `%ax` | `%al` | **Caller** | Return Val |
| `%rbx` | `%ebx` | `%bx` | `%bl` | **Callee** |  |
| `%rcx` | `%ecx` | `%cx` | `%cl` | **Caller** | Arg 4 |
| `%rdx` | `%edx` | `%dx` | `%dl` | **Caller** | Arg 3 |
| `%rsi` | `%esi` | `%si` | `%sil` | **Caller** | Arg 2 |
| `%rdi` | `%edi` | `%di` | `%dil` | **Caller** | Arg 1 |
| `%rsp` | `%esp` | `%sp` | `%spl` | **Callee** | Stack Ptr |
| `%rbp` | `%ebp` | `%bp` | `%bpl` | **Callee** | Base Ptr? |
| `%r8` | `%r8d` | `%r8w` | `%r8b` | **Caller** | Arg 5 |
| `%r9` | `%r9d` | `%r9w` | `%r9b` | **Caller** | Arg 6 |
| `%r10` | `%r10d` | `%r10w` | `%r10b` | **Caller** |  |
| `%r11` | `%r11d` | `%r11w` | `%r11b` | **Caller** |   |
| `%r12` | `%r12d` | `%r12w` | `%r12b` | **Callee** |   |
| `%r13` | `%r13d` | `%r13w` | `%r13b` | **Callee** |   |
| `%r14` | `%r14d` | `%r14w` | `%r14b` | **Callee** |   |
| `%r15` | `%r15d` | `%r15w` | `%r15b` | **Callee** |   |

Caller Save: Restore after calling func
Callee Save: Restore before returning

# Lecture 10 - 3/3/26 

> [!NOTE]
> You can use gdb on ASM code too!

To run an assembly file, let's use an assembler, which is included with gcc/clang!
`gcc hi.asm` compiles the assembly!

To compile a c file to assembly, run `gcc -S hi.c`, 
use the -Og flag to make it more readable.

> [!WARNING]
> We use AT&T syntax, intel syntax will result in a ZERO on exam/lab/projects

### Basic Instruction Classes
- Remember: Goal is to understand assembly as a target for higher level languages
- Means we won't hit all 4,834 pages of the x86-64 manual
- Brown University's x64 cheat sheet is a good overview
- x86 assembly from Yale is also good, but limited to 32bit

### Data Movement: mov instruction

#### Overview
- Moves data
    - Reg to Reg
    - Mem to Reg
    - Reg to Mem
    - Imm to ...
- Reg: register
- Mem: main memory
- Imm: "immediate value" (constant), must start with $

#### Variations
- movq, movl, movw, movb
- b -> byte, w -> word, l -> long (2) word, q -> quad word
- b = 8bit, w = 16bit, l = 32bit, q = 64bit


### Operands and Addressing Modes

| Style | Address Mode | C-like | Notes |
| --------------- | --------------- | --------------- | --------------- |
| $21, $0xD2 | Immediate | 21 | value of constant |
| %rax | register | rax | to/from register contents |
| (%rax) | indirect | `*rax` | deref |
| 8(%rax) | displaced | `*(rax + 2)`, `rax->field` | base plus const  |
| (%rax, %rbx) | indexed | `*(rax + rbx)`, `arr[rbx]` | no sizeof |
| (%rax, %rbx, 4) | scaled index | `*(rax + rbx * 4)` | sizeof |
| 1024 | absolute | `(void*)(1024)` | Absolute memory address|

> [!WARNING]
> ALWAYS USE **MOVQ** to copy memory addresses, as movl will not correctly copy, and will cause segfault

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

# Lecture 16 - 3/31/26
Unix processes and exceptional control flow

### Traditional vs Modern Computing Devices
- Old-school computers had a single executing programs which could interact freely with all parts of the computing hardware
- Modern computing devices have different expectations summarized below

| Traditional   | Modern |
|--------------- | --------------- |
| Single program on device   | Multiple programs sharing single device   |
| No Operating System | OS manages all programs   |
| Program has access to all hardware | OS controls/coordinates hardware access |
| Relatively simple hardware interactions | OS isolates memories of each program |
| Single "user" running programs at once | Complex interactions of many devices |
| Apple II: insert disk to run program | Mac OS: Click to start another program |

- New hardware and expectations led to new concepts
- **Operating Systems**: "manager" program that coordinates activities of all programs / users, manages hardware and provides abstraction layer, enforces security and fairness
- **Process**: a running program with its context

### OS Kernel and Kernel Mode
1. The central or most important part of something

Operating System code is usually in the **kernel**, a program that starts running when a computing system is powered on. 

- Kernel sets up handlers for various exceptional control flows
- Most CPUs have (at least) two modes
    1. **User / Normal** mode
    2. **Kernel / Privileged / Supervisor**  mode
- User programs run in user mode, cannot directly manipulate hardware or access to certain resources
- Requests OS to perform some operations which jumps to kernel code running in kernel mode

Ex) BSD, Linux, NT, XNU

### Processes: Running Programs
- Hardware just executes a stream of instructions
- The OS creates the notion of a **process*, a running program and its resources
- Processes can be executed for a while, then paused while another process executes, then continues
- To accomplish this, OS usually provides
    1. Bookkeeping info for processes (resources)
    2. Ability to interrupt / pre-empt a running process to allow OS actions to take place
    3. Scheduler that decides which process runs and for how long
- Will discuss these from a **Systems Programming** perspective: control low-level program details to utilize OS Service Hardware as effectively as possible

### Exceptional Control flow
- CPUs use "regular" control flow most of the time but support several kinds of **exceptional control flow**
- General idea is as follows

- Kernel tracks data structures associated with Processes that allows them to be paused/resumed
- **Process Context** includes
    - Values of registers
    - Regions of main memory
    - Open files and resources
- Switching between processes is a **Context Switch**
    - OS saves context A somewhere, and loads context B for other program
    - Later, can resume with the context A that we saved

#### Enable Multiple Processes to Share the CPU
- OS sets a hardware timer
- OS Starts Process A running, A's code runs in user mode;
- When timer expires ("rings"), control jumps to the OS
- OS can select Process B to run, resuming A later after B's timer expires

#### Hiding I/O Latency
- Processes pause while waiting for I/O, allowing other code to run while I/O

> [!NOTE]
> CMSC412: Operating Systems
> CMSC412 covers a lot more details

### Overview of Process Creation/Coordination
- The following are the 4 fundamental process creation / coordination system calls provided by UNIX
- `getpid() / getppid()`
    - Get process ID of self, or parent
- `fork()`
    - Create a child process
    - Identical to parent EXCEPT for return value of fork() call and PID
    - returns 0 if child, otherwise returns id of child
    - Determines child/parent
    - calling fork is recursive/exponential, the following code prints **4** times
```c
fork();
fork();
printf("Hi!");
```

- `wait() / waitpid()`
    - Wait for any child to finish
    - Wait for a specific child to finish
- `exec() / execvp()`
    - Replace running program with another executable

> [!NOTE]
> This [video](https://youtu.be/SwIPOf2YAgI), is **really** good for this

# Lecture 17 - 4/7/26

### Effects of fork()
- Duplicate process
- Only difference is return value of `fork()`
- returns 0 if child, otherwise returns id of child
- exponential, `fork(); fork(); fork();` leads to 8 copies of the program


### Coordination is hard
Coordination two programs is hard. The order of execution is random and unreliable. Ensure that your code doesn't depend on a specific order without a `wait()` call. Debugging is especially hard as the scheduler is hard to predict and control/influence.

### Child exit status
- Calls to `wait()` can set a status variable indicating the fate of a child
- Several **macros** are used to parse that
- zero = normal, non-zero = error

- You can `raise()` a signal

## Blocking vs. Non-blocking Activities
**Blocking**
- A call to `wait()` or `waitpid()` may cause calling process to **block**
- Blocking is associated with other activities as well
    - I/O, obtain a lock, get a signal, etc.
- Generally creates **synchronous** situations: waiting for something to finish means everything happens in order
**Non-blocking**
- Contrast with **non-blocking** (asynchronous) activities: calling process goes ahead even if something isn't finished yet
- `wait()` is blocking, but `waitpid()` can be blocking or non-blocking 
- idea behind async runtimes like tokio

### Non-Blocking waitpid()
- Use the `WNOHANG` option
- Returns immediately regardless of the child's status

### Zombie Processes
1. Parent creates a child
2. Child completes
3. Child becomes a **zombie**!
4. Parent waits for child
5. Child reaped

Zombie Process: A process that has finished, but has not been `wait()`'ed for by its parent yet so it cannot be eliminated from the system. OS can reclaim child resources like memory once parent `wait()`'s 

### Orphans are always Adopted
- Parent exits without `wait()`ing leaving them orphaned.
- Adopted by root process with PID=1 (systemd, launchd, etc...)

### Reapers and the Subreapers
- Process X creates many children, Orphans them
- Children of X complete, becomes Zombies until...
- Newly assigned Parent `wait()`'s for them
- Adoptive parent like Process 1 sometimes referred to as a **Reaper** process: "reaps the dead processes"
- System may designate a *Subreaper* to do this per user so orphans NOT re-parented to process ID 1
- Graphical Login on Ubuntu Linux systems usually designates a Subreaper for each user

# Lecture 18 - 4/9/26

### The Process Table
- OS maintains data on all processes in a Process Table 
- Process Table Entry = Process Control Block
- Contains info like PID, instruction that process is executing, Virtual Memory Address Space and **Files in Use**

### File Descriptors
- Each Process Table entry contains a table of open files
- User Programs refers to these via **File Descriptors (FD)**
- File Descriptor is an integer index into Kernel's table
```c
int fd = open("some_file.txt", O_RDONLY)`
```
- FD Table entry refers to other Kernel/OS data structures

- Unix presents many things via files/file descriptors
- Many Unix system actions are handled via `read()` or `write()`
- FDs allow interaction with "normal" files like `myfile.txt` or `commando.c` to read/change them
- FDs also allow interaction with many other things
    - Pipes for interprocess communication
    - Sockets for network communication
    - Special files to manipulate terminal, audio, graphics, etc.
    - Raw block of Memory Shared between Processes
    - Even processes themselves have special files in the file system: `ProcFs` in /proc/PID# provides info on running processes
- We will focus on standard File I/O using FDs now and touch on some broader uses Later
- Also must discuss FD interactions with previous System Calls (open files when you fork?)

### Open and Close: File Descriptors for Files
`open()`/`close()` show common features of many system calls
- Returns -1 on errors
- Show errors using the `perror()` function
- Use of vertical pipe (|) to bitwise-OR several options/flags

### read() from File Descriptors
- Read up to SIZE from an open file descriptor
- Bytes stored in buffer, overwrite it
- Return value is number to bytes read, -1 for error
- SIZE commonly defined but can be variable, constant, etc.

> [!CAUTION]
> Bad things happen in buffer is actually smaller than SIZE
> `read()` does NOT null terminate, add '\0' manually

### Behavior of read()
- OS maintains **file positions** for each open File Descriptor
- I/O functions like read() use/change position in a file
- `read()` overwrites input array
- OS **does not** update positions in user arrays
- `read()` returns # bytes read, or 0 when at EOF

> [!WARNING]
> `read()` DOES not need to return the number of bytes you asked for, even if that number of bytes IS available

### write() to File Descriptors
- Write up to SIZE bytes to open file descriptor
- Bytes taken from `buffer`, leave in intact
- Return value is number of bytes written, -1 for error

# Lecture 19 - 4/14/26

### read()/write() work with bytes
- In C, byte=char
- Not so for other types: int is often 4 bytes
- Require s care with non-char types
- All calls read/write actual bytes

### Standard File Descriptors
- When a process is born, comes with 3 open file descriptors
- Related to FILE* streams

| Symbol | # | FILE* | FD for... |
| -------- | ------- | ---------- | --------- |
| STDIN_FILENO | 0 | stdin | standard input (keyboard) |
| STDOUT_FILENO | 1 | stdout | standard output (screen) |
| STDERR_FILENO | 2 | stderr | standard error (screen) |


### Shell I/O Redirection
- Shells can direct input / output for programs using < and >

```bash
some_program > output.txt
# output in output.txt

interactive_prog < input.txt
# input.txt rather than typing

some_program &>> everything.txt
# stdout and stderr into everything.txt

some_program 2> /dev/null
# no stderr
```

### Manipulating the File Descriptor Table
- System calls `dup()` and `dup2()` manipulate the FD table
- `int backup_fd = dup(fd);` copies a file descriptor
- `dup2(src, dst);` copies fd at src to dst
(See picture)

### C `FILE` structs use file descriptors in UNIX
Typical Unix implementation of standard I/O library `FILE` is
- A file descriptor
- Some buffers with positions
- Some options controlling buffering

For example, the FILE struct on Darwin/XNU
```c
typedef	struct __sFILE {
	unsigned char *_LIBC_UNSAFE_INDEXABLE	_p;	/* current position in (some) buffer */
	int	_r;		/* read space left for getc() */
	int	_w;		/* write space left for putc() */
	short	_flags;		/* flags, below; this FILE is free if 0 */
	short	_file;		/* fileno, if Unix descriptor, else -1 */
	struct	__sbuf _bf;	/* the buffer (at least 1 byte, if !NULL) */
	int	_lbfsize;	/* 0 or -_bf._size, for inline putc */

	/* operations */
	void	*_cookie;	/* cookie passed to io functions */
	int	(* _Nullable _close)(void *);
	int	(* _Nullable _read) (void *, char *_LIBC_COUNT(__n), int __n);
	fpos_t	(* _Nullable _seek) (void *, fpos_t, int);
	int	(* _Nullable _write)(void *, const char *_LIBC_COUNT(__n), int __n);

	/* separate buffer for long sequences of ungetc() */
	struct	__sbuf _ub;	/* ungetc buffer */
	struct __sFILEX *_extra; /* additions to FILE to not break ABI */
	int	_ur;		/* saved _r when _r is counting ungetc data */

	/* tricks to meet minimum requirements even when malloc() fails */
	unsigned char _ubuf[3];	/* guarantee an ungetc() buffer */
	unsigned char _nbuf[1];	/* guarantee a getc() buffer */

	/* separate buffer for fgetln() when line crosses buffer boundary */
	struct	__sbuf _lb;	/* buffer for fgetln() */

	/* Unix stdio files get aligned to block boundaries on fseek() */
	int	_blksize;	/* stat.st_blksize (may be != _bf._size) */
	fpos_t	_offset;	/* current lseek offset (see WARNING) */
} FILE;
```

### Mixing standard and low level I/O
- Standard IO (e.g. `printf`, `scanf`, `fprintf`, `fscanf`), is internally buffered by default
- Using a `read/write` call after may read/write in unexpected order

### Basic File Stats with `stat`

| Command | C functon | Effect |
| --------------- | --------------- | --------------- |
| stat -L file | `int ret = stat(file, &statbuf)` | Get statistics on file |
| stat file | `int ret = lstat(file, &statbuf)` | Don't follow symlinks |
| | |
| | `FILE fd = open(file, ...)` | Same as above but |
| | `int ret = fd(file, &statbuf)` | On a open file descriptor |


### Measuring Time in Code
- Measure CPU time with the standard `clock()` function; measure time difference and convert to seconds
- Measure Wall (real) time with `gettimeofday()` or related functions; fill struct with info on time of day

> [!NOTE]
> Wall and CPU time can diverge with scheduling, if other programs have significant competition with your program, Wall can be a lot longer than CPU time

# Lecture 20 - 4/16/26

### Core Idea
Study the following code
```c
int *data = NULL;

int sum_simple(int len, int stride) {
  int sum = 0;
  for(int i = 0; i < len; i += stride) {
    sum += data[i];
  }
  return sum;
}
```

We would expect the throughput (additions / s) to stay the same as stride increases right?
It doesn't. 

As it increases, throughput actually decreases, due to cache misses!

```
Throughout with stride: 1 is 931,721,395.176098 additions/s
Throughout with stride: 8 is 870,210,119.557042 additions/s
Throughout with stride: 16 is 727,388,510.730544 additions/s
Throughout with stride: 24 is 405,114,636.531566 additions/s
Throughout with stride: 32 is 158,212,934.497652 additions/s
Throughout with stride: 64 is 135,012,682.675594 additions/s
Throughout with stride: 96 is 96,563,939.634631 additions/s
```

This is with data being a 1GB array of random ints, run on a m1 macbook pro
Run without optimizations, they kinda mess this up (too fast).

Also, this is a device with SOC ram, use a laptop with separate ram and the difference will be more obvious.

### CPU vs Memory Speed
- Early Computing Systems had CPU Chip and Memory Chips, little if any data storage in the CPU
- CPU and Memoy Chips ran at similar speeds / clock freqs., CPU would fetch from mem, do op, and write to mem
- CPU chip speed was a lot easier to speed up compared to memory
- Registers and Cache were developed in response to the growing speed difference between CPU and Memory
- Registers, LN cache


### Cache Favors Temporal and Spatial Locality
- Hardware people realized that programmers often access memory sequentially e.g.

```c
int sum = 0;
for(int i = 0; i < n; i++) {
    sum = data[i];
}
```

**Temporal Locality**
Memory used recently is likely to be used again (i, sum)

**Spatial Locality**
nearby addresses to recently used memory likely to be used (data\[i\], data\[i+1\])

- Using cache well speeds up code A LOT
- This is the reason why vectors/arrays are so much faster than linked lists

### The Memory Pyramid?

CPU - Extremely small (one thing)
Registers - small (16, 8 byte registers) 256b
CPU Cache (L1 4-16kb per core, L2 64-128 per core, L3 4-64mb per cpu)
RAM (4gb-1024gb)
SSD (128gb to 16tb)
HDD (128gb to 16tb)
Network (infinite)

As you go up, less memory but more speed

### Numbers Everyone Should Know
- "Main Memory" is composed of many different physical devices that work together and have differing sizes/speeds
- Accessing memory at #4096 may involve some or all of
    - CPU Cache
    - Ram
    - SSD/HDD

### Why isn't everything cache?
Cache is big, cache is REALLY expensive.
But this is kinda happening, look at AMD X3D chips and INTEL bLLC

# Lecture 21 - 4/21/26

### Direct Mapped Cache
- CPU Caches are normally LRU (but with some special coloring/restrictions)
- Regions of memory map to one cache lane

## 2D Arrays - Matrices
- Several way to construct matrices in C
- All must **embed** a 2D construct into 1-dimensional memory

### Methods

Repeat mallocs
```c
int **matr = malloc(rows * sizeof(int*));

for(int i = 0; i < rows; i++) {
    matr[i] = malloc(cols * sizeof(int));
}

process(matr, rows, cols);

for(int i = 0; i < rows; i++) {
    free(matr[i]);
}

free(matr);
```

2 mallocs
```c
int **matr = malloc(rows * sizeof(int*));
int *data = malloc(rows * cols * sizeof(int));

for(int i = 0; i < rows; i++) {
    matr[i] = data + i * cols;
}

process(matr, rows, cols);

free(data);
free(matr);
```

2 mallocs strategy is more cache-efficient

### Single Malloc Matrices
- Use a 1D array as a 2D matrix
- Usually need to do math to get index \[i\]\[j\] -> \[i * cols + j\]
- Can use macros for this

### Row Major vs Column Major
- Many languages use **Row-Major** order for 2D arrays/lists
     - C, Java, Python, Ocaml
     - mat\[i\] is a row, mat\[i\]\[j\] is the ith row, jth col
- Numerically-oriented languages use **Column-Major** order
     - Fortran, Matlab, R, Ocaml
     - mat\[j\] is a col
- Beware of convention to save performance
    - A sum on a appreciably large matrix is 6x faster with the correct order, due to cache

## Flavors of Permanent Storage
- Many types of fast memory, but they're **small** and *ephemeral* - all bits become 0 when powered off
- At the bottom of the pyramid are slow and **large** memory
- These are persistent

### HDD - Hard Disk Drive
- Store bits "permanently" as magnetized areas on special platters
- Magnetic disks: moving parts -> slow
- Cheap
- Infinitely readable/writeable

### SSD - Solid State Drive
- Faster, Lighter, Denser
- No moving parts, store electrons instead
- Writes degrade drive, only rewritable a few thousand times
- There is a write-balancing chip on the drive, won't write the same cells everytime
- More expensive
- Seq faster than random, but only a little
- Faster reads then writes
- Better SSDS will have a DRAM cache

> [!NOTE]
> This is a very good video about SSDs: [https://youtu.be/JwYttFnXRps]

### LTO - Tape Drives
- Slow: store bits as magnetic field on a tape/cassette
- Extremely cheap for GB, can store a LOT of data
- Only writable once

## The I/O System Connects CPU and Peripherals
- Motherboard connects everything together
- Connects CPU to ram, storage, and everything else (sata, pci(e), usb)

# Lecture 22 - 4/23/26

### Conflicts in Memory
- Two programs running could both reference the same absolute address
- This creates a conflict between these programs

#### Solutions
- Run programs one at a time
- Compiler compiles programs such that they don't use the same memory addresses
- Give each program an offset, so they refer to different blocks of memory
    - This evolves into virtual memory, lie about memory locations
    - This is the current solution

### Paged Memory
- Physical devices divide memory into chunks called **pages**
- Common page size supported by many OS's (Linux) and hardware is 4KB = 4096 bytes, can be larger with OS config
    - Fun fact, Asahi Linux/M series macs use 16KB pages, which make a lot of software break
    - Android is moving to 16KB pages sizes too
- CPU models use some # of bits for **Virtual Addresses**
    - This is because we don't need all 2^64 addresses right now for our memory, that's 18446744100GB

### Translation happens at the Page Level
- Within a page, addresses are sequential
- Between pages, may be non-sequential

### Address Translation Hardware
- Translation must be **fast** so involves special hardware
- **MMU (Memory Management Unit)** is a hardware element specifically designed for address translations
- Usually contains a special cache, **TLB (Translation Lookaside Buffer)** which stores recently translated addresses, speeds up repeated translation the same address
- Kernel interacts with MMU
- Provides location of the **Page Table** data structure, which relates Virtual/Physical Addresses
- Page Fault: MMU couldn't find a Virtual address in Main Memory (DRAM), runs a Kernel routine to handle the fault

### Trade-offs of Address Translation
**Pros**
- Avoids memory conflicts
- Programs can be compiled individually and assume they can access all memory
- OS can make decisions about DRAM use and set policies for security

**Cons**
- Not O(1), has an impact on algorithms
- Requires special hardware to be fast (MMU/TLB)
- Not need for single program machines

Wins greatly outweigh losses on most systems

#### Other advantages
1. Swap space: Use disk for increased memory, (Trashing is when swap to disk happens to often, big slowdown)
2. Security: Allow OS to check address validity
3. Debugging: Valgrind
4. Sharing Data: Processes can share pages with each other
5. Sharing libraries (like libc)
6. Convenient I/O: Map files into "memory"

### pmap: show virtual address space
- Use: `pmap <pid>`
- Shows: page size, permissions, and logical (physical) location

### Memory protection
- Label pages read, write, and or execute
- Segfault if you do something not allowed (e.g. write to libc)

### Virtual Memory and `mmap()`
- Normally programs interact indirectly with Virtual Memory
- `mmap()` and  `mummap()` interact with Virtual Memory directly
    - `mmap()` creates entry in page table
    - `mummap()` removes entry
    - Can map arbitrary or specific addresses
- `mmap()` is used to set up memory regions when programs are loaded
- can also be used to interact with virtual memory
- We will use `mmap()` to
    1. Implement `malloc()` and `free()`
    2. Create data structures independent of the heap
    3. Interact with memory mapped files

# Lecture 23 - 4/30/26

> [!IMPORTANT]
> FINAL EXAM will have (a) PMAP question(s), get used to the syntax

### Kernel Structures for `mmap()`'d files

File descriptor is an index into a FD table

This table has info about data location (inode) and
cached pages (areas that the os read into memory)

`mmap()`ing caches the entire file

## Threads

### Processes vs Threads
- From the Operating System Perspective, Processes and Threads are "scheduleable" entities that want time on the CPU
- Differences come down to **sharing** and what resources are shared by default
- When cooperating on shared data, both Processes and Threads should use coordination mechanisms to ensure computations are correct


| Processes   | Threads |
|--------------- | --------------- |
| No memory shared by default | **Memory shared** by default |
| Child changes do not affect parent | Data changes by one thread visible to all |
| Limited communication by default  | Easy sharing, rich communication possible |
| Limited damage between processes | Unlimited damage between threads |
| Memory protection between procs | No memory protection between threads |
| (Marginally) Longer startup| Faster startup |
| `fork()` / `waitpid()`   | `pthread_create()` / `pthread_join()`  |

- Threads are good for lighter tasks
- Processes are good for things which need better isolation, like untrusted execution like in a Chrome tabs (v8)

### Every Thread has its own stack
- Threads share heap, globals, and text regions of memory
- Threads have their own stack space

### Motivation for Threads
- Like use of `fork()`, threads increase program complexity
- **Improving execution efficiency** is a primary motivator
- Assign independent tasks in program to different threads
- 2 common ways this can speed up program runs

#### Parallel Execution with Threads
- Each thread/task computes part of an answer and then results are combined to form the total solution
- Discuss in Lecture (Pi Calculation)
- REQUIRES multiple CPUs to improve on Single thread *Why?*

#### Hide Latency of Slow Tasks via Threads
- Slow tasks block a thread but Fast tasks can proceed independently allowing programs to stay busy while running
- Textbook coverage (I/O latency reduction)
- Does NOT require multiple CPUs to get benefit, *Why?*

> [!NOTE]
> hiding latency is beyond the scope of this class

### Thread Creation

```c
#include <pthread.h>
pthread_create(pthread_t *thread, const pthread_attr_t *arr, void *(*start_routine)(void *), void *arg);
```

Example

```c standalone
#include <pthread.h>
#include <stdio.h>

void *doit(void *param) {
    int *p = (int*) param;
    *p *= 2;
    return (void *)p;
}

int main() {
    pthread_t thread_1;
    pthread_create(thread_1, NULL, doit, (void *) 42);

    int xrem; // return value
    pthread_join(thread_t, (void *) &xrem);

    // COMPLETE LATER
}
```

### Model problem: A slice of Pi
- Use the Monte Carlo algorithm
- Randomly generate (x, y) in Unit-Square
- if x^2 + y^2 <=1, inside square, so **hit**
- After a very large number of tries
- `pi = 4 * total hits / total points`;
# Lecture 24 - 5/5/26

Monte Carlo PI estimation

In this example, each thread runs the simulation, which parallelizes the number of dart throws/random points.

You can't modify variables from multiple threads at once, use a MUTEX.

### Why is our calculation so bad?
- The instruction `total_hits++` is **not atomic**
- Takes several CPU steps to complete
```asm
movq %RIP(total_hits), %rax
addq $1, %rax
movq %rax, %RIP(total_hits)
```
- But if multiple threads do this at once, these instructions can interleave
- This results in multiple increments not increasing the value multiple times

Ex)
```asm
movq %RIP(total_hits), %rax
// THREAD 1 is here, with value 17
```

```asm
movq %RIP(total_hits), %rax
addq $1, %rax
// THREAD 2 is here, with value 18
movq %rax, %RIP(total_hits)
```

One instruction later, thread_2 will write 18
Then another instruction later, thread_1 will write 18

Two increments resulted in total_hits only being one larger

### Critical Regions and Mutex locks
- Access to shared data must be coordinated among threads
- A **mutex** allows mutual exclusion
- Locking a mutex is an **atomic operation**
    - Locking is a system call to the OS
    - Guaranteed by the OS to complete all at once
    - No interleaving can happen
- Threads will **block** until granted a mutex by the OS
- Related to synchronize in Java

### Avoid Mutex Contention for Speed
- Locking/Unlocking mutexes is a **system call**, and the OS takes time to coordinate threads
- Avoiding repeated locks/unlocks saves time
- Often, this requires storing local data on each thread, and synchronizing that with the global later

### Thread Safety
- To have thread safety: use local data or share carefully
- Historically: many functions were not multi-threaded safe
- Nowadays: most things are

# Lecture 25 - 5/7/26

### Mutexes vs Semaphores

**Similarities**
- Both can be used to protect critical parts of code from other processes and threads
- Both use non-busy waiting, other things can be scheduled while waiting
- Both can be process private or shared between processes
    - Mutex: shared memory
    - Private semaphore with: pshared == 0

**Differences**
- Semaphores loosely associated with process coordination
- Mutexes loosely associated with thread coordination
- Both can be used for either with setup
- Semaphores possess an arbitrary **natural number**
- Mutexes are either locked/unlocked
- Mutexes have: `pthread_spinlock_t`

## What now?

That's every single note I've written for CMSC216. You can also look at some exam prep material in the exams folder, however, these are only marginally applicable/useful.
