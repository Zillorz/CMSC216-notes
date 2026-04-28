Fork is exponential, and choordinating child processes is hard.

File descriptors in UNIX I/O are ints

### dup() and dup2()
`dup()` takes in a file descriptor and returns a new one, this new file descriptor refers to the same object but has a new integer descriptor/handle

`dup2(SRC, DST)` 
- This closes the descriptor in DST (if needed)
- changes the handle/int DST to point to the same object as SRC
- need a dup to copy/restore
- confusing 'man' entry

```c
int log_file =  open("log.txt", O_CREAT | OR_WRONLY, S_IRUSR | S_IWUSR);
int stdout_save = dup(STDOUT_FILENO);
dup2(log_file, STDOUT_FILENO);

printf("[LOG] This text is in log.txt!\n")

// restore using dup2
dup2(stdout_save, STDOUT_FILENO);
printf("Hello terminal!\n");
```

The timestamps on linux are mtim stim and ctim, not (m/s/c)timespec like on some posix

Row Major > Col Major (caches)

Accessing caches is fastest sequentially, with low strides being better
