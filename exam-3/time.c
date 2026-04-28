#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

int *data = NULL;

int sum_simple(int len, int stride) {
  int sum = 0;
  for(int i = 0; i < len; i += stride) {
    sum += data[i];
  }
  return sum;
}

void time_sum(int n, int stride) {
  clock_t start = clock();
  sum_simple(n, stride);
  clock_t end = clock();

  double elapsed = (end - start) / (double)CLOCKS_PER_SEC;
  
  int additions = n / stride;
  double mbps = (double)additions * sizeof(int) / (1024 * 1024);

  // printf("Sum with stride: %'d took %'lf s\n", stride, elapsed);
  printf("Throughout with stride: %'d is %'lf additions/s\n", stride, additions / elapsed);
}

int main() {
  int n = 256 * 1024 * 1024;
  data = malloc(n * sizeof(int));

  srand(clock());
  for(int i = 0; i < n; i++) {
    data[i] = rand();
  }

  setlocale(LC_NUMERIC, "");

  int test[7] = { 1, 8, 16, 24, 32, 64, 96 };
  for(int i = 0; i < 7; i++) {
    time_sum(n, test[i]);
  }

  return 0;
}
