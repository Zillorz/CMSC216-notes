// every one of these uses a different testing framework lmao

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

int fork_so_many_times();
int redirect_files();
int unix_read_file();
int measure_time();
int row_vs_col_major();

char *exit_msg[] = {
  "An exit msg (not touched as 0 is not a failure)",
  "fork didn't write to file 2^fork_count times",
  "stdin and or stdout redirection didn't work",
  "Writing/opening/reading file error",
  "Unix 'read' calls did not read file correctly",
  "CPU time was too short",
  "Wall time was too short",
  "Wall time was less than cpu time?",
  "Row major was slower than Col major"
};

int main() {
  #define NUM_TESTS 5
  char *names[NUM_TESTS] = { "Fork so many times", "Redirecting std*", "Unix file read", "Timing functions", "Big strides suck: Row vs Column major - cache" };
  int (*tasks[NUM_TESTS])() = { fork_so_many_times, redirect_files, unix_read_file, measure_time, row_vs_col_major };

  pid_t pids[NUM_TESTS];

  // necessary to flush
  printf("Running tests in parallel:\n");
  // start in parallel
  for(int i = 0; i < NUM_TESTS; i++) {
    pid_t p = fork();

    if (p == 0) {
      return tasks[i]();
    }

    pids[i] = p;
  }

  for(int i = 0; i < NUM_TESTS; i++) {
    int status;
    waitpid(pids[i], &status, 0);

    if (WIFEXITED(status)) {
      int code = WEXITSTATUS(status);

      if (code != 0) {
        printf("TEST %d failed with status %d: %s\n", i+1, code, exit_msg[code]);
      } else {
        printf("TEST %d passed\n", i+1);
        // exit
      }
    } else {
      printf("TEST %d failed suddenly\n", i+1);
    }
  }
}

int fork_so_many_times() {
  pid_t parent = getpid();

  int fd = open("a.txt", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

  fork();
  fork();
  fork();

  // this code runs 8 times
  // to prove it, let's run write to a file 
  int r = write(fd, "a", 1); // write one 'a' to the file
                             
  // just assume it writes properly

  if (getpid() != parent) {
    return 0;
  }

  // wait for all the children/forks to finish
  wait(NULL);
  close(fd);

  
  // check that we have 8 a's in our file
  // reading is annoying so use libc api
  FILE *f = fopen("a.txt", "r");
  char buf[16];
  fscanf(f, "%s", buf);
  fclose(f);

  if (strcmp(buf, "aaaaaaaa") != 0) {
    return 1;
  }
  
  return 0;
}

int redirect_files() {
  int stdout_save = dup(STDOUT_FILENO);
  int stdin_save = dup(STDIN_FILENO);

  int ofd = open("b.txt", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
  if (ofd == -1) { return 3; }

  // make stdout point to ofd
  dup2(ofd, STDOUT_FILENO);

  printf("ABABBABCHDCBDBE\n");
  printf("This text is going to the file b.txt, not the terminal!\n");

  // restore stdout
  dup2(stdout_save, STDOUT_FILENO);
  close(ofd);

  // Now, lets make stdin come from a file
  int ifd = open("b.txt", O_RDONLY);
  if (ifd == -1) { return 3; }

  dup2(ifd, STDIN_FILENO);

  char checkphrase[128];
  scanf("%s", checkphrase);

  // restore stdin
  dup2(stdin_save, STDIN_FILENO);
  close(ifd);

  if (strcmp(checkphrase, "ABABBABCHDCBDBE") != 0) {
    return 2;
  }

  return 0;
}

// read an entire file using unix, not trivial
int unix_read_file() {
  FILE* out = fopen("c.txt", "w");

  char *poem = "A file so long\n"
    "Text so plentiful\n"
    "Pain through song\n"
    "A rest so bountiful\n"
    "Work a bog\n"
    "But failing is so painful\n";

  fwrite(poem, sizeof(char), strlen(poem), out);
  fclose(out);

  // use stat to figure out file size
  struct stat poem_file_stat;
  stat("c.txt", &poem_file_stat);

  char* output_buf = malloc(sizeof(char) * (poem_file_stat.st_size + 1));
  int poem_fd = open("c.txt", O_RDONLY);

  int offset = 0;
  // read can stop at any number of bytes read, so it requires the usage of a loop to read everything
  while(1) {
    int nread = read(poem_fd, output_buf+offset, poem_file_stat.st_size - offset);

    if (nread == 0) {
      break;
    } else if (nread == -1) {
      return 3; 
    }
  }

  // you need to null terminate yourself
  output_buf[poem_file_stat.st_size] = '\0';

  if (strcmp(output_buf, poem) != 0) {
    return 4;
  }

  return 0;
}

int measure_time() {
  clock_t begin = clock();
  struct timeval wbegin;
  gettimeofday(&wbegin, NULL);

  // let the cpu rest
  sleep(1);

  // do work for 1 second (spin-sleep)
  clock_t temp = clock();
  while (clock() - temp < CLOCKS_PER_SEC) {}

  clock_t end = clock();
  struct timeval wend;
  gettimeofday(&wend, NULL);

  // cpu time should be over half a second
  double cpu_dif = (double)(end - begin) / CLOCKS_PER_SEC;
  if (cpu_dif < 1) {
    return 5;
  }

  double wall_dif = (wend.tv_sec - wbegin.tv_sec) + 
    (double)(wend.tv_usec - wbegin.tv_usec) / 1000000;

  if (wall_dif < 2) {
    return 6;
  }

  // other programs were scheduled during sleep call
  // so cpu time is half of wall time
  if (wall_dif < cpu_dif) {
    return 7;
  }

  return 0;
}

struct matrix {
  int *backing;
  int rows;
  int cols;
};

int row_vs_col_major() {
  // make a really big matrix
  int rows = 20000;
  int cols = 12000;
  int *big_array = malloc(rows * cols * sizeof(int));

  struct matrix matr = {
    .backing = big_array,
    .rows = rows,
    .cols = cols
  };

  #define MGET(matr, x, y) matr.backing[x * matr.cols + y]
  #define MSET(matr, x, y, val) matr.backing[x * matr.cols + y] = val

  // fill this matrix
  // using random numbers is really slow here so lets js use multiplication
  for(int i = 0; i < matr.cols; i++) {
    for(int j = 0; j < matr.rows; j++) {
      MSET(matr, i, j, i * j);
    }
  }

  clock_t start, end;

  start = clock();
  long long sum = 0;
  for(int i = 0; i < matr.rows; i++) {
    for(int j = 0; j < matr.cols; j++) {
      sum += MGET(matr, i, j);
    }
  }
  end = clock();

  double row_major_time = (double)(end - start) / CLOCKS_PER_SEC;

  start = clock();
  sum = 0;

  for(int j = 0; j < matr.cols; j++) {
    for(int i = 0; i < matr.rows; i++) {
      sum += MGET(matr, i, j);
    }
  }

  end = clock();

  double col_major_time = (double)(end - start) / CLOCKS_PER_SEC;

  if (row_major_time > col_major_time) {
    return 8;
  }

  return 0;
}
