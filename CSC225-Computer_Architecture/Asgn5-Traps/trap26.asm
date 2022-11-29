; Supports interrupt-driven keyboard input.
; CSC 225, Assignment 5
; Given code, Winter '20

            .ORIG x1000

; Reads one character, executing a second program while waiting for input:
;  1. Saves the keyboard entry from the IVT.
;  2. Sets the keyboard entry in the IVT to ISR180.
;  3. Enables keyboard interrupts.
;  4. Returns to the second program.
; NOTE: The first program's state must be swapped with the second's.
    
            ; save unmangled registers
TRAP26      ST  R1, P1R1
            ST  R2, P1R2
            ST  R3, P1R3
            ST  R4, P1R4
            ST  R5, P1R5
            ST  R7, P1R7
            ; save program1's PC
            LDR R1, R6, #0  ; load PC into R0
            ST  R1, P1PC    ; store PC into P1PC saver location
            ; swap with P2's PC
            LD  R1, P2PC    ; load P2's PC into R0
            STR R1, R6, #0  ; Put P2's PC into the admin stack where P1's was previously
            ; save P1's PSR
            LDR R1, R6, #1  ; load PSR into R0
            ST  R1, P1PSR   ; store PC into P1PC saver location
            ; swap with P1's PSR
            LD  R1, P2PSR   ; load P2's PSR into R0
            STR R1, R6, #1  ; Put P2's PSR into the admin stack where P1's was previously
            
            ; save default KBIV 
            LDI R1, KBIV
            ST  R1, SAVEIV
            
            ; set the KBIV to point to ISR180
            LEA R2, ISR180  ; load the address of the new service routine into R2
            STI R2, KBIV    ; store the new vector into the KBIV memory address
            
            ; enable keyboard interrupts
            LD  R1, KBIMASK ; LD the keyboard status register address into r1
            STI R1, KBSR
            
            ; load in the Registers from R2
            LD  R0, P2R0
            LD  R1, P2R1
            LD  R2, P2R2
            LD  R3, P2R3
            LD  R4, P2R4
            LD  R5, P2R5
            LD  R7, P2R7
            
            RTI
            

; Responds to a keyboard interrupt:
;  1. Disables keyboard interrupts.
;  2. Restores the original keyboard entry in the IVT.
;  3. Reads the typed character into R0.
;  4. Returns to the caller of TRAP26.
; NOTE: The second program's state must be swapped with the first's.
            ; save P2 registers
ISR180      ST  R0, P2R0    
            ST  R1, P2R1
            ST  R2, P2R2
            ST  R3, P2R3
            ST  R4, P2R4
            ST  R5, P2R5
            ST  R7, P2R7
            
            ; output pressed key into R0
            LDI R0, KBDR
            ; disable keyboard interrupts
            LDI R2, KBSR    ; load the contents of the kbsr into R2
            LD  R3, KBIMASK ; load the interupt on mask into R3
            NOT R3, R3      ; negate the interupt mask
            ADD R3, R3, #1  
            ADD R2, R2, R3  ; add the mask to the KBSR value
            STI R2, KBSR    ; store KBSR value with flipped bit back into KBSR address
            
            ; set the KBIV to point to the original
            LD  R1, SAVEIV  ; Store the original KBIV into the address referencing the KBIV memory location
            STI R1, KBIV    
            ; swap in P1's state
            LD  R1, P1PC    ; load P1's PC into R0
            STR R1, R6, #0  ; Put P1's PC into the admin stack where P1's was previously
            
            LD  R1, P1PSR   ; load P2's PSR into R0
            STR R1, R6, #1  ; Put P2's PSR into the admin stack where P1's was previously
            ; swap in P1's Register values
            LD  R1, P1R1    
            LD  R2, P1R2
            LD  R3, P1R3
            LD  R4, P1R4
            LD  R5, P1R5
            LD  R7, P1R7
            
            RTI


; Program 1's data:
P1R1        .FILL x0000     ; TODO: Use these memory locations to save and
P1R2        .FILL x0000     ;       restore the first program's state.
P1R3        .FILL x0000
P1R4        .FILL x0000
P1R5        .FILL x0000
P1R7        .FILL x0000
P1PC        .FILL x0000
P1PSR       .FILL x0000

; Program 2's data:
P2R0        .FILL x0000     ; TODO: Use these memory locations to save and
P2R1        .FILL x0000     ;       restore the second program's state.
P2R2        .FILL x0000
P2R3        .FILL x0000
P2R4        .FILL x0000
P2R5        .FILL x0000
P2R7        .FILL x0000
P2PC        .FILL x4000     ; Initially, Program 2's PC is 0x4000.
P2PSR       .FILL x8002     ; Initially, Program 2 is unprivileged.

; Shared data:
SAVEIV      .FILL x0000     ; TODO: Use this memory location to save and
                            ;       restore the keyboard's IVT entry.

; Shared constants:
KBIV        .FILL x0180     ; The keyboard's interrupt vector
KBSR        .FILL xFE00     ; The Keyboard Status Register
KBDR        .FILL xFE02     ; The Keyboard Data Register
KBIMASK     .FILL x4000     ; The keyboard interrupt bit's mask


            .END
