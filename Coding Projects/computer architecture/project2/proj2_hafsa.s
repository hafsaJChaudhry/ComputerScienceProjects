# Tell assembler to not insert instructions to fill branch delay slots.
# This is necessary when branch delay slots are disabled.
.set noreorder
.set noat 

.global _start
_start:
	ADDI $1, $0, 0 		#R1 = w_old = 0 
	ADDI $2, $0, 10*4 	#R2 = LAST loop value
	ADDI $3, $0, 0x100	#R3 = in 
	ADDI $4, $0, 0x200	#R4 = out
	ADDI $5, $0, 0x300	#R5 = w
	
LOOP: 
	BEQ $1, $2, DONE 	#if R1 == R2 then DONE  [to get out of loop] 
	
	LW  $6, 0($3)		#R6 = in(n)
	LW  $7, -4($5) 		#R7 = w(n-1)  aka ... w_old
	SRA $8, $7, 1		#R8 = 0.5*w(n-1)
	ADD $8, $6, $8		#R8 = in(n) + 0.5*w(n-1)
	SW  $8, 0($5) 		#stores R8(above) into R5(w) aka ... w = in(n) + 0.5*w(n-1)
	
	SUB $10, $8, $7 	#R10 = w - w(n-1) aka ... w - w_old
	SW $10, 0($4) 		#stores R10(above) into R4(out) aka ... out = w - w_old
	
	ADDI $1, $1, 4  	#R1 = 4?
	ADDI $3, $3, 4		#R3 = 4?
	ADDI $4, $4, 4		#R4 = 4?
	ADDI $5, $5, 4 		#R5 = 4?
	
	J 	LOOP
DONE: NOP				#exits 