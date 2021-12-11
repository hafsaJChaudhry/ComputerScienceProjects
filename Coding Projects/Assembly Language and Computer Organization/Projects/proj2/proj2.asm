%define STDIN 0
%define STDOUT 1
%define STDERR 2
%define SYSCALL_EXIT  1
%define SYSCALL_READ  3
%define SYSCALL_WRITE 4
%define BUFLEN 256

	[SECTION .data]

msg1:	   db "Enter string: "	; user prompt
len1:	   equ $-msg1		; length of  message

msg2:	   db "Original: "	; original string label
len2:	   equ $-msg2		; length of second message

msg3:	   db "Convert:  "	; converted string label
len3:	   equ $-msg3

msg4:	   db  "Error: unknown escape sequence", 10 ; unknown escape error
len4:	   equ $-msg4

msg5:	   db "Error: octal value overflow!", 10 ; overflow error
len5:	   equ $-msg5


table:	 db 07, 08, -1, -1, -1, 12, -1, -1, -1, -1, -1, -1, -1, 10, -1, -1, -1, 13, -1, 9, -1, 11, -1, -1, -1, -1

	[SECTION .bss]


buf:	 resb BUFLEN		; buffer for read
newstr:	 resb BUFLEN		; converted ACII to decimal
readlen:	 resb 4		; storage for the length of string we read


	[SECTION .text]

OA	global _start

_start:

	    mov     eax, SYSCALL_WRITE
	    mov     ebx, STDOUT
	    mov     ecx, msg1
	    mov     edx, len1
	    int     80H

	    mov     eax, SYSCALL_READ
	    mov     ebx, STDIN
	    mov     ecx, buf
	    mov     edx, BUFLEN
	    int     80H
	    mov     [ecx + eax-1], byte 0 ; null terminate final buffer

	    mov     [readlen], eax

	;;  loop over all characters
	    mov esi, buf
	    mov edi, newstr

	.loop:
	    mov bh, [esi]
	    cmp bh, 0
	    je .print

	    add     esi, 1
	;;  Find where you have a backslash in ascii
	    cmp     bh, 05Ch

	    je      .foundBackSlashChar
				
	;; Progress as usual if you don't find a backslash character
	    mov     [edi], bh
	    inc     edi		
	    jmp     .loop	


	.foundBackSlashChar:
	    call    .seq
	;; Once control returns, step to next letter, print, go to loop
	    add     edi, 1
	    cmp     esi, 0
	    je      .print
	    jmp     .loop

	.print:
	;;  print the original string message
	    mov eax, SYSCALL_WRITE
	    mov ebx, STDOUT
	    mov ecx, msg2
	    mov edx, len2
	    int 80H

	;;  print the original string
	    mov eax, SYSCALL_WRITE
	    mov ebx, STDOUT
	    mov ecx, buf
	    mov edx, [readlen]
	    int 80H

	;;  print the converted string message
	    mov eax, SYSCALL_WRITE
	    mov ebx, STDOUT
	    mov ecx, msg3
	    mov edx, len3
	    int 80H

	;;  print the converted string
	    mov eax, SYSCALL_WRITE
	    mov ebx, STDOUT
	    mov ecx, newstr
	    mov edx, [readlen]
	    int 80H

	    jmp .end

	.end:

	        mov eax, SYSCALL_EXIT ; sys_exit syscall
	        mov ebx, 0	      ; no error
	        int 80H


	.seq:
	        mov     bh, [esi]
	        add     esi, 1

	;; See if we find two backslashes
	;; Still trying to figure out why this doesn't function
		cmp	bh, 05Ch
        	je 	.backslash

		cmp bh, 05Ch
		jne .letters

		
	
	.letters:
	;;  only transform values that are between 'a' and 'z'
	        cmp bh, 'a'
	        jl .lettercomplete
	        cmp bh, 'z'
	        jg .lettercomplete

	;;  use the offset between lowercase letters and uppercase letters
	;;  in the ASCII table to shift the letter to lowercase
	        sub bh, 'a'

	        movzx eax,bh
	        mov bl, [table + eax]

	        cmp bl , -1		
	        mov [edi], bl


	.lettercomplete:
	    ret



	.backslash:
	;;  Keep track of this backslash and then step to next letter and return back to calling function to proceed
	        mov     [edi], byte 05Ch
	        add     edi, 1		
	        ret

	.notEscSeq:
		mov     eax, SYSCALL_WRITE
	        mov     ebx, STDOUT
	        mov     ecx, msg4
	        mov     edx, len4
	        int     80H
	
;;;  This program cannot handle two backslashes (92 or \\), currently
;;;  Only \a, \b, \t, \n\, \v, \f, and \r :(