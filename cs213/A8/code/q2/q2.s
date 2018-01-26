.pos 0x0
main:            ld   $0x1028, r5           # r5 = address after stbm(stack bottom)
                 ld   $0xfffffff4, r0       # r0 = -12
                 add  r0, r5                # r5 = allocate caller part of q2's frame 
                 ld   $0x200, r0            # r0 = &p1
                 ld   0x0(r0), r0           # r0 = p1
                 st   r0, 0x0(r5)           # a = p1
                 ld   $0x204, r0            # r0 = &p2
                 ld   0x0(r0), r0           # r0 = p2
                 st   r0, 0x4(r5)           # b = p2
                 ld   $0x208, r0            # r0 = &p3
                 ld   0x0(r0), r0           # r0 = p3
                 st   r0, 0x8(r5)           # c = p3
                 gpc  $6, r6                # set return address  
                 j    0x300                 # call q2(p1, p2, p3)
                 ld   $0x20c, r1            # r1 = &val
                 st   r0, 0x0(r1)           # val = q2 (p1, p2, p3)
                 halt                       # halt
.pos 0x200
p1:              .long 0x00000000           # p1         
p2:              .long 0x00000000           # p2
p3:              .long 0x00000000           # p3
val:             .long 0x00000000           # val

.pos 0x300
q2:              ld   0x0(r5), r0           # r0 = a   
                 ld   0x4(r5), r1           # r1 = b
                 ld   0x8(r5), r2           # r2 = c
                 ld   $0xfffffff6, r3       # r3 = -10
                 add  r3, r0                # r0 = a - 10
                 mov  r0, r3                # r3 = a - 10
                 not  r3                    # r3 = ~(a - 10)
                 inc  r3                    # r3 = ~(a - 10) + 1 = -(a - 10)
                 bgt  r3, default           # if (a - 10) < 0 (a < 10), goto default
                 mov  r0, r3                # r3 = a - 10
                 ld   $0xfffffff8, r4       # r4 = -8
                 add  r4, r3                # r3 = a - 18
                 bgt  r3, default           # if (a - 18) > 0 (a > 18), goto default
                 ld   $0x400, r3            # r3 = &jumptable[0]
                 j    *(r3, r0, 4)          # goto jumptable[a - 10]

.pos 0x330
case10:          add  r1, r2                # r2 = b + c           
                 br   end                   # goto end

case12:          not  r2                    # r2 = ~c                 
                 inc  r2                    # r2 = ~c + 1 = -c
                 add  r1, r2                # r2 = b - c
                 br   end                   # goto end

case14:          not  r2                    # r2 = ~c
                 inc  r2                    # r2 = ~c + 1 = -c
                 add  r1, r2                # r2 = b - c
                 bgt  r2, L0                # if (b - c) > 0 (b > c), goto L0
                 ld   $0x0, r2              # c = 0 
                 br   L1                    # goto L1

L0:              ld   $0x1, r2              # r2 = 1

L1:              br   end                   # goto end
                 
case16:          not  r1                    # r1 = ~b
                 inc  r1                    # r1 = ~b + 1 = -b 
                 add  r2, r1                # r1 = c - b
                 bgt  r1, L2                # if (c - b) > 0 (c > b), goto L2
                 ld   $0x0, r2              # r2 = 0
                 br   L3                    # goto L3

L2:              ld   $0x1, r2              # r2 = 1
                 
L3:              br   end                   # goto end 

case18:          not  r2                    # r2 = ~c
                 inc  r2                    # r2 = ~c + 1 = -c
                 add  r1, r2                # r2 = b - c
                 beq  r2, L4                # if (b-c) = 0 (b = c), goto L4
                 ld   $0x0, r2              # c = 0
                 br   L5                    # goto L5

L4:              ld   $0x1, r2              # r2 = 1

L5:              br   end                   # goto end

default:         ld   $0x0, r2              # r2 = 0           
                 br   end                   # goto end 

end:             mov  r2, r0                # r0 = c  
                 j    0x0(r6)               # return 

.pos 0x400
jumptable:       .long 0x00000330           # case a == 10  
                 .long 0x00000384           # default
                 .long 0x00000334           # case a == 12 
                 .long 0x00000384           # default
                 .long 0x0000033c           # case a == 14 
                 .long 0x00000384           # default
                 .long 0x00000354           # case a == 16 
                 .long 0x00000384           # default
                 .long 0x0000036c           # case a == 18 
.pos 0x1000
                 .long 0x00000000           # stack    
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
stbm:            .long 0x00000000         
