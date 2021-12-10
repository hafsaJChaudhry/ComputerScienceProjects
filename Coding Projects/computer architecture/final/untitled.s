# Use .set noreorder to prevent the assembler from filling branch
# delay slots, if you want to fill delay slots manually.
 .set noreorder

.global _start
_start:
	addi $1, $0, 0x100;     # array[]
	lw $3, 0($1)			# max
	lw $4, 0($1)			# min
	addi $5, $0, 0			# i
	addi $6, $0, 4*6		
	addi $7, $0, 0 			# array[i]
	addi $8, $0, 0			# cur
	addi $9, $0, 0			# temp
	
loop:
	beq	$5, $6, loop		
	lw	$8, 0($1)			
	slt	$9, $3, $8			# max < array[i]
	beq	$9, $0, Max	
	slt	$9, $4, $8			# array[i] < min
	beq	$9, $0, Min	
	sw	$8, 0($1)
	addi	$1, $1, 4
	addi	$5, $5, 4
	j loop	

Max:
	add	$3, $0,	$8		# max = array[i]
	slt	$9, $4, $8		# array[i] < min
	beq	$9, $0, Min	
	addi	$1, $1, 4
	addi	$5, $5, 4
	j loop
	
Min:
	add	$4, $0, $8		# min = array[i]
	addi	$1, $1, 4
	addi	$5, $5, 4
	j loop 