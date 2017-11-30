.pos 0x1000
        ld $0, r0           # i = 0
        ld $0, r1           # o = 0
        ld $1, r2           # r2 = 1
        ld $j, r3           # r3 = &j
        ld (r3), r3         # r3 = j
        ld $a, r4           # r4 = &a

L0:     beq r3, L2          # goto L2 if j==0
        ld (r4,r0,4), r5    # r5 = a[i]
        and r2, r5          # r5 = a[i] & 1
        beq r5, L1          # goto L1 if a[i] == 0
        inc r1              # r1 +=1

L1:     inc r0              # i++
        dec r3              # j--
        br L0               # goto L0

L2:     ld $o, r0           # r0 = &o
        st r1, (r0)         # o = 0
        halt

.pos 0x2000
j:      .long 2
a:      .long 1
        .long 2
        .long 3
o:      .long 0
