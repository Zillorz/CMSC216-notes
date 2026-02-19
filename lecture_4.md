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
| Interchangable | Yes | Yes |
| Has length property | No | No |

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

    // interchangable
    print_arr(p);

    int a[] = { 5, 10, 15 };

    // interchangable
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


C strings end with the null terminator '\0', and have no length property.
Accessing the length with a function like `strlen` is actually `O(n)`

Another thing about this null terminator, not having it in a string can cause undefined behavior. 
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
- `strcpy(dst, src)` - copys string from src to dst
- `strcmp(s1, s2)` - compares string lexicographically, 0 if equal
