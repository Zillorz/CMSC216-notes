# These are some tests that prove some asm behavior
# before a jcc .ERROR_*, there will be a movl $IDX, %edi
# .if you want a error reason, it's on line 275+IDX
# this could be a lot better, but its what I got ig :\

.text
.global over_under_flow 
over_under_flow:
    movb $255, %al
    addb $1, %al

    # 255 + 1 = 0?
    cmpb $0, %al

    movl  $1, %edi
    jne  .ERROR_F

    # we can actually check overflow
    movw  $0xFFFF, %ax
    addw  $0x1, %ax

    # use jc/jnc (carry) for unsigned
    movl  $2, %edi
    jnc   .ERROR_F

    movw  $0xFFFF, %ax
    // remove top bit, making number positive
    shlw  $1, %ax
    shrw  $1, %ax

    // %ax is now 16-bit max signed
    addw  $0x1, %ax

    # use jo/jno (overflow) for signed
    movl  $3, %edi
    jno   .ERROR_F

    ret
.ERROR_F:
    pushq %rax
    call assert_failure
    popq %rax
    ret


.global shifts_are_mult 
shifts_are_mult:
    movl $8247, %eax
    movl %eax, %ecx
    
    # 8247 * 4 = 8247 << 2
    imull $4, %eax
    sall  $2, %ecx

    cmpl  %eax, %ecx
    movl  $4, %edi
    jne   .ERROR_SM

    # also true for division
    # A / 4 = A >> 2
    movl  $4, %r8d
    movl  $0, %edx

    cltq
    idivl %r8d

    sarl  $2, %ecx

    cmpl  %eax, %ecx
    movl  $5, %edi
    jne   .ERROR_SM
    
    ret
.ERROR_SM:
    pushq %rax
    call assert_failure
    popq %rax
    ret


.global high_bit_behavior 
high_bit_behavior:
    movq  $0, %rax
    notq  %rax # every bit of rax is 1

    movq  %rax, %rdx

    # moving 32bit data to edx zeroes the top bits of rdx
    movl  $32, %edx

    cmpq  $32, %rdx
    movl  $6, %edi
    jne   .ERROR_H

    # moving 16/8 bit data does not zero the top bits
    movq  %rax, %rdx
    movw  $0x7E, %dx # expect similar behavior with movb

    cmpq  $0xFFFFFFFFFFFF007E, %rdx
    movl  $7, %edi
    jne   .ERROR_H

    ret
.ERROR_H:
    pushq %rax
    call assert_failure
    popq %rax
    ret


.global function_caller;
function_caller:
    # to call a function the stack must be divisible by 16
    # by default, this means you must sub 8
    subq  $8, %rsp

    call  private_function
    # return value is in eax

    cmpl  $13, %eax
    movl  $8, %edi
    jne   .ERROR_FC


    # must align stack again at end
    addq  $8, %rsp

    ret
.ERROR_FC:
    call assert_failure
    popq %rax
    ret

private_function:
    mov $13, %eax
    ret

.global ptrs_on_stack
ptrs_on_stack:
    // make space for two ints on the stack
    subq  $8, %rsp

    leaq  (%rsp), %rdx
    # movq %rsp, %rdx also works
    leaq  4(%rsp), %rcx

    # modify with dereference (%reg)
    movw  $0x4, (%rdx)
    movw  $0x15, (%rcx)

    # make sure modification worked
    movl  (%rsp), %r8d
    movl  4(%rsp), %r9d

    movl  $9, %edi
    
    cmpl  $0x4, %r8d
    jne   .ERROR_PS

    cmpl  $0x15, %r9d
    jne   .ERROR_PS

    addq  $8, %rsp

    ret

.ERROR_PS:
    call assert_failure
    popq %rax
    ret

.data
a_message: 
  .string "[GLOBALS] I'm a evil test, I printed on my own!"

single_big_int:
  .int 16

big_array:
  .int 9 
  .int 5
  .int 7

.text
.global globals
globals:

    # print a message
    leaq a_message(%rip), %rdi
    subq $8, %rsp
    call puts
    addq $8, %rsp

    # modify a global
    addq  $15, single_big_int(%rip)

    movl  $10, %edi
    cmpl  $31, single_big_int(%rip)
    jne   .ERROR_G

    leaq  big_array(%rip), %rcx

    # rsi is index
    movq  $2, %rsi
    
    movl  $11, %edi
    cmpl  $7, (%rcx, %rsi, 4)
    jne   .ERROR_G

    ret
.ERROR_G:
    pushq %rax
    call assert_failure
    popq %rax
    ret


# This struct is the first argument
# struct Pack {
#     int ab;
#     short c;
#     char d;
#     char e;
# };
# we passed in { .ab = 17, .c = 34, .d = 'A', .e = 'Z' }
.global a_packed_struct
a_packed_struct:

    # move from rdi to rsi because we use edi
    movq  %rdi, %rsi
    
    # first 32 bits are ab
    movl $12, %edi
    cmpl $17, %esi
    jne .ERROR_APS

    # next 16 bits are c
    movq %rsi, %rcx
    shrq $32, %rcx
    andq $0xFFFF, %rcx

    movl $13, %edi
    cmpq $34, %rcx
    jne .ERROR_APS


    # next 8 bits are d, and 8 after that are e
    movq %rsi, %rcx
    shrq $48, %rcx

    movq %rcx, %rdx
    shrq $8, %rdx

    andq $0xFF, %rcx
    andq $0xFF, %rdx

    movl $14, %edi
    cmpq $'A', %rcx
    jne .ERROR_APS
    
    movl $15, %edi
    cmpq $'Z', %rdx
    jne .ERROR_APS
    
    ret

.ERROR_APS:
    pushq %rax
    call assert_failure
    popq %rax


.data
test_errors:
    .string "THESE_MUST_BE_THIS_LONG_ALWAYS" # this one is a placeholder, there isn't a test 0
    .string "Overflow did not work         " # 1
    .string "Unsigned carry not detected   " # 2
    .string "Signed carry not detected     " # 3
    .string "Shift left is not multiply    " # 4
    .string "Shift right is not divide     " # 5
    .string "movl did not clear top 32bits " # 6
    .string "movw/b cleared top 48/56bits  " # 7
    .string "Did not call function correct " # 8
    .string "Stack pointers didn't work    " # 9
    .string "Could not modify/read global  " # 10
    .string "Indexing global didn't work   " # 11
    .string "Reading Pack.ab failed        " # 12
    .string "Reading Pack.c failed         " # 13
    .string "Reading Pack.d failed         " # 14
    .string "Reading Pack.e failed         " # 15

.text
.global get_reason
get_reason:
    leaq  test_errors(%rip), %rdx
    imulq $31, %rdi
    leaq  (%rdx, %rdi), %rax
    ret

