default rel
section .text
global check_traffic

check_traffic:
    xor eax, eax
    cmp ecx, edx
    setg al
    ret
