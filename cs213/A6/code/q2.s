.pos 0x100
                ld $0, r0                 # r0 = temp_i = 0   
                ld $a, r1                 # r1 = &a
                ld $0, r2                 # r2 = temp_s = 0
                ld $0xfffffffb, r3        # r3 = -5

loop:           mov r0, r4                # r4 = temp_i
                add r3, r4                # r4 = temp_i - 5
                beq r4, end_loop          # goto end_loop if r4 == 0
                ld (r1, r0, 4), r5        # r5 = a[i]
                bgt r5, L0                # goto L0 if r5 > 0

L0:             add r5, r2                # temp_s += a[i]
                inc r0                    # temp_i ++        
                br loop                   # goto loop

end_loop:       ld $s, r1                 # r1 = address of s       
                st r2, 0x14(r1)           # s = temp_s
                st r4, 0x0(r1)            # i = temp_i
                halt                     
.pos 0x1000
i:               .long 10                 # i
a:               .long 10                 # a[0]
                 .long -30                # a[1]
                 .long -12                # a[2]
                 .long 4                  # a[3]
                 .long 8                  # a[4]
s:               .long 0                  # s
