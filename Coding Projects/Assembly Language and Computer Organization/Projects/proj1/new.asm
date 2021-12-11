%define STDIN 0
	        %define STDOUT 1
	        %define STDERR 2
	        %define SYSCALL_EXIT  1
	        %define SYSCALL_READ  3
	        %define SYSCALL_WRITE 4
	        %define BUFLEN 256

	        [SECTION .data]

;;; ; ; ; ; Here we declare initialized data. For example: messages, prompts,
;;; ; ; ; ; and numbers that we know in advance

msg1:	            db "Enter 10-dig ISBN number: "
len1:	            equ $-msg1
msg2:	            db "This is NOT a valid ISBN number.",13,10,0
len2:	            equ $-msg2
msg3:	            db "This is a valid ISBN number.",13,10,0
len3:	            equ $-msg3

	        [SECTION .bss]

;;; ; ; ; ; Here we declare uninitialized data. We're reserving space (and
;;; ; ; ; ; potentially associating names with that space) that our code
;;; ; ; ; ; will use as it executes. Think of these as "global variables"

buf:	             resb BUFLEN ; buffer for read
readlen:	             resb 4 ; storage for the length of string we read

	        [SECTION .text]

	        global _start	; make start global so ld can find it

_start:				; the program actually starts here

;;; ; ; ;  prompt for user input
	                mov eax, SYSCALL_WRITE
	                mov ebx, STDOUT
	                mov ecx, msg1
	                mov edx, len1
	                int 80H

;;; ; ; ;  read user input
	                mov eax, SYSCALL_READ
	                mov ebx, STDIN
	                mov ecx, buf
	                mov edx, BUFLEN
	                int 80H

;;; ; ; ;  remember how many characters we read
	                mov [readlen], eax
;;; ; ; ;  fail if 10 digits not input
	                cmp eax, 11
	                jne .incorrect
;;; ; ; ;  loop over all characters
	                xor ecx, ecx
;;;   t4 is bl , sum4 is bh
	                mov esi, buf
	.loop:
	                cmp ecx, 10 ; counter
	                je .end_loop ; when counter is 10 then done
	                mov al, [esi]
	                cmp al, 88 ; compare to 'X'
	                je .handle_X
	                sub al, 48 ; not X, convert to int
	                cmp al, 0
	                jl .incorrect
	                cmp al, 9
	                jg .incorrect
	                jmp .mod_11_sum

	            .update_loop:
	                inc esi
	                inc ecx	; counter (cl) + 1
	                jmp .loop ; reloop

	            .end_loop:

			cmp bh, 0
	                je .correct
	                jmp .incorrect

	            .mod_11_sum:

;;; ; ; bl is t4
;;; ; ; bh is sum4
	                add bl, al ; t4 += a[i]
	                cmp bl, 11 ; if (t4 >= 11)
	                jl .update_sum4 ;   t4 -= 11
	                sub bl, 11

	                .update_sum4:
	                    add bh, bl ; sum4 += t4
	                    cmp bh, 11 ; if (sum4 >= 11)
	                    jl .update_loop ;   sum4 -= 11
	                    sub bh, 11
	                    jmp .update_loop


	            .handle_X:

			mov al, 10
	                jmp .mod_11_sum

			.incorrect:
;;; ; ; ;  write user input
	                mov eax, SYSCALL_WRITE
	                mov ebx, STDOUT
	                mov ecx, msg2
	                mov edx, len2
	                int 80H
	                jmp .end

;;; ; ; ;  write user input

	                mov eax, SYSCALL_WRITE
	                mov ebx, STDOUT
	                mov ecx, msg3
	                mov edx, len3
	                int 80H
	                jmp .end

	            .end:
			mov eax, SYSCALL_EXIT ; sys_exit syscall
			mov ebx, 0	; no error
			int 80H		; kernel interrupt
			xor ebx, ebx
	