// This is a helper file
// content.asm is where the notes 
// and code you want are

void over_under_flow();
void shifts_are_mult();
void high_bit_behavior();
void function_caller();
void ptrs_on_stack();
void globals();

struct Pack {
    int ab;
    short c;
    char d;
    char e;
};

void a_packed_struct(struct Pack p);

#include <stdio.h>



int main() {
  over_under_flow();
  shifts_are_mult();
  high_bit_behavior();
  function_caller();
  ptrs_on_stack();
  globals();

  struct Pack t = { .ab = 17, .c = 34, .d = 'A', .e = 'Z' };
  a_packed_struct(t);

  return 0;
}

// treating longs as int, bad idea? (should be fine as movl clears top 32bits)
char* get_reason(int i);

void assert_failure(int test_n) {
  printf("FAILURE on test %d\n", test_n);
  printf("Message: %s\n", get_reason(test_n));
}
