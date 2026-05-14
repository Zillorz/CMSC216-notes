# Lecture 24 - 5/5/26

Monte Carlo PI estimation

In this example, each thread runs the simulation, which parallelizes the number of dart throws/random points.

You can't modify variables from multiple threads at once, use a MUTEX.

### Why is our calculation so bad?
- The instruction `total_hits++` is **not atomic**
- Takes several CPU steps to complete
```asm
movq %RIP(total_hits), %rax
addq $1, %rax
movq %rax, %RIP(total_hits)
```
- But if multiple threads do this at once, these instructions can interleave
- This results in multiple increments not increasing the value multiple times

Ex)
```asm
movq %RIP(total_hits), %rax
// THREAD 1 is here, with value 17
```

```asm
movq %RIP(total_hits), %rax
addq $1, %rax
// THREAD 2 is here, with value 18
movq %rax, %RIP(total_hits)
```

One instruction later, thread_2 will write 18
Then another instruction later, thread_1 will write 18

Two increments resulted in total_hits only being one larger

### Critical Regions and Mutex locks
- Access to shared data must be coordinated among threads
- A **mutex** allows mutual exclusion
- Locking a mutex is an **atomic operation**
    - Locking is a system call to the OS
    - Guaranteed by the OS to complete all at once
    - No interleaving can happen
- Threads will **block** until granted a mutex by the OS
- Related to synchronize in Java

### Avoid Mutex Contention for Speed
- Locking/Unlocking mutexes is a **system call**, and the OS takes time to coordinate threads
- Avoiding repeated locks/unlocks saves time
- Often, this requires storing local data on each thread, and synchronizing that with the global later

### Thread Safety
- To have thread safety: use local data or share carefully
- Historically: many functions were not multi-threaded safe
- Nowadays: most things are
