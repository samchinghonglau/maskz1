.pos 0x100

    # i = a[3]
    ld $a, r0           # r0 = address of a
    ld $3, r1           # r1 = 3
    ld (r0, r1, 4), r0  # r0 = a[3]
    ld $i, r2           # r2 = address of i
    st r0, (r2)         # i = a[3]

    # i = a[i]
    ld (r2), r1         # r1 = i
    ld $a, r0           # r0 = address of a
    ld (r0, r1, 4), r1  # r1 = a[i]
    ld $i, r2           # r2 = address of i
    st r1, (r2)         # i = a[i]

    # p  = &j;
    ld $p, r3           # r3 = address of p
    ld $j, r4           # r4 = address of j
    st r4, (r3)         # p = &j

    # *p = 4;
    ld $4, r5           # r5 = 4
    st r5, (r4)         # *p = 4

    # p  = &a[a[2]];
    ld $2, r6           # r6 = 2
    ld $a, r0           # r0 = address of a
    ld (r0, r6, 4), r1  # r1 = a[2]
    shl $2, r1          # r1 = a[2] << 2 = &a[2] * 4
    add r1, r0          # r0 = &a[a[2]]
	ld $p, r3			# r3 = address of p
    st r0, (r3)         # p = &a[a[2]]

    # *p = *p + a[4];
    ld $4, r6           # r6 = 4
    ld $a, r0           # r0 = address of a
    ld (r0, r6, 4), r0  # r0 = a[4]
	ld $p, r1			# r1 = address of p
    ld (r1), r6         # r6 = p
	ld (r6),r5			# r6 = *p
    add r5, r0          # r0 = *p + a[4]
    st r0, (r6)         # *p = *p + a[4]

    halt

.pos 0x200
# Data area

i:  .long 11             # i
j:  .long 12             # j
p:  .long 13             # p
a:  .long 0             # a[0]
    .long 1             # a[1]
    .long 2             # a[2]
    .long 3             # a[3]
    .long 4             # a[4]
    .long 5             # a[5]
    .long 6             # a[6]
    .long 7             # a[7]
    .long 8             # a[8]
    .long 9             # a[9]
