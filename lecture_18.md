# Lecture 18 - 4/9/26

### The Process Table
- OS maintains data on all processes in a Process Table 
- Process Table Entry = Process Control Block
- Contains info like PID, instruction that process is executing, Virtual Memory Address Space and **Files in Use**

### File Descriptors
- Each Process Table entry contains a table of open files
- User Programs refers to these via **File Descriptors (FD)**
- File Descriptor is an integer index into Kernel's table
```c
int fd = open("some_file.txt", O_RDONLY)`
```
- FD Table entry refers to other Kernel/OS data structures

- Unix presents many things via files/file descriptors
- Many Unix system actions are handled via `read()` or `write()`
- FDs allow interaction with "normal" files like `myfile.txt` or `commando.c` to read/change them
- FDs also allow interaction with many other things
    - Pipes for interprocess communication
    - Sockets for network communication
    - Special files to manipulate terminal, audio, graphics, etc.
    - Raw block of Memory Shared between Processes
    - Even processes themselves have special files in the file system: `ProcFs` in /proc/PID# provides info on running processes
- We will focus on standard File I/O using FDs now and touch on some broader uses Later
- Also must discuss FD interactions with previous System Calls (open files when you fork?)

### Open and Close: File Descriptors for Files
`open()`/`close()` show common features of many system calls
- Returns -1 on errors
- Show errors using the `perror()` function
- Use of vertical pipe (|) to bitwise-OR several options

### read() from File Descriptors
- Read up to SIZE from an open file descriptor
- Bytes stored in buffer, overwrite it
- Return value is number to bytes read, -1 for error
- SIZE commonly defined but can be variable, constant, etc.

> [!CAUTION]
> Bad things happen in buffer is actually smaller than SIZE
> `read()` does NOT null terminate, add '\0' manually

### Behavior of read()
- OS maintains **file positions** for each open File Descriptor
- I/O functions like read() use/change position in a file
- `read()` overwrites input array
- OS **does not** update positions in user arrays
- `read()` returns # bytes read, or 0 when at EOF

> [!WARNING]
> `read()` DOES not need to return the number of bytes you asked for, even if that number of bytes IS available
