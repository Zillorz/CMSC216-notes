# Lecture 25 - 5/7/26

### Mutexes vs Semaphores

**Similarities**
- Both can be used to protect critical parts of code from other processes and threads
- Both use non-busy waiting, other things can be scheduled while waiting
- Both can be process private or shared between processes
    - Mutex: shared memory
    - Private semaphore with: pshared == 0

**Differences**
- Semaphores loosely associated with process coordination
- Mutexes loosely associated with thread coordination
- Both can be used for either with setup
- Semaphores possess an arbitrary **natural number**
- Mutexes are either locked/unlocked
- Mutexes have: `pthread_spinlock_t`
