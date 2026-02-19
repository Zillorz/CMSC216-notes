/*
 * PRELUDE:
 * Normally, notes are delivered in the .md format, as they are
 * primarily suppose to be English, with bits of code intended to be run with
 * note-runner in-between.
 *
 * These files are instead .c files, as they are primarily meant to be code
 * with notes on the side to understand execution
 *
 * TABLE OF CONTENTS:
 *
 * content.c - Complicated content with a very non-robust testing framework
 * valgrind.c - Common memory errors with valgrind execution logs alongside
 *
 * */

// Include headers are not expected to be necessary on the exam, however it's
// still good to know about them.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <stdint.h> is useful, but we're allowed to assume standard sizes

// We didn't include a header for easily compilation and single-file support
// but the following lines are what you would see in a header file
//
// BEGIN HEADER

// Show that pointers and arrays are interchangeable
void pointers_and_arrays();

// Show properties of void pointers
void void_pointers_can_be_anything();

// C doesn't bound check arrays
void no_bounds_checking();

// Pointer arithmetic and it's consequences
void pointer_arithmetic();

// Demonstration of C strings and common methods
void c_strings_are_char_arrays();

// Demonstration of memory allocation
void malloc_and_free_memory();

// Proof that size_of is only for compile time
void size_of_sometimes_works();

// This is how you define a struct
typedef struct {
  char name[32];
  int age;
  double height;
} simple_t;

// Simple use of structs
void simple_structs();

// Demonstration of dot syntax vs arrow syntax
void ptr_to_a_struct();

typedef struct {
  char *data;
} string_t;

// Copy semantics of structs
void copying_strings_is_complicated();

// Simple file ops
void managing_files();

// Binary files exist too
void files_in_binary();

// More advanced file ops
void print_file();

// END HEADER

/*
 * A word about the main function.
 *
 * The return value, is indicative of success or failure. A return value of 0
 * means a successful execution, and every other value is a error.
 *
 * The first parameter argv, is the number of items in the pointer argc.
 *
 * argc is an array of the string arguments to the program
 * argc[0] will ALWAYS be the name of the program, and will always exist
 * argc[1] is the first user argument
 * argc[?] is null terminated
 * the last arg is argc[argv - 1]
 * */
int main(int argv, char **argc) {
  // the following code is a bit beyond the exam 1 spec, but makes this
  // soooo much nicer

#define NUM_TESTS 13

  char names[NUM_TESTS][64] = {"Pointers are arrays",
                               "Void pointers can be anything",
                               "No bounds checking",
                               "Pointer arithmetic",
                               "C strings are character arrays",
                               "Allocation with malloc/free",
                               "size_of works sometimes",
                               "Simple struct behavior",
                               "Pointers to a struct & ->",
                               "Copying strings in a struct",
                               "File operations 1",
                               "File operations 2",
                               "File operations 3"};

  void (*tasks[NUM_TESTS])() = {pointers_and_arrays,
                                void_pointers_can_be_anything,
                                no_bounds_checking,
                                pointer_arithmetic,
                                c_strings_are_char_arrays,
                                malloc_and_free_memory,
                                size_of_sometimes_works,
                                simple_structs,
                                ptr_to_a_struct,
                                copying_strings_is_complicated,
                                managing_files,
                                files_in_binary,
                                print_file};

  for (int i = 0; i < NUM_TESTS; i++) {
    tasks[i]();
    printf("TEST '%s' passed [%d/%d]\n", names[i], i + 1, NUM_TESTS);
  }

  return 0;
}

// A lazy assert statement, writes to stderr if statement is non-truthy
void assert(char *context, int statement) {
  if (!statement) {
    fprintf(stderr, "[FAILURE] message: %s\n", context);
    exit(1);
  }
}

void pointers_are_arrays(int *inp) {
  inp[3] = 12;
  *inp = 14;
}

void pointers_and_arrays() {
  int arr[4] = {1, 2, 3, 4};
  pointers_are_arrays(arr);

  assert("An array is a pointer", arr[0] == 14 && arr[3] == 12);
}

void void_pointers_can_be_anything() {
  int a = 2;
  char c = 'b';
  double g = 7.42;

  void *ptr = &a;

  assert("Void can be a int", *(int *)(ptr) == 2);

  ptr = &c;
  assert("Void can be a char", *(char *)(ptr) == 'b');

  ptr = &g;
  assert("Void can be a double", *(double *)(ptr) == 7.42);

  g = 8;
  /*
   * g is now 8 as a double
   * and the proper *(double *)(ptr) would return 8.0
   * but ints and doubles have different representation
   * so don't expect 8 with the wrong type!
   */
  assert("Make sure your void is the right thing", *(int *)(ptr) != 8);
}

// This test is technically undefined behavior, but it's defined enough for us
void no_bounds_checking() {
  int arr[4] = {1, 2, 3, 4};

  // clangd will actually give us a warning here
  arr[5] = 17;
  assert("This isn't in bounds, but c doesn't care", arr[5] == 17);
}

void pointer_arithmetic() {
  int arr[5] = {1, 2, 3, 4, 5};
  int *ptr = arr;

  assert("Pointer points to first element of array", arr[0] == *ptr);
  assert("Array indexing is just pointer arithmetic", arr[4] = *(ptr + 4));
}

