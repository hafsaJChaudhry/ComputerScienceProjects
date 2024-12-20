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

msg1:       db "Enter 10-dig ISBN number: "
len1:	    equ $-msg1
msg2:	    db "This is NOT a valid ISBN number.",13,10,0
len2:	    equ $-msg2
msg3:	    db "This is a valid ISBN number.",13,10,0
len3:	    equ $-msg3

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
		cmp eax, 11
		jne .incorrect
;;; ; ; ;  loop over all characters
		xor ecx, ecx
        	mov esi, buf
	
	.loop:
                cmp ecx, 10
                je .finish
                mov al, [esi]
                cmp al, 'X'
                je .handleX
                sub al, 48
                cmp al, 0
                jl .incorrect
                cmp al, 9
                jg .incorrect
                jmp .subtractEl

        .increment:
                inc esi
                inc ecx
                jmp .loop
       	.finish:
                cmp bh, 0
                je .success
                jmp .incorrect

	.subtractEl:
	
                add bl, al
                cmp bl, 11
                jl .update_sum4
                sub bl, 11
	
        .update_sum4:
        	add bh, bl
                cmp bh, 11
                jl .increment
        	sub bh, 11
                jmp .increment
        .handleX:
		mov al, 10
        	jmp .subtractEl

	.success:

;;; ; ; ; ;  write user input
		mov eax, SYSCALL_WRITE
		mov ebx, STDOUT
		mov ecx, msg3
		mov edx, len3
		int 80H
		jmp .end
	
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

        .end:
                mov eax, SYSCALL_EXIT ; sys_exit syscall
		mov ebx, 0	; no error
		int 80H		; kernel interrupt
		xor ebx, ebx

	