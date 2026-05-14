# Lecture 16 - 3/31/26
Unix processes and exceptional control flow

### Traditional vs Modern Computing Devices
- Old-school computers had a single executing programs which could interact freely with all parts of the computing hardware
- Modern computing devices have different expectations summarized below

| Traditional   | Modern |
|--------------- | --------------- |
| Single program on device   | Multiple programs sharing single device   |
| No Operating System | OS manages all programs   |
| Program has access to all hardware | OS controls/coordinates hardware access |
| Relatively simple hardware interactions | OS isolates memories of each program |
| Single "user" running programs at once | Complex interactions of many devices |
| Apple II: insert disk to run program | Mac OS: Click to start another program |

- New hardware and expectations led to new concepts
- **Operating Systems**: "manager" program that coordinates activities of all programs / users, manages hardware and provides abstraction layer, enforces security and fairness
- **Process**: a running program with its context

### OS Kernel and Kernel Mode
1. The central or most important part of something

Operating System code is usually in the **kernel**, a program that starts running when a computing system is powered on. 

- Kernel sets up handlers for various exceptional control flows
- Most CPUs have (at least) two modes
    1. **User / Normal** mode
    2. **Kernel / Privileged / Supervisor**  mode
- User programs run in user mode, cannot directly manipulate hardware or access to certain resources
- Requests OS to perform some operations which jumps to kernel code running in kernel mode

Ex) BSD, Linux, NT, XNU

### Processes: Running Programs
- Hardware just executes a stream of instructions
- The OS creates the notion of a **process*, a running program and its resources
- Processes can be executed for a while, then paused while another process executes, then continues
- To accomplish this, OS usually provides
    1. Bookkeeping info for processes (resources)
    2. Ability to interrupt / pre-empt a running process to allow OS actions to take place
    3. Scheduler that decides which process runs and for how long
- Will discuss these from a **Systems Programming** perspective: control low-level program details to utilize OS Service Hardware as effectively as possible

### Exceptional Control flow
- CPUs use "regular" control flow most of the time but support several kinds of **exceptional control flow**
- General idea is as follows

- Kernel tracks data structures associated with Processes that allows them to be paused/resumed
- **Process Context** includes
    - Values of registers
    - Regions of main memory
    - Open files and resources
- Switching between processes is a **Context Switch**
    - OS saves context A somewhere, and loads context B for other program
    - Later, can resume with the context A that we saved

#### Enable Multiple Processes to Share the CPU
- OS sets a hardware timer
- OS Starts Process A running, A's code runs in user mode;
- When timer expires ("rings"), control jumps to the OS
- OS can select Process B to run, resuming A later after B's timer expires

#### Hiding I/O Latency
- Processes pause while waiting for I/O, allowing other code to run while I/O

> [!NOTE]
> CMSC412: Operating Systems
> CMSC412 covers a lot more details

### Overview of Process Creation/Coordination
- The following are the 4 fundamental process creation / coordination system calls provided by UNIX
- `getpid() / getppid()`
    - Get process ID of self, or parent
- `fork()`
    - Create a child process
    - Identical to parent EXCEPT for return value of fork() call and PID
    - returns 0 if child, otherwise returns id of child
    - Determines child/parent
    - calling fork is recursive/exponential, the following code prints **4** times
```c
fork();
fork();
printf("Hi!");
```

- `wait() / waitpid()`
    - Wait for any child to finish
    - Wait for a specific child to finish
- `exec() / execvp()`
    - Replace running program with another executable

> [!NOTE]
> This [video](https://youtu.be/SwIPOf2YAgI), is **really** good for this
