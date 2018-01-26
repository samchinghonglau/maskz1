.pos 0x0
                 ld   $sb, r5               # sp = address of last word of stack
                 inca r5                    # sp = address of word after stack
                 gpc  $6, r6                # ra = pc + 6
                 j    0x300                 # jump to foo    
                 halt                       # halt
.pos 0x100
                 .long 0x00001000            
.pos 0x200
                 ld   0x0(r5), r0           # r0 = b0
                 ld   0x4(r5), r1           # r1 = b1
                 ld   $0x100, r2            # r2 = address of 0x1000
                 ld   0x0(r2), r2           # r2 = arr[0]
                 ld   (r2, r1, 4), r3       # r3 = arr[b1]
                 add  r3, r0                # r0 = b0 + arr[b1]
                 st   r0, (r2, r1, 4)       # arr[b1] = b0 + arr[b1]
                 j    0x0(r6)               # return
             
.pos 0x300
                 ld   $0xfffffff4, r0       # r0 = -12
                 add  r0, r5                # allocate callee part of c's frame
                 st   r6, 0x8(r5)           # store return address to stack
                 ld   $0x1, r0              # r0 = 1 = a0
                 st   r0, 0x0(r5)           # store value of a0 to stack
                 ld   $0x2, r0              # r0 = 2 = a1
                 st   r0, 0x4(r5)           # store value of a1 to stack 

                 ld   $0xfffffff8, r0       # r0 = -8
                 add  r0, r5                # allocate caller part of c's frame
                 ld   $0x3, r0              # r0 = 3 = b0
                 st   r0, 0x0(r5)           # store value of b0 to stack
                 ld   $0x4, r0              # r0 = 4 = b1
                 st   r0, 0x4(r5)           # store value of b1 to stack 
                 gpc  $6, r6                # set return address  
                 j    0x200                 # c(3,4)
                 ld   $0x8, r0              # r0 = 8
                 add  r0, r5                # deallocate caller part of c's frame

                 ld   0x0(r5), r1           # r1 = a0 = 1
                 ld   0x4(r5), r2           # r2 = a1 = 2
                 ld   $0xfffffff8, r0       # r0 = -8
                 add  r0, r5                # allocate caller part of c's frame
                 st   r1, 0x0(r5)           # b0 = a0
                 st   r2, 0x4(r5)           # b1 = a1
                 gpc  $6, r6                # set return address                 
                 j    0x200                 # c(1,2)
                 ld   $0x8, r0              # r0 = 8
                 add  r0, r5                # deallocate caller part of c's frame
                 ld   0x8(r5), r6           # load return address from stack

                 ld   $0xc, r0              # r0 = 12
                 add  r0, r5                # deallocate callee part of c's frame 
                 j    0x0(r6)               # return       
.pos 0x1000
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
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
