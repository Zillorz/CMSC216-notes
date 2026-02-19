#include <stdio.h>
#include <stdlib.h>

int *evil_stack_ret() {
  int i = 12;
  return &i;
}

int main() {
  /*
    Leak a malloc:

    HEAP SUMMARY:
        in use at exit: 16 bytes in 1 blocks
      total heap usage: 2 allocs, 1 frees, 20 bytes allocated

    LEAK SUMMARY:
       definitely lost: 0 bytes in 0 blocks
       indirectly lost: 0 bytes in 0 blocks
         possibly lost: 0 bytes in 0 blocks
       still reachable: 16 bytes in 1 blocks
            suppressed: 0 bytes in 0 blocks
  */
  int *t = malloc(sizeof(int) * 4);

  /*
    Use a malloc result before initialization:

    Conditional jump or move depends on uninitialised value(s)
        at 0x400768: main (valgrind.c:35)
    Uninitialised value was created by a heap allocation
        at 0x48855D0: malloc (vg_replace_malloc.c:447)
        by 0x400753: main (valgrind.c:24)
   * */
  if (t[2] == 23) {
    printf("Hello undefined behavior!");
  }

  int *q = malloc(sizeof(int));
  *q = 0;
  free(q);

  /*
    Using a free'd ptr:

    Invalid read of size 4
        at 0x400798: main (valgrind.c:55)
    Address 0x4a89090 is 0 bytes inside a block of size 4 free'd
        at 0x48887CC: free (vg_replace_malloc.c:990)
        by 0x400793: main (valgrind.c:41)
    Block was alloc'd at
        at 0x48855D0: malloc (vg_replace_malloc.c:447)
        by 0x40077F: main (valgrind.c:39)
  */
  if (*q != 0) {
    printf("How did we get here?");
  }

  char j;
  /*
    Accessing an uninitialized stack value:

    Conditional jump or move depends on uninitialised value(s)
        at 0x4007B8: main (valgrind.c:67)
    Uninitialised value was created by a stack allocation
        at 0x400744: main (valgrind.c:9)

    Note: all stack variables will have the line number of the definition of 
    main, not where they're declared (why its valgrind.c:9 instead of 59)
  */
  if (j == 'c') {
    printf("We didn't initalize this char!");
  }

  char *qj = NULL;

  /*
    Dereferencing a NULL ptr:

    Invalid read of size 1
        at 0x4007B8: main (valgrind.c:84)
    Address 0x0 is not stack'd, malloc'd or (recently) free'd
  */
  if (*qj == 'c') {
    printf("Dereferenced a null ptr!");
  }

  int *stack_ptr = evil_stack_ret();
  /*
    Accessing a stack ptr after it's block:
    
    Invalid read of size 4
        at 0x4007C0: main (valgrind.c:96)
    Address 0x0 is not stack'd, malloc'd or (recently) free'd 
  */
  if (*stack_ptr + 2 == 1) {
    printf("This stack ptr points to a popped location in the stack!");
  }

  return 0;
}

// WARNING
// If you run valgrind on this program, you won't see all the results that are
// here, that is because many of these cause a seg-fault and do not execute the
// remaining statements.
