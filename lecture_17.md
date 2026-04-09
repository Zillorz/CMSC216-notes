# Lecture 17 - 4/7/26

### Effects of fork()
- Duplicate process
- Only difference is return value of `fork()`
- returns 0 if child, otherwise returns id of child
- exponential, `fork(); fork(); fork();` leads to 8 copies of the program


### Coordination is hard
Coordination two programs is hard. The order of execution is random and unreliable. Ensure that your code doesn't depend on a specific order without a `wait()` call. Debugging is especially hard as the scheduler is hard to predict and control/influence.

### Child exit status
- Calls to `wait()` can set a status variable indictating the fate of a child
- Several **macros** are used to parse that
- zero = normal, non-zero = error

- You can `raise()` a signal

## Blocking vs. Nonblocking Activites
**Blocking**
- A call to `wait()` or `waitpid()` may cause calling process to **block**
- Blocking is associated with other activites as well
    - I/O, obtain a lock, get a signal, etc.
- Generally creates **synchronous** situations: waiting for something to finish means everything happens in order
**Non-blocking**
- Contrast with **non-blocking** (asynchronous) activites: calling process goes ahead even if something isn't finished yet
- `wait()` is blocking, but `waitpid()` can be blocking or non-blocking 
- idea behind async runtimes like tokio

### Non-Blocking waitpid()
- Use the `WNOHANG` option
- Returns immediately regardless of the child's status

### Zombie Processes
1. Parent creates a child
2. Child completes
3. Child becomes a **zombie**!
4. Parent waits for child
5. Child reaped

Zombie Process: A process that has finished, but has not been `wait()`'ed for by its parent yet so it cannot be eliminated from the system. OS can reclaim child resources like memory once parent `wait()`'s 

### Orphans are always Adopted
- Parent exits without `wait()`ing leaving them orphaned.
- Adopted by root process with PID=1 (systemd, launchd, etc...)

### Reapers and the Subreapers
- Process X creates many children, Oprhans them
- Children of X complete, becomes Zombies until...
- Newly assigned Parent `wait()`'s for them
- Adoptive parent like Process 1 sometimes referred to as a **Reaper** process: "reaps the dead processes"
- System may designate a *Subreaper* to do this per user so oprhans NOT re-parented to process ID 1
- Graphical Login on Ubuntu Linux systems usually designates a Subreaper for each user
