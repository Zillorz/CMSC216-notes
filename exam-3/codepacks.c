// collection of code that will be useful to copy on the exam
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/file.h>

// main function
int main(int argc, char** argv) {


  return 0;
}

// unix read
char* read_file(char *fname, int max_chunk_size) {
  struct stat v;
  if (stat(fname, &v) == -1) {
    perror("Failed to stat file");
    return NULL;
  }

  int fd = open(fname, O_RDONLY);
  if (fd == -1) {
    perror("Failed to open file");
    return NULL;
  }

  char* buf = malloc(v.st_size + 1);
  int offset = 0;

  while (v.st_size > offset) {
    int read_size = v.st_size-offset;
    if (read_size > max_chunk_size) { read_size = max_chunk_size; }

    int nread = read(fd, buf+offset, read_size);
    offset += nread;
  }

  close(fd);
  buf[v.st_size] = '\0';
  return buf;
}

// create file
void create_file(char *fname) {
  // requires 3 args, including S_IRUSR and S_IWUSR
  int fd = open(fname, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
  close(fd);
  return;
}

void redirect_pipes(char *outfile, char *infile) {
  int stdout_save = dup(STDOUT_FILENO);
  int stdin_save = dup(STDIN_FILENO);
    
  int ofd = open(outfile, O_WRONLY);
  int ifd = open(infile, O_RDONLY);

  dup2(ofd, STDOUT_FILENO);
  dup2(ifd, STDIN_FILENO);

  // redirected
  // to save pipes, run

  dup2(stdout_save, STDOUT_FILENO);
  dup2(stdin_save, STDIN_FILENO);
}

void traversals(int rows, int cols) {

  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < cols; j++) {
      // row major
    }
  }


  for(int j = 0; j < cols; j++) {
    for(int i = 0; i < rows; i++) {
      // col major
    }
  }
}
