; File: bookcmp.asm
;
;Defines the bookcmp subroutine for use by the sort algorithm in sort_books.c
;
%define TITLE_OFFSET 0
%define AUTHOR_OFFSET 33
%define SUBJECT_OFFSET 54
%define YEAR_OFFSET 68
	

        SECTION .text                   ; Code section.
        global  compare                 ; let loader see entry point
	extern	book1, book2

compare:
	
	push 	ebp
	mov 	ebp, esp
	push	edi
	push 	edx
	push	esi
	
; push registers we want to use
;;; 	mov	edi, [book1]	get the pointer to the first book
;;; 	mov	eax, [edi + YEAR_OFFSET] ;   and fetch the year field
;;; 	mov	esi, [book2]		; Get the pointer to the second book
;;; 	cmp	eax, [esi + YEAR_OFFSET] and compare the year field to book1's 

	mov 	edi, [ebp + 8]
	mov 	esi, [edi + AUTHOR_OFFSET + SUBJECT_OFFSET + YEAR_OFFSET]

;;; Now I should have edi and esi pointing to the two different books in memory
	mov 	eax, [edi + YEAR_OFFSET]
	mov 	edx, [esi + YEAR_OFFSET]

	cmp	eax, edx
	jne	cmpDone			; If they're different, we're done
					; with comparisons

cmpTitles:				; Fall through to here if years same
	;; 	mov	edi, [book1]		; Get address of book1's title
	;; 	add	edi, TITLE_OFFSET
	;; 	mov	esi, [book2]	t address of book2's title
	;; 	add	esi, TITLE_OFFSET

	add 	edi, TITLE_OFFSET
	add 	esi, TITLE_OFFSET
	
	
L1:	mov	al, [edi]		; Fetch next char of book1's title
	cmp	byte al, [esi]		; and compare it to matching char in
					; book 2
	jne	cmpDone			; If we find a difference, we are done
					; (note this also works w/single NULL)

	;; Last special case: strings are identical up to, and including, NULL
	cmp	byte al, 0
	je	cmpDone
	inc	edi
	inc	esi
	jmp	L1

cmpDone:
        ;; condition codes still hold result of the last compare, so use it
	jg	L_gt
	je	L_eq
	mov	eax, -1		; book1 is strictly less than book2
	jmp	end
	
L_eq:	mov	eax, 0		; book1 equals book2
	jmp	end
	
L_gt:	mov	eax, 1		; book1 is strictly greater than book2

	;; Clean up and finish
end:	pop	esi
	pop 	edx
	pop	edi
	pop 	ebp
	ret
