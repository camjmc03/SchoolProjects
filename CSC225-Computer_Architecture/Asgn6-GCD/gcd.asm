; Defines functions for computing greatest common divisors.
; CSC 225, Assignment 6
; Given code, Spring '21
; TODO: Complete this file.

            .ORIG x4000

; int gcd(int, int)
      
            ;---------;
            ;--SETUP--;
            ;---------;
            ; callee half of setup
            ; Push space for the return value
GCDFN       ADD R6, R6, #-1
            ; Push the return address (R7)
            ADD R6, R6, #-1
            STR R7, R6, #0
            ; Push the dynamic link (R5)
            ADD R6, R6, #-1
            STR R5, R6, #0
            ; Set the new frame pointer (R5)
            ADD R5, R6, #-1
            ; Push local variables (normal order)
            ADD R6, R6, #-1 ;tmp
            
            ;-------------;
            ;--EXECUTION--;
            ;-------------;
            ST  R0, SAVER0
            ST  R1, SAVER1
            ST  R2, SAVER2
            ST  R3, SAVER3
            ST  R4, SAVER4
            
            LDR R1, R5, #4  ; load arg a
            LDR R2, R5, #5  ; load arg b
            
            NOT R3, R1      ; negate arg a and put into R3
            ADD R3, R3, #1
            
                            
            ADD R4, R3, R2  ; if (a == b) {
            BRz EQUAL       ;     tmp = a;
                            ; }
            ADD R4, R3, R2  ; else if (a < b) {
            BRp LESS        ;     tmp = gcd(a, b - a);
                            ; }
            ADD R4, R3, R2, ; else {
            BRn GREATER     ;     tmp = gcd(a - b, b);
                            ; }
            
EQUAL       STR R1, R6, #0  ; tmp = a
            BRnzp CALLEETD

LESS        LDR R1, R5, #4  ; load arg a
            LDR R2, R5, #5  ; load arg b
            NOT R3, R1      ; negate a
            ADD R3, R3, #1
            ADD R2, R2, R3  ; R2 now holds b-a
            ADD R6, R6, #-1 ; Push b-a
            STR R2, R6, #0  
            ADD R6, R6, #-1 ; Push a
            STR R1, R6, #0  
            LEA  R0, GCDFN
            JSRR R0         ; Call "gcd(a, b - a)".
            BRnzp CALLERTD

GREATER     LDR R1, R5, #4  ; load arg a
            LDR R2, R5, #5  ; load arg b
            NOT R3, R2      ; negate b
            ADD R3, R3, #1
            ADD R1, R1, R3  ; R1 now holds a-b
            ADD R6, R6, #-1 ; Push b
            STR R2, R6, #0  
            ADD R6, R6, #-1 ; Push a-b
            STR R1, R6, #0  
            LEA  R0, GCDFN
            JSRR R0         ; Call "gcd(a - b, b)".
            
            ; Caller half of teardown from recursive call
CALLERTD    LDR R1, R6, #0  ; Pop the return value.
            ADD R6, R6, #1
            ADD R6, R6, #1  ; Pop "a".
            ADD R6, R6, #1  ; Pop "b".
            STR R1, R6, #0  ; store return into tmp
            
            LD  R0, SAVER0
            LD  R1, SAVER1
            LD  R2, SAVER2
            LD  R3, SAVER3
            LD  R4, SAVER4
            
            ;------------;
            ;--TEARDOWN--;
            ;------------;
            ; callee half of teardown
            ; Place return value in Stack Frame
CALLEETD    LDR R1, R6, #0  ; load in tmp
            STR R1, R5, #3  ; return value = tmp
            ; Pop local variables
            ADD R6, R6, #1  ; pop local var tmp
            ; Pop the dynamic link (R5)
            LDR R5, R6, #0  ; load dynamic link of next stack frame down into r5
            ADD R6, R6, #1  ; pop pop logic
            ; Pop the return address (R7)
            LDR R7, R6, #0  ; load return address into R7
            ADD R6, R6, #1  ; pop logic
            RET
            ; Return (RET/JMP R7)
SAVER0      .FILL X0000
SAVER1      .FILL X0000
SAVER2      .FILL X0000
SAVER3      .FILL X0000
SAVER4      .FILL X0000


        .END
