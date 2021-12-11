%define STDIN 0
	%define STDOUT 1
	%define STDERR 2
	%define SYSCALL_EXIT  1
	%define SYSCALL_READ  3
	%define SYSCALL_WRITE 4
	%define BUFLEN 256

	[SECTION .data]
;;; ; Here we declare initialized data. For example: messages, prompts,
;;; ; and numbers that we know in advance

msg1:	   db "Enter 10-dig ISBN number: "	; user prompt
len1:	   equ $-msg1		; length of first message

msg2:	   db "Original Number" ; original string label
len2:	   equ $-msg2		  ; length of second message

msg3:	   db "Incorrect ISBN number   " ; converted string label
len3:	   equ $-msg3

msg4:	   db "Correct ISBN number"
len4:	   equ $-msg4
	
	[SECTION .bss]
;;; ; Here we declare uninitialized data. We're reserving space (and
;;; ; potentially associating names with that space) that our code
;;; ; will use as it executes. Think of these as "global variables"

buf:	 resb BUFLEN		; buffer for read
newstr:	 resb BUFLEN		; converted string
readlen:	 resb 4		; storage for the length of string we read

	[SECTION .text]

	global _start		; make start global so ld can find it

_start:				; the program actually starts here
;;;  prompt for user input
	        mov eax, SYSCALL_WRITE
	        mov ebx, STDOUT
	        mov ecx, msg1
	        mov edx, len1
	        int 80H

;;;  read user input
	        mov eax, SYSCALL_READ
	        mov ebx, STDIN
	        mov ecx, buf
	        mov edx, BUFLEN
	        int 80H

;;;  remember how many characters we read
	        mov [readlen], eax

;;;  loop over all characters
	        mov esi, buf
	        mov edi, newstr
	
	.loop:
		cmp ecx, 0 
		je .done
		mov al, [esi]
		sub al, 48
		inc esi
		cmp al, 'X'
		je .overwrite
		add ah, al
		cmp ah, 11
		jge .closeFinish
		dec ecx
		jmp .loop
	
	.closeFinish:
		sub ah, 11
		jmp .addUp

	.subtractEl:

		sub bh, 11
		dec ecx
		jmp .loop

	.addUp:
        	add bh, ah
		cmp bh, 11
		jge .subtractEl
	

	.overwrite:
		mov al, 10
		add ah, al
		cmp ah, 11
		jge .overwrite
		dec ecx
		jmp .loop
	.done:
		cmp bh, 0
		jg .failure
		mov eax, SYSCALL_WRITE
		mov ebx, STDOUT
		mov ecx, msg2
		mov edx, len2
		int 80H
		jmp .finish

	.failure:
		mov eax, SYSCALL_WRITE
		mov ebx, STDOUT
		mov ecx, msg3
		mov edx, len3
		int 80H
		jmp .finish

	.finish:
		mov eax, SYSCALL_WRITE
		mov ebx, STDOUT
 		mov ecx, msg4
 		mov edx, len4
		int 80H
;;; Finally end this
        	jmp .end
	.end:
		mov eax, SYSCALL_EXIT ; sys_exit syscall
		mov ebx, 0 ; no error
		int 80H	; kernel interrupt  