section .asm

global insb
global insw
global outb
global outw

insb:
    ; prepare frame
    push ebp 
    mov ebp, esp

    xor eax, eax ; set eax to 0
    mov edx, [ebp+8]
    in al, dx
    
    ; end frame
    pop ebp
    ret

insw:
    ; prepare frame
    push ebp 
    mov ebp, esp

    xor eax, eax ; set eax to 0
    mov edx, [ebp+8]
    in ax, dx
    
    ; end frame
    pop ebp
    ret

outb:
    push ebp
    mov ebp, esp

    mov eax, [ebp+12]
    mov edx, [ebp+8]
    out dx, al

    pop ebp
    ret

outw:
    push ebp
    mov ebp, esp

    mov eax, [ebp+12]
    mov edx, [ebp+8]
    out dx, ax

    pop ebp
    ret