void c_strings_are_char_arrays() {
  char *string = "Hello world!";
  char copy[30];

  // strcpy is DST, SRC
  strcpy(copy, string);

  assert("C Strings don't have lengths associated with them, use strlen",
         strlen(string) == 12);

  // C strings are terminated with \0
  assert("C Strings are terminated with '\\0'", string[12] == '\0');

  int the_same = strcmp(string, copy) == 0;
  assert("String equality is with strcmp", the_same);
}

void malloc_and_free_memory() {
  int size = 5 * 5 - 12;
  double *dynamic_array = malloc(sizeof(double) * size);

  for (int i = 0; i < size; i++) {
    dynamic_array[i] = 1.2;
  }

  dynamic_array[5] = 5.234;

  // Once we're done with our malloc, we must use free
  free(dynamic_array);

  // DO NOT access a ptr after it's free'd
}

// This test will FAIL on non-standard machines.
// Only run this on 64 bit machines!
void size_of_sometimes_works() {
  assert("char is 1 byte", sizeof(char) == 1);
  assert("int is 4 bytes", sizeof(int) == 4);
  assert("pointer is 8 bytes", sizeof(int *) == 8);

  int arr_int[4] = {7, 6, 5, 4};
  char *str = "Hi guys!";
  double *arr_dynamic = malloc(sizeof(double) * 12);

  assert("Shows size of static/compile time array",
         sizeof(arr_int) == 4 * sizeof(int));
  // clangd is smart enough to warn us here!
  assert("Doesn't get length of string", sizeof(str) == 8);
  assert("Doesn't get size of dynamic array", sizeof(arr_dynamic) == 8);

  free(arr_dynamic);
}

void simple_structs() {
  // Initialize one of two ways
  simple_t first = {"A name", 18, 5.5};
  simple_t second = {.name = "James Farmer", .age = 20, .height = 6.0};

  simple_t third = {};
  assert("Fields not specified are zeroed", third.height == 0);

  assert("Access with names.field", second.age == 20);

  first.age = 12;
  strcpy(first.name, "Name 2");
  assert("Can modify fields!",
         first.age == 12 && strcmp(first.name, "Name 2") == 0);
}

void ptr_to_a_struct() {
  simple_t second = {
      .name = "Liam Jandy", .age = 44, .height = 5.0 + 9.0 / 12.0};

  simple_t *ptr = &second;

  assert("Use -> syntax on a ptr to a struct", ptr->age == 44);
  assert("Use . syntax on a struct", second.height > 5.0);
}

void copying_strings_is_complicated() {
  simple_t first = {.name = "James Farmer", .age = 20, .height = 6.0};

  simple_t second = first;

  first.age = 21;
  assert("Simple fields are independent", first.age != second.age);

  first.name[0] = 'T';
  assert("Arrays are copied too!", strcmp(second.name, "James Farmer") == 0);

  string_t string_og = {};
  string_og.data = malloc(sizeof(char) * 16);
  strcpy(string_og.data, "Hello world!");

  string_t evil_copy = string_og;
  string_og.data[0] = 'W';

  assert("These are linked, as ptrs are copied differently!",
         strcmp(evil_copy.data, "Wello world!") == 0);

  // use malloc and strcpy!
  evil_copy.data = malloc(sizeof(char) * 16);
  strcpy(evil_copy.data, string_og.data);

  string_og.data[0] = 'J';
  assert("Now they're independent",
         strcmp(evil_copy.data, "Wello world!") == 0);
}

void managing_files() {
  // Use fopen to open a file
  // specify the mode
  // "r" - read, "w" - write, "a" append
  FILE *fin = fopen("test.txt", "w");

  int orange_count = 5;
  // write to file
  fprintf(fin, "Orange\n");
  fprintf(fin, "%d", orange_count);

  // close files when you're done with them
  fclose(fin);

  // Let's read that file now
  FILE *test_file = fopen("test.txt", "r");

  char out[16];
  fscanf(test_file, "%s", out);

  int count;
  fscanf(test_file, "%d", &count);

  assert("Reading from file works.", strcmp(out, "Orange") == 0);
  assert("Reading int from file works.", count == orange_count);

  fclose(test_file);
}

void files_in_binary() {
  simple_t s = {.name = "James", .age = 32, .height = 5.0 + 10.0 / 12.0};

  FILE *f = fopen("test.bin", "wb");
  fwrite(&s, sizeof(simple_t), 1, f);
  fclose(f);

  simple_t s2;

  FILE *f2 = fopen("test.bin", "rb");
  fread(&s2, sizeof(simple_t), 1, f2);
  fclose(f2);

  assert("Can read binary struct from file", s.age == s2.age);
}

void print_file() {
  char comp[200];

  FILE *f = fopen("test.txt", "w");
  fprintf(f, "Hello, world!\n");
  fprintf(f, "This is my file\n");
  fprintf(f, "Goodbye world!\n");
  fclose(f);

  sprintf(comp, "Hello, world!\n");
  sprintf(comp + strlen(comp), "This is my file\n");
  sprintf(comp + strlen(comp), "Goodbye world!\n");

  FILE *fin = fopen("test.txt", "r");

  int size = 0;
  while (fgetc(fin) != EOF) {
    size++;
  }

  char *file_contents = malloc(sizeof(char) * (size + 1));
  rewind(fin);

  for (int i = 0; i < size; i++) {
    file_contents[i] = fgetc(fin);
  }

  file_contents[size] = '\0';

  fclose(fin);

  assert("We read the entire file into file_contents, including whitespace",
         strcmp(file_contents, comp) == 0);
}
