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

- Stack grows towards the heap, if they collide, we get a Stack Overflow error. [^1]
- Global and Text regions are usually fixed in size.
- "Logical Regions" for humans to organize their programs; no physical differences for regions[^2]

[^1]: Code block below demonstrates stack overflow (code: -1073741571), on GitHub might be somewhere else
```c
int expand_stack() {
    expand_stack();
}

expand_stack();
```

[^2]: Technically memory can have different physical locations (CPU cache vs RAM) but these are complicated

Common misconception: `sizeof(thing)`

- `sizeof(thing)` returns a **Compile Time Size** of thing from C source
- Useful when `malloc()`ing stuff like `int *arr = malloc(count * sizeof(int));`
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
    - Reading/writing free'd memory
    - Reading/writing before/after malloc block
    - Memory leaks
- Source line where problem arose (but not its cause)
- Super easy to use: `valgrind ./program`
- Slows execution of program down
