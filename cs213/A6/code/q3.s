.pos 0x100
		#calculate average for each student
		#for(int i = 0; i < n; i++;)
		ld $0, r3		# r3 = temp_i = 0
if:		ld $n, r4 		# r4 = address of n
		ld (r4), r4		# r4 = n
		not r4			# r4 = -n
		inc r4
		add r3, r4 		# r4 = temp_i - n
		beq r4, endif 	# if (temp_i - n = 0) then go to end
		
		#inside for loop
		mov r3, r4		# r4 = temp_i
		mov r3, r5		# r5 = temp_i
		shl $3, r4		# r4 = 8 * temp_i
		shl $4, r5		# r5 = 16 * temp_i
		add r5, r4		# r4 = 8 * temp_i + 16 * temp_i = temp_i * 24
	 
		#store s[i] student's average
		ld $s, r0		# r0 = address of s
		ld (r0), r0		# r0 = s = address of s[0]
		add r4, r0		# r0 = address of s[i]
		ld 4(r0), r1	# r1 = s[i].grade[0]
		ld 8(r0), r2	# r2 = s[i].grade[1]
		add r2,r1		# r1 = s[i].grade[0] + s[0].grade[1]
		ld 12(r0), r2	# r2 = s[i].grade[2]
		add r2,r1		# r1 = s[i].grade[0] + s[0].grade[1] + s[0].grade[2]
		ld 16(r0), r2	# r2 = s[i].grade[3]
		add r2,r1		# r1 = s[i].grade[0] + s[0].grade[1] + s[0].grade[2] + s[0].grade[3]
		shr $2, r1		# r1 = r1 >> 2
		st r1, 20(r0)	# s[i].average = r1
		
		inc r3			# temp_i++
		br if			# go to if
		#end for loop
endif:	
		
		#sorting
		#for (int j=0; j < n; j++;)
		
		ld $n,r0
		ld (r0),r0
		dec r0
		beq r0, endif4
		
		ld $0, r6		# r6 = 0 = temp_j
if3:	ld $n, r0
		ld (r0),r0		# r0 = n
		not r0			# r0 = -n
		inc r0
		add r6, r0		# r6 = temp_j - n
		beq r0, endif4	# if (temp_j - n) then go to endif4
	
		#for(int i=0; i < n - j;i++;)
		ld $0, r0		# r0 = 0 = temp_i2
if2:	ld $n, r5		# r5 = address of n
		ld (r5),r5		# r5 = n
		not r5			# r5 = -n + 1
		inc r5
		inc r5
		add r0,r5		# r5 = temp_i2 - n + 1
		beq r5,endif3	# if (temp_i2 - n + 1) then go to endif3
		
		#if (s[i+1].average - s[i].average > 0)
		ld $s, r1		# r1 = address of s
		ld (r1), r1		# r1 = s = address of s[0]
		
		mov r0, r2		# r4 = temp_i2
		mov r0, r5		# r5 = temp_i2
		shl $3, r2		# r4 = 8 * temp_i2
		shl $4, r5		# r5 = 16 * temp_i2
		add r5, r2		# r4 = 8 * temp_i2 + 16 * temp_i2 = temp_i * 24
		add r2, r1		# r1 = address of s[i]
		
		ld 20(r1),r3	# r3 = s[i].average
		ld 44(r1),r4	# r4 = s[i+1].average
		not r4			# r4 = -s[i+1].average		
		inc r4
		add r3,r4		# r4 = s[i+1].average - s[i].average
		bgt r4,then		# if (s[i+1].average - s[i].average > 0) then go to "then"
		br endif2
		
		#transfer data
then:	ld (r1), r3		# r3 = s[0].id
		ld 24(r1), r4		# r4 = s[1].id
		st r4,(r1)		# s[0].id = s[1].id
		st r3,24(r1)		# s[1].id = s[0].id
		ld 4(r1), r3	# r3 = s[0].grade[0]
		ld 28(r1), r4	# r4 = s[1].grade[0]
		st r4,4(r1)		# s[0].grade[0] = s[1].grade[0]
		st r3,28(r1)		# s[1].grade[0] = s[0].grade[0]
		ld 8(r1), r3	# r3 = s[0].grade[1]
		ld 32(r1), r4	# r4 = s[1].grade[1]
		st r4,8(r1)		# s[0].grade[1] = s[1].grade[1]
		st r3,32(r1)		# s[1].grade[1] = s[0].grade[1]
		ld 12(r1), r3	# r3 = s[0].grade[2]
		ld 36(r1), r4	# r4 = s[1].grade[2]
		st r4,12(r1)	# s[0].grade[2] = s[1].grade[2]
		st r3,36(r1)	# s[1].grade[2] = s[0].grade[2]
		ld 16(r1), r3	# r3 = s[0].grade[3]
		ld 40(r1), r4	# r4 = s[1].grade[3]
		st r4,16(r1)	# s[0].grade[3] = s[1].grade[3]
		st r3,40(r1)	# s[1].grade[3] = s[0].grade[3]
		ld 20(r1), r3	# r3 = s[0].average
		ld 44(r1), r4	# r4 = s[1].average
		st r4,20(r1)	# s[0].average = s[1].average
		st r3,44(r1)	# s[1].average = s[0].average
endif2:	
		inc r0			# temp_i2++
		br if2			# go to if2
endif3:
		inc r6			# temp_j++
		br if3			# go to if3
endif4:
		# median
		ld $s,r0		# r0 = address of s
		ld $n, r1		# r1 = address of n
		ld (r1),r1		# r1 = n
		shr $1, r1		# r1 = n/2
		ld (r0),r0		# r0 = s = address of s[0]
		mov r1, r2		# r4 = temp_i
		mov r1, r3		# r5 = temp_i
		shl $3, r2		# r4 = 8 * temp_i
		shl $4, r3		# r5 = 16 * temp_i
		add r2, r3		# r3 = 8 * n/2 + 16 * n/2 = n/2 * 24
		add r3, r0		# r0 = address of s[n/2]
		ld (r0),r0	# r0 = id of median
		ld $m, r1
		st r0, (r1)		# m = median
		halt			#halt

		
		
.pos 0x1000
n:		.long 7 		# number of students
m: 		.long 0 		# median of students
s: 		.long base		# address of dynamic array of stu.

base: 	.long 1			# student id
		.long 80		# grade 0
		.long 60		# grade 1
		.long 78		# grade 2
		.long 94		# grade 3
		.long 0			# computed average
		
		.long 2			# student id
		.long 23		# grade 0
		.long 66		# grade 1
		.long 47		# grade 2
		.long 5			# grade 3
		.long 0			# computed average

		.long 3			# student id
		.long 60		# grade 0
		.long 80		# grade 1
		.long 70		# grade 2
		.long 70		# grade 3
		.long 0 		# computed average
		
		.long 4			# student id
		.long 91		# grade 0
		.long 89		# grade 1
		.long 74		# grade 2
		.long 74		# grade 3
		.long 0 		# computed average
		
		.long 5			# student id
		.long 91		# grade 0
		.long 89		# grade 1
		.long 74		# grade 2
		.long 74		# grade 3
		.long 0 		# computed average
		
		.long 6			# student id
		.long 23		# grade 0
		.long 66		# grade 1
		.long 47		# grade 2
		.long 5			# grade 3
		.long 0			# computed average

		.long 7			# student id
		.long 80		# grade 0
		.long 60		# grade 1
		.long 78		# grade 2
		.long 86		# grade 3
		.long 0			# computed average