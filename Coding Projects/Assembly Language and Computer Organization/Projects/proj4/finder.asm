%define STDIN 0
%define STDOUT 1
%define STDERR 2
%define SYSCALL_EXIT  1
%define SYSCALL_READ  3
%define SYSCALL_WRITE 4

;;  Offsets for fields in struct book.
;;
%define AUTHOR_OFFSET 21
%define TITLE_OFFSET 0
%define SUBJECT_OFFSET 36
%define YEAR_OFFSET 44

	_start:	 
	
		push eax
		push ebx
		push ecx
		push edx	

		extern book1, book2

		global bookcmp

	bookcmp:
		
	
	;;  "mov eax" will return whatever the function is going to do to the program


		pop edx
		pop ecx
		pop ebx
		pop eax