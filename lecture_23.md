# Lecture 23 - 4/30/26

> [!IMPORTANT]
> FINAL EXAM will have (a) PMAP question(s), get used to the syntax

### Kernel Structures for `mmap()`'d files

File descriptor is an index into a FD table

This table has info about data location (inode) and
cached pages (areas that the os read into memory)

`mmap()`ing caches the entire file

## Threads

### Processes vs Threads
- From the Operating System Perspective, Processes and Threads are "schedulable" entities that want time on the CPU
- Differences come down to **sharing** and what resources are shared by default
- When cooperating on shared data, both Processes and Threads should use coordination mechanisms to ensure computations are correct


| Processes   | Threads |
|--------------- | --------------- |
| No memory shared by default | **Memory shared** by default |
| Child changes do not affect parent | Data changes by one thread visible to all |
| Limited communication by default  | Easy sharing, rich communication possible |
| Limited damage between processes | Unlimited damage between threads |
| Memory protection between procs | No memory protection between threads |
| (Marginally) Longer startup| Faster startup |
| `fork()` / `waitpid()`   | `pthread_create()` / `pthread_join()`  |


- Threads are good for lighter tasks
- Processes are good for things which need better isolation, like untrusted execution like in a Chrome tabs (v8)

### Every Thread has its own stack
- Threads share heap, globals, and text regions of memory
- Threads have their own stack space

### Motivation for Threads
- Like use of `fork()`, threads increase program complexity
- **Improving execution efficiency** is a primary motivator
- Assign indepedent tasks in program to different threads
- 2 common ways this can speed up program runs

#### Parallel Execution with Threads
- Each thread/task computes part of an answer and then results are combined to form the total solution
- Discuss in Lecture (Pi Calculation)
- REQUIRES multiple CPUs to improve on Single thread *Why?*

#### Hide Latency of Slow Tasks via Threads
- Slow tasks block a thread but Fast tasks can proceed indepedently allowing programs to stay busy while running
- Textbook coverage (I/O latency reduction)
- Does NOT require multiple CPUs to get benefit, *Why?*

> [!NOTE]
> hiding latency is beyond the scope of this class

### Thread Creation

```c
#include <pthread.h>
pthread_create(pthread_t *thread, const pthread_attr_t *arr, void *(*start_routine)(void *), void *arg);
```

Example

```c standalone
#include <pthread.h>
#include <stdio.h>

void *doit(void *param) {
    int *p = (int*) param;
    *p *= 2;
    return (void *)p;
}

int main() {
    pthread_t thread_1;
    pthread_create(thread_1, NULL, doit, (void *) 42);

    int xrem; // return value
    pthread_join(thread_t, (void *) &xrem);

    // COMPLETE LATER
}
```

### Model problem: A slice of Pi
- Use the Monte Carlo algorithm
- Randomly generate (x, y) in Unit-Square
- if x^2 + y^2 <=1, inside square, so **hit**
- After a very large number of tries
- `pi = 4 * total hits / total points`;
