%macro	syscall1 2
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro	syscall3 4
	mov	edx, %4
	mov	ecx, %3
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro  exit 1
	syscall1 1, %1
%endmacro

%macro  write 3
	syscall3 4, %1, %2, %3
%endmacro

%macro  read 3
	syscall3 3, %1, %2, %3
%endmacro

%macro  open 3
	syscall3 5, %1, %2, %3
%endmacro

%macro  lseek 3
	syscall3 19, %1, %2, %3
%endmacro

%macro  close 1
	syscall1 6, %1
%endmacro

%macro get_pic_addr 1
        call get_my_loc
        sub esi, dword next_i-%1
%endmacro

%define	STK_RES	200
%define	RDWR	2
%define	SEEK_END 2
%define SEEK_SET 0

%define STDOUT 1 

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8
	
	global _start

	section .text
	
	get_my_loc:
            call next_i
            
        next_i:
            pop esi
            ret
	
_start:	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage
	
        get_pic_addr OutStr
        write STDOUT, esi, 31

        
        get_pic_addr FileName
        open esi, RDWR, 0777
        
        
        get_pic_addr .passed
        cmp eax, dword 0
        jge .passed
        
        exit -1
        
    .passed:
        mov dword [ebp-4], eax
        
        mov edi, ebp
        sub edi, dword 8
        read [ebp-4], edi, 4
        
        get_pic_addr VirusExit
        cmp dword [ebp-8], 0x464c457f
        je .is_ELF
        
        close [ebp-4]
        exit -1
        
        .is_ELF:
        
        
       

VirusExit:
       exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)
	
FileName:	db "ELFexec", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
Failstr:        db "perhaps not", 10 , 0
	
PreviousEntryPoint: dd VirusExit
virus_end:


