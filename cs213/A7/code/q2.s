.pos 0x100
start:
    ld $sb, r5                  # sp = address of last word of stack
    inca    r5                  # sp = address of next word after stack
    gpc $6, r6                  # r6 = pc + 6
    j main                      # call main()
    halt                        # halt

f:
    deca r5                     # allocate caller part of f's frame  
    ld $0, r0                   # r0 = 0
    ld 4(r5), r1                # r1 = x[i]
    ld $0x80000000, r2          # r2 = 0x80000000
f_loop:
    beq r1, f_end               # if r1 == 0, goto f_end
    mov r1, r3                  # r3 = r1
    and r2, r3                  # r3 = r3 & r2
    beq r3, f_if1               # if r3 == 0, goto f_if1
    inc r0                      # r0 = r0 + 1
f_if1:
    shl $1, r1                  # r1 = r1 * 2 
    br f_loop                   # goto f_loop
f_end:
    inca r5                     # deallocate caller part of f's frame
    j(r6)                       # return

main:
    deca r5                     # allocate callee part of main's frame
    deca r5                     # allocate callee part of main's frame
    st r6, 4(r5)                # store return address to stack
    ld $8, r4                   # i = 8
main_loop:
    beq r4, main_end            # if i == 0, goto main_end
    dec r4                      # i--
    ld $x, r0                   # r0 = address of x
    ld (r0,r4,4), r0            # r0 = x[i]
    deca r5                     # allocate caller part of f's frame
    st r0, (r5)                 # store value of x[i] into stack
    gpc $6, r6                  # set return address
    j f                         # jump to f
    inca r5                     # deallocate caller part of f's frame

    ld $y, r1                   # r1 = address of y
    st r0, (r1,r4,4)            # y[i] = r0
    br main_loop                # goto main_loop
main_end:
    ld 4(r5), r6                # load return address from stack
    inca r5                     # deallocate callee part of main's frame
    inca r5                     # deallocate callee part of main's frame
    j (r6)                      # return

.pos 0x2000
x:
    .long 1
    .long 2
    .long 3
    .long 0xffffffff
    .long 0xfffffffe
    .long 0
    .long 184
    .long 340057058

y:
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0

.pos 0x8000
# These are here so you can see (some of) the stack contents.
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
sb: .long 0

