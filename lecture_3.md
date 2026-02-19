# Lecture 3

### Traditional data types

| Bytes [^1]| Name | Range |
| ------------- | ------------- | -------------- |
|   | INTEGRAL |   |
| 1 | char | -128 to 127 |
| 2 | short | -32,768 to 32,767 |
| 4 | int | -2^31 to 2^31 - 1 |
| 8 | long | -2^63 to 2^63 - 1 |
| ------------- | ------------- | -------------- |
|   | FLOATING |   |
| 4 | float | +- 3.4e+38 |
| 8 | double | +- 1.8e+308 |
| ------------- | ------------- | -------------- |
|   | POINTER |   |
| 4/8 [^2]  | pointer  | Either 32/64 bits |
|   | array | type of pointer |

[^1]: These sizes are not standard, portable code cannot assume any given size
[^2]: We run 64 bit computers, so we will assume 8 bytes for every pointer

### More about pointers
- Pointers are written with a *, e.g. 
- `double *d` `int *p`, `char *s`, `void *p`
- An array is a pointer to a fixed location
- Array indexing is a pointer deference
- `array[12]` = `*(array + 12)`
- Strings are represented by `char*` or `char[]`, there isn't a dedicated string type 
- Same with booleans, usually represented by char/int wit 0 = false and 1 = true
- `void` normally means, nothing but a `void*` is just a pointer to a memory address that stores anything

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
// we set b to the deferenced value of ptr, after casting ptr to an int pointer
int b = *((int *) ptr); 
// b = 5

ptr = &x;
// we set y to the deferenced value of ptr, after casting ptr to a double pointer
double y = *((double *) ptr);
// y = 1.2345

// SAFETY: ptr points to a double, but we casted it to an int??
int c = *((int *) ptr);
printf("%d", c);
// c will have the same bits as x, but as doubles and ints are different formats, we have a completely gibberish number
```
'c' will have the same bits as 'x' (at least the first 32 bits of x), but the alternate formats and size. 'c' will be a completely gibberish number.

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

stdint.h defines int64, uint64 and other int types with garunteed sizes


## Arrays
- A continuous block of homogenous data
- Automatically allocated by compiler with a fixed size [^3]
- Uses \[ \] syntax
- Refer to element with arr\[3\]
- Bare name 'arr' is the memory address where array starts
- This actually means arrays are also pointers!
- And array indexing is pointer deferencing, `array[3]` == `*(array + 3)`

[^3]: This is refering to fixed size arrays, not heap allocated arrays made with malloc/calloc
