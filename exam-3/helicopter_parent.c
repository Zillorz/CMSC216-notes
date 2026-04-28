#include <locale.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

  int pid = fork();

  if (pid == 0) {
    char* args[3] = { "sleep", "1", NULL } ;
    execvp("sleep", args);
  } else {
    printf("My child is %d\n", pid);
    sleep(2000);

    int status = 0;
    int count = 0;
    int retcode = 0;

    while ((retcode = waitpid(pid, &status, WNOHANG)) == 0) {
      printf("Juniors taking so long!!! Is he among the 50%% of people who are below avg!\n");
      // do some other work
      // poll again
      count += 1;
    }

    setlocale(LC_NUMERIC, "");
    printf("I checked %'d times, exited with status: %d\n!", count, status);
  }
 

  return 0;
}
