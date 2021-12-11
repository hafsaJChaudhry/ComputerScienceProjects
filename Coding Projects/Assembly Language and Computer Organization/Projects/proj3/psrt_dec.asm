%define STDIN 0
%define STDOUT 1
%define SYSCALL_EXIT  1
%define SYSCALL_READ  3
%define SYSCALL_WRITE 4


        [SECTION .data]	; initialized data section

lf:	     db  10		; just a linefeed

msg1:	   db " plus "
	len1 equ $ - msg1

msg2:	   db " minus "
	len2 equ $ - msg2

msg3:	   db " equals "
	len3 equ $ - msg3
	
print0:	   db "0"
	len4 equ $ - print0
	
        [SECTION .text]	; Code section.
	global prt_dec


	.prt_dec:
	
		cmp esp, 0
		je .zero
		push eax
		push ebx
		push ecx
		push edx
		
	;; ecx will contain a counter that will tell the length of the esp item
		mov ecx, 0
		mov eax, [esp]
		mov ebx, 10
		call .handleLoop
	;;  At this point, we have the length in ecx, quotient in eax, remainder in edx
;;; Find out what to do with these numbers and how to print them out in order
;;; Then the next lines will return whatever was stored in the stack from the registers
;;; to their respective register
	
		pop edx
		pop ecx
		pop ebx
		pop eax
	

	.zero:

        	mov     eax, SYSCALL_WRITE ; write message
	        mov     ebx, STDOUT
	        mov     ecx, print0
	        mov     edx, len4
	        int     080h
		jmp .done

	.handleLoop:
;;; edx and eax will be storing the quotient and 
		div ebx
		add ecx, 1

		cmp esp, 0
		jge .handleLoop
	;; Return control back to calling function
		ret



	.done:
		