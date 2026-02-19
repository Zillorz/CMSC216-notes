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
- Structs alloated in memory are laid out compactly
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

