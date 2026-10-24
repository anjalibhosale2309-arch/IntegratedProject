; ==============================================================================
; MIT ACADEMY OF ENGINEERING, ALANDI (PUNE)
; Department of Computer Engineering
; Course: Computer Organization and Architecture (2310221T / 2310221L)
; Course Outcomes:
;   - CO1: Explicate architecture and instruction set of 8086/80386 processor
;   - CO2: Develop assembly language programs (ALP) using instruction set
;   - CO3: Processor level details about arithmetic operations (CMP, JBE)
;   - CO4: Control signals and decision logic
;
; Description:
;   8086 Assembly Program to compare active lane vehicle count with threshold
;   and generate priority control signal for traffic light override.
; ==============================================================================

.MODEL SMALL
.STACK 100H

.DATA
    VEHICLE_COUNT DB 14        ; Current vehicle count in lane
    THRESHOLD     DB 8         ; Congestion threshold
    PRIORITY      DB 0         ; 0 = Normal Cycle, 1 = Priority Green Needed
    ROAD_ID       DB 3         ; 0=East, 1=West, 2=South, 3=North

.CODE
MAIN PROC
    MOV AX, @DATA              ; Initialize Data Segment
    MOV DS, AX

    MOV AL, VEHICLE_COUNT      ; Load vehicle count into Accumulator AL
    CMP AL, THRESHOLD          ; Compare AL with THRESHOLD (ALU subtraction)
    JBE NORMAL_TRAFFIC         ; If Count <= Threshold, jump to Normal

HIGH_TRAFFIC:
    MOV PRIORITY, 1            ; Set Priority Override Flag = 1
    JMP FINISH

NORMAL_TRAFFIC:
    MOV PRIORITY, 0            ; Set Priority Flag = 0

FINISH:
    MOV AH, 4CH                ; DOS terminate program interrupt
    INT 21H
MAIN ENDP

END MAIN
