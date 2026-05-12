These get harder as they go on

### Question 1
Boj is trying to write code for the following function
```c
char *strdouble(char *inp);
```

The `strdouble` function is provided a string inp and should return the string concatenated to itself. You can assume that inp is null terminated.

Example calls to `strdouble`
```c
char *old_str = "bob1";
char *new_str = strdouble(old_str);

printf("%s\n", new_str); // "bob1bob1"
free(new_str);
```

### Question 2
Alice is trying to write a function which takes a list as input and returns a reversed copy. Alice is trying to use the following function prototype, but Casey argues that more needs to be added to the prototype. Who is right? Why are they right? If Casey is right, what should the function prototype be?

Alices function prototype:
```c
int *reverse(int *list);
```

### Question 3
David is wrote a function to add two ints together. But it doesn't seem to work. David understands that returning a 'int' instead of an 'int*' fixes the function, but he wants to return a pointer.

```c
int *calculate_sum(int a, int b) {
    int c = a + b;
    return &c;
}

int main() {
    int *sum = calculate_sum(2, 3);
if (*sum == 5) {
        printf("Yay\n");
    }
    return 0;
}
```

When running the following program under valgrind, David receives this valgrind report.
```
/*
    Invalid read of size 4
        at 0x4007C0: main (sum.c:8)
    Address 0x3434 is not stack'd, malloc'd or (recently) free'd
*/
```

Why doesn't this work? Explain how David can change the function to fix the issue, and explain what causes the invalid read.

### Question 4
Ethan and Frank work as programmers at a car company. Ethan wrote a routine to calculate the air pressure of a tire, but Frank believes that he can improve on the routine by adjusting the sensor_t struct.

Ethan argues:
```c
struct sensor_t {
    float force;
    float radius;

    int id;
    char state;
}

// num_sensors is between 768 and 1682
float calculate_air_pressure(struct sensor_t *sensors, size_t num_sensors) {
    float total_pressure = 0.0;

    for(size_t i = 0; i < num_sensors; i++) {
        total_pressure = sensors[i].force * sensors[i].radius;
    }

    return total_pressure;
}
```

Frank argues that struct sensor_t *sensors, should instead be a list 
```c
struct sensor_list_t {
    float forces*;
    float radii*;

    int ids*;
    char states*;
}
```

A) Who is right? Assume that the code is running on a modern x86_64 cpu. Explain how hardware in the CPU leads to this result; 

B) For Ethan's solution, what is the stride? How many bytes apart are sensors\[0\].force and sensors\[1\].force?

C) Let's say that Ethan and Frank cannot come to an agreement, how could they time the two code variants to see which is faster?

### Question 5
Gertrude is new to assembly and needs your help with a lot of things. 

A)
Firstly, to implement the function
```c 
int dereference_and_double(int *arg);
```

Gertrude wrote
```asm
# how do I say this is my code??
# Do i need something here? Gcc isn't finding this implementation
dereference_and_double:
    movl %edi, %eax 
    # eax is gibberish? Did I forget a step?
    addq %eax, %eax
    ret
```

There are a multitude of problems with this code. Write a fix to ALL of them. Use Gertrude's comments as a hint.

B)
Gertrude was also trying to translate the following conditional to assembly, but got stuck.

```c
if (rsi > 25) {
    rsi += 5;
} else if (rsi < 10) {
    rsi = 0;
}
```

Translate this into assembly for him. Assume the 'rsi' variable in c is stored in the '%rsi' register.

C) Gertrude saw a `testl %eax %eax` in some code and was confused, explain why this instruction could be present.

D) Gertrude was trying to call another function in assembly, but it didn't seem to work. What does he need to add to this code?

```asm
function_outer:
    # do work
    call function_inner
    # do work
    ret
```

E) After your fix in (D), Gertrude noticed that the registers '%rsi' and '%rdi' changed values after the function call, but '%rbx' did not. Explain why. In your explanation, explain how any compliant 'function_inner' should have similar behavior, at least with respect to '%rbx'.

### Question 6
Hamilton has two structs. Both of these structs are packed into registers.

```c
struct student_t {
    int id;
    short room;
    char first_inital;
    char last_inital;
};
```


```c
struct teacher_t {
    char first_inital;
    char last_inital;
    short room;
    int id; 
};
```

