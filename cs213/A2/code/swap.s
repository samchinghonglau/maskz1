.pos 0x100
                 ld   $t, r0              # r0 = address of t
                 ld   $a, r1              # r1 = address of array[0]
                 ld   (r1), r2            # r2 = array[0]
                 st   r2, (r0)            # t = array[0]
                 ld   $0x1, r3            # r3 = 1
                 ld   (r1, r3, 4), r2     # r2 = array[1]
                 st   r2, (r1)            # array[0] = array [1]
                 ld   $t, r0              # r0 = address of t
                 ld   (r0), r2            # r2 = t
                 st   r2, (r1, r3, 4)     # array[1] = t
                 halt                     # halt

.pos 0x1000
t:               .long 0xffffffff         # t
.pos 0x2000
a:               .long 0xffffffff         # a[0]
                 .long 0xffffffff         # a[1]
