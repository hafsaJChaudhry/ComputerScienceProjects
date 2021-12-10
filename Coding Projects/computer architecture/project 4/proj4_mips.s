# Use .set noreorder to prevent the assembler from filling branch
# delay slots, if you want to fill delay slots manually.
# .set noreorder

.global _start
_start:
	addi $1, $0, 0x100;	#in
	addi $2, $0, 0x200; #hysteresis[]
	addi $3, $0, 5; 	#const int low_limit = 5
	addi $4, $0, 25;    #const int high_limit = 25
	addi $5, $0, 40;    #const int out_high = 40
	addi $6, $0, 0;		#const int out_low = 0	
	addi $7, $0, 9; 	#Loop Bound
	addi $8, $0, 0;		# i
	addi $9, $0, 0;		#int previous_out
hysteresis:
	beq	$8, $7, end;	# while (i < loop bound)
	lw	$10, 0($1);		#load in
	slt	$12, $3, $10;	# if (low_limit < in)
	slt	$11, $10, $4;	# Check if (in < high_limit)
	and	$13, $11, $12;	# &&
	addi $14, $0, 1;	# if && == 1;
	beq	$13, $14, previous_out;
	bgt $10, $4,out_high;	#out_high;
	blt	$10, $5, out_low;	#out_low
out_low:
	sw	$6, 0($2);		#hysteresis[i] = 40
	add	$9, $9, $6;		#previous_out = 40
	addi $1, $1, 4;		#in[i+1]
	addi $2, $2, 4;		#hysteresis[i+1]
	addi $8, $8, 4;		#i++
	j hysteresis;
out_high:
	sw	$5, 0($2);		
	add	$9, $9, $5;		
	addi $1, $1, 4;		
	addi $2, $2, 4;		
	addi $8, $8, 4;		
	j hysteresis;
previous_out:
	sw	$9, 0($2);		#hysteresis[i] = out_old;
	addi $1, $1, 4;		#in[i+1]
	addi $2, $2, 4;		#hysteresis[i+1]
	addi $8, $8, 4;		#i++	
	j hysteresis;
end:
	nop;
	