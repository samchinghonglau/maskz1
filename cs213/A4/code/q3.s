.pos 0x1000
code:
		#v0 = s.x[i];
		ld $i, r0										#r0 = address of i
		ld (r0),r0										#r0 = i
		ld $s, r1										#r1 = address of s
		ld (r1,r0,4),r1									#r1 = s.x[i]
		ld $v0,r2										#r2 = address of v0
		st r1,(r2)										#v0 = s.x[i]

		#v1 = s.y[i];
		ld $i, r0										#r0 = address of i
		ld (r0),r0										#r0 = i
		ld $s,r1										#r1 = s
		ld $2, r2										#r2 = 3
		ld (r1,r2,4),r3									#r3 = address of s.y
		ld (r3,r0,4),r3									#r3 = s.y[i]
		ld $v1,r4										#r4 = v1
		st r3,(r4)										#v1 = s.y[i]

		#v2 = s.z->x[i];
		ld $i, r0										#r0 = address of i
		ld (r0), r0										#r0 = i
		ld $s, r1										#r1 = address of s
		ld $3, r2										#r2 = 3
		ld (r1,r2,4),r3									#r3 = address of s.z
		ld (r3,r0,4),r3									#r3 = s.z->x[i]
		ld $v2, r4										#r4 = v2
		st r3,(r4)										#v2 = s.z->x[i]


		#v3 = s.z->z->y[i];
		ld $i, r0										#r0 = address of i
		ld (r0), r0										#r0 = i
		ld $s, r1										#r1 = s
		ld $3, r2										#r2 = 4
		ld (r1,r2,4),r3									#r3 = address of s.z
		ld (r3,r2,4),r3									#r3 = address of s.z->z
		ld $2, r2										#r2 = 3
		ld (r3,r2,4),r3									#r3 = address of s.z->z->y
		ld (r3,r0,4),r3									#r3 = s.z->z->y[i]
		ld $v3, r4										#r4 = v3
		st r3,(r4)										#v3 = s.z->z->y[i]

		halt 											#halt

.pos 0x2000
static:
i:							.long 0
v0:							.long 0
v1: 						.long 0
v2: 						.long 0
v3: 						.long 0
s:							.long 123					# s.x[0]
							.long 234					# s.x[1]
							.long 0x00003000			# s.y
							.long 0x00003008			# s.z

.pos 0x3000
heap:
s_y: 						.long 1						# s.y[0]
							.long 2						# s.y[1]
s_z:						.long 3						# s.z->x[0]
							.long 4						# s.z->x[1]
							.long 10					# s.z->y
							.long 0x00003018			# s.z->z
s_z_z:						.long 5						# s.z->z->x[0]
							.long 6						# s.z->z->x[1]
							.long 0x00003028			# s.z->z->y
							.long 1						# s.z->z->z
s_z_z_y:					.long 7						# s.z->z->y[0]
							.long 8 					# s.z->z->y[1]
