# Lecture 22 - 4/23/26

### Conflicts in Memory
- Two programs running could both reference the same absolute address
- This creates a conflict between these programs

#### Solutions
- Run programs one at a time
- Compiler compiles programs such that they don't use the same memory addresses
- Give each program an offset, so they refer to different blocks of memory
    - This evolves into virtual memory, lie about memory locations
    - This is the current solution

### Paged Memory
- Physical devices divide memory into chunks called **pages**
- Common page size supported by many OS's (Linux) and hardware is 4KB = 4096 bytes, can be larger with OS config
    - Fun fact, Asahi Linux/M series macs use 16KB pages, which make a lot of software break
    - Android is moving to 16KB pages sizes too
- CPU models use some # of bits for **Virtual Addresses**
    - This is because we don't need all 2^64 addresses right now for our memory, that's 18446744100GB

### Translation happens at the Page Level
- Within a page, addresses are sequential
- Between pages, may be non-sequential

### Address Translation Hardware
- Translation must be **fast** so involves special hardware
- **MMU (Memory Management Unit)** is a hardware element specifically designed for address translations
- Usually contains a special cache, **TLB (Translation Lookaside Buffer)** which stores recently translated addresses, speeds up repeated translation the same address
- Kernel interacts with MMU
- Provides location of the **Page Table** data structure, which relates Virtual/Physical Addresses
- Page Fault: MMU couldn't find a Virtual address in Main Memory (DRAM), runs a Kernel routine to handle the fault

### Trade-offs of Address Translation
**Pros**
- Avoids memory conflicts
- Programs can be compiled individually and assume they can access all memory
- OS can make decisions about DRAM use and set policies for security

**Cons**
- Not O(1), has an impact on algorithms
- Requires special hardware to be fast (MMU/TLB)
- Not need for single program machines

Wins greatly outweigh losses on most systems

#### Other advantages
1. Swap space: Use disk for increased memory, (Trashing is when swap to disk happens to often, big slowdown)
2. Security: Allow OS to check address validity
3. Debugging: Valgrind
4. Sharing Data: Processes can share pages with each other
5. Sharing libraries (like libc)
6. Convenient I/O: Map files into "memory"

### pmap: show virtual address space
- Use: `pmap <pid>`
- Shows: page size, permissions, and logical (physical) location

### Memory protection
- Label pages read, write, and or execute
- Segfault if you do something not allowed (e.g. write to libc)

### Virtual Memory and `mmap()`
- Normally programs interact indirectly with Virtual Memory
- `mmap()` and  `mummap()` interact with Virtual Memory directly
    - `mmap()` creates entry in page table
    - `mummap()` removes entry
    - Can map arbitrary or specific addresses
- `mmap()` is used to set up memory regions when programs are loaded
- can also be used to interact with virtual memory
- We will use `mmap()` to
    1. Implement `malloc()` and `free()`
    2. Create data structures independent of the heap
    3. Interact with memory mapped files

