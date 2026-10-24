; ==============================================================================
; Computer Organization & Architecture (COA) - Assembly Language Module
; MIT Academy of Engineering, Alandi (Pune) - Second Year B.Tech
; Course Code: 2310221T / 2310221L
;
; Course Outcomes Mapped:
;   - CO1: Explicate 80386/x86_64 architecture, register usage (RAX, RCX, RDX)
;   - CO2: Develop assembly language programs (ALP) using instruction set
;   - CO3: Processor-level details about arithmetic operations (CMP, IMUL, ADD, SUB, TEST)
;   - CO4: Demonstrate control signals, status flags, and priority decision logic
; ==============================================================================

default rel
section .text
global check_traffic_priority
global calculate_green_time
global coa_multiply
global coa_evaluate_sensor

; ------------------------------------------------------------------------------
; check_traffic_priority(int vehicleCount, int threshold)
; ABI: RCX/ECX = vehicleCount, RDX/EDX = threshold
; Output: EAX = 1 if vehicleCount > threshold, else 0
; Demonstrates: ALU subtraction (CMP) and conditional flag setting (SETG)
; ------------------------------------------------------------------------------
check_traffic_priority:
    xor eax, eax            ; Clear accumulator EAX = 0
    cmp ecx, edx            ; ALU subtraction: vehicleCount - threshold
    setg al                 ; If count > threshold (ZF=0 and SF=OF), set AL = 1
    ret

; ------------------------------------------------------------------------------
; calculate_green_time(int queueSize, int isEmergency)
; ABI: RCX/ECX = queueSize, RDX/EDX = isEmergency
; Output: EAX = calculated green duration in seconds
; Demonstrates: TEST bitwise evaluation, conditional jump (JNZ), IMUL arithmetic
; ------------------------------------------------------------------------------
calculate_green_time:
    test edx, edx           ; Bitwise AND to check if emergency flag is non-zero
    jnz .emergency_corridor ; If emergency active, jump to high-priority allocation

    ; Normal dynamic cycle: 6s base + (queueSize * 2)
    mov eax, ecx            ; EAX = queueSize
    imul eax, 2             ; EAX = queueSize * 2 (ALU multiplication)
    add eax, 6              ; EAX = (queueSize * 2) + 6
    cmp eax, 18             ; Compare with maximum clamp (18 seconds)
    jle .done               ; If <= 18s, keep result
    mov eax, 18             ; Else clamp to 18s
    jmp .done

.emergency_corridor:
    mov eax, 12             ; Emergency preemption green corridor duration (12s)

.done:
    ret

; ------------------------------------------------------------------------------
; coa_multiply(int a, int b) - CO3: Arithmetic Operations
; ABI: RCX/ECX = a, RDX/EDX = b
; Output: EAX = a * b
; ------------------------------------------------------------------------------
coa_multiply:
    mov eax, ecx
    imul eax, edx
    ret

; ------------------------------------------------------------------------------
; coa_evaluate_sensor(int roadMask, int emergencyMask) - CO4: Control Signals
; ABI: RCX/ECX = roadMask, RDX/EDX = emergencyMask
; Output: EAX = evaluated control signal bits
; ------------------------------------------------------------------------------
coa_evaluate_sensor:
    mov eax, ecx
    or eax, edx
    ret