Hamilton wrote the following function the process_student function in assembly and it worked. But had trouble with process_teacher
```c
void process_student(student_t student);
void process_teacher(teacher_t teach);
```

He thought since student_t and teacher_t have the same fields, he could copy the logic to process_teacher. 

A) Explain why it didn't work. 

B) Then, then write following function in assembly.
```c
int get_id(teacher_t teach);
```

C) During a particularly complicated section of code, Hamilton didn't have any registers left, but needed to store more data. Name the two regions of logical memory and explain briefly how he would access them.

### Question 7
Jacob is writing a web server and found that his program crashed. While he was initially annoyed, his professor, Kauffman, told him that this was actually the best case scenario and he was lucky.

Jacob did some investigating, and found this was the code that was causing the problem.

```c
void read_from_network(connection_t *con, char *buf);

void handle_request(connection_t con) {
    char buf[1024];
    read_from_network(&con, buf);

    if (strcmp(buf, "Heartbeat")) {
        handle_heartbeater(con);
    }

    // more code...
}
```

A) Explain why the program crashed. Talk about the regions of memory and how they grow, counter-measures that prevented worse case scenarios.

*tricky and a little unrelated*

B) How could Jacob fix this? Could he rely on a message-length value from the connection?

### Question 8
Ian, which is also Jacob's last name, is confused as to how user executables cannot just take control of everything on a computer. Explain to Ian how CPU and kernel design prevents one program from manipulating memory, hardware, and storage while still being able to execute arbitrary code.

### Question 9
Levermore was compiling a latex document and scrolling through lecture notes at the same time. He realized his computer only has one CPU core and that it could not execute code concurrently. Explain how Levermore is able to have two programs running at once.

### Question 10
Matei was looking at some notes on his computer and noticed that in process manager, his browser had 12 processes open, but he only saw one window. Explain why Matei saw so many processes and why the program could be using multiple processes.

### Question 11
Olivia is trying to coordinate processes with the following code.

```c
int number = 0;

for(int i = 0; i < 3; i++) {
    pid_t pid = fork();

    if (pid == 0) {
        calculate_slow_sum();
        return;
    }
}
```

A) She realizes that the parent exits before the children, explain what happens to the child process when this happens, and explain how she can fix it without sacrificing concurrency.

### Question 12
Peligro is building multiple tools, but doesn't know when to use multi-processing vs multi-threading. Pick one for the following 4 programs and explain why.

1. Web Browser
2. Integral (math) calculator
3. Indexer (finds files across many directories)
4. Web Server which handles private client data, which cannot be leaked to other clients

### Question 13
Quincy, son of Quincy, wants to parallelize his code. Help him out

```c
double product(matr_t matrix) {
    double product = 1.0;
    for (int r = 0; r < matrix.rows; r++) {
        for(int c = 0; c < matrix.cols; c++) {
            product *= MGET(matrix, r, c);
        }
    }

}
```

### Question 14
Richard has a problem with his multi-threaded code.

```c
int SUM = 0;

struct ctx_t {
    int inp;
}

// many threads do stuff here
void *worker(void *inp) {
    ctx_t *ctx = inp;

    for(int i = 0; i < 100; i++) {
        SUM += slow_calculation(ctx->inp);
    }
}

// spawns multiple threads
```

A) Explain how Richard could modify his code  
B) Richard listened to you! But his code is slower than the non-threaded solution, why?

```c
struct ctx_t {
    int inp;
    int *sum;
    pthread_mutex_t *lock;
}

void *worker(void *inp) {
    ctx_t *ctx = inp;

    for(int i = 0; i < 100; i++) {
        pthread_mutex_lock(ctx->lock);
        ctx->sum += slow_calculation(ctx->inp);
        pthread_mutex_unlock(ctx->lock);
    }
}
```

### Question 15
Take project 5. Let's say the OS sent us a message that memory is tight and we want to save memory. Write a function which frees/removes pages from the heap if empty.

```c
void remove_pages_from_head();
```

### Question 16
Optimize this function to run faster.

```c
int order_matters(matr_t matrix) {
    int sum = 0;

    for(int c = 0; c < matrix.cols; c++) {
        for(int r = c; r < matrix.row; r++) {
            sum += MGET(matrix, r, c);
            sum -= MGET(matrix, r-1, c-1);
        }
    }
}
```
