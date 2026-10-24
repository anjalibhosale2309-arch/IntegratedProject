
; Smart Traffic - COA Demo
; 16-bit DOS program for NASM

org 100h

section .text
start:
    mov al, [vehicle_count]
    cmp al, [threshold]
    jbe normal_traffic

    mov byte [priority], 1
    jmp finish

normal_traffic:
    mov byte [priority], 0

finish:
    mov al, [priority]

    ; Display priority result
    cmp al, 1
    je high_traffic

    mov dx, normal_msg
    mov ah, 09h
    int 21h
    jmp exit_program

high_traffic:
    mov dx, high_msg
    mov ah, 09h
    int 21h

exit_program:
    mov ax, 4C00h
    int 21h

section .data
vehicle_count db 18
threshold     db 10
priority      db 0

normal_msg db 'Normal traffic priority', 13, 10, '$'
high_msg   db 'High traffic priority', 13, 10, '$'
