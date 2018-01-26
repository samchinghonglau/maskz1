.pos 0x100
start:           ld   $stackBtm, r5       # sp = address of last word of stack
                 inca r5                  # sp = address of word after stack
                 gpc  $6, r6              # ra = pc + 6
                 j    main                # call main()
                 halt

.pos 0x200
main:            deca r5                  # allocate callee part of main's frame
                 st r6, (r5)              # store return address on stack
                 deca r5                  # allocate caller part of copy's frame
                 deca r5                  # allocate caller part of copy's frame

                 mov r5, r1               # r1 = r5

                 gpc $6, r6               # r6 = pc + 6
                 j copy                   # call copy()

                 ld (r5), r6
                 inca r5                  # deallocate callee part of main's frame              
                 j (r6)

.pos 0x300
copy:            ld $0, r0                # r0 = i = 0
                 ld $src, r2              # r1 = &src[0]
                 

copy_loop:       ld (r2, r0, 4), r3       # r3 = src[i]
                 beq r3, end              # goto end if src[i] == 0
                 st r3, (r1, r0, 4)       # store src[i] into stack
                 inc r0                   # i++
                 br copy_loop             # goto copy_loop  

end:             inca r5                  # deallocate caller part of copy's frame
                 inca r5                  # deallocate caller part of copy's frame
                 ld $0, r4                # r4 = 0
                 st r4, (r5, r0, 4)       # store dst[i] into stack
                 j (r6)                   # return
                          

.pos 0x400
src:             .long 0x00000001
                 .long 0x00000001         
                 .long 0x1010
                 .long 0xff000000          # nop
                 .long 0xffffffff          # r0 = 0
                 .long 0xff000100          # nop
                 .long 0xffffffff          # r0 = 0
                 .long 0xff000200          # nop
                 .long 0xffffffff          # r0 = 0
                 .long 0xff000300          # nop
                 .long 0xffffffff          # r0 = 0
                 .long 0xff000400          # nop
                 .long 0xffffffff          # r0 = 0
                 .long 0xff000500          # nop
                 .long 0xffffffff          # r0 = 0
                 .long 0xff000600          # nop
                 .long 0xffffffff          # r0 = 0
                 .long 0xff000700          # nop
                 .long 0xffffffff          # r0 = 0
                 .long 0xff000000          # nop
                 .long 0xffffffff          # r0 = 0
                 .long 0xf000f000 

.pos 0x1000
stackTop:        .long 0x00000000
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
stackBtm:        .long 0x00000000