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
- if, else if, else, goto [^1]
- labels:
- Very basic, same as Java

[^1]: DO NOT USE GOTO, you need permission from Kauffman to not lose ALL style points with a goto :/

`switch()/case`: The **worst** control structure

Aside: I honestly disagree with this in any other programming language, but C has a really ugly error prone syntax. The Rust `match` statement is really nice and I think that having a exhaustive, break free, switch/match is superior to if/else!

Kauffman really, really hates switch statements. NEVER use them unless you're writing a emulator?

### A program is born
- Write some C code in `program.c`
- Compile it with a C compiler
    - `gcc -o program prog.c`
    - `clang -o program prog.c`
- Compilation is a multi-step process
    - Check syntax for correctness/errors
    - Perform optimizations on the code if possible
    - Translate result to **ASM** for specific target e.g. ARM64, AMD64
    - Assemble code into object code
    - Link required libraries
- Result: executable program
- Running a progrm requires a loader


**END OF EXAM 1 CONTENT**

### Binary encoding
- Unsigned integers are always positive (recall [lecture 3](/lecture_3.md)
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
    free(inp); // bad practice to free an arugment
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
- "%x": Unsigned hexadeciaml
- Octal is 0 through 7, base 8
- Hexadecimal is 0 to F (switches to letters after 9)

- A trick to convert these, go to binary, then to them!
    - Group in 4 (as 2^4 = 16) for hex
    - Group in 3 (as 2^3 = 8) for octal
