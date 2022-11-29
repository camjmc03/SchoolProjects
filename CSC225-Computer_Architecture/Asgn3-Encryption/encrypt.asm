; Encrypts a string using a Caesar cipher.
; CSC 225, Assignment 3
; Given code, Spring '22

            .ORIG x3000

            ; Prompt for the key.
            LEA R0, KEY_PROMPT  ; Load the prompt.
            PUTS                ; Print the prompt.

            ;       Read the encryption key from the keyboard, echo it, and
            ;       convert it into an integer. Save the encryption key in R3.
            LD  R1 NEW_OFFSET
GET_ENCKEY  GETC
            OUT                 ; ...echo the character...
            ADD R4, R0, R1      ; ...and the character...
            BRz GETEK_DONE      ; ...is not the newline...
            LD  R1, INT_OFFSET  ; load -48 into r1 (offset from ASCII value to integer value)
            ADD R3, R0, R1      ; ADD the inputted ASCII character plus the offset into R3
            GETC                ; get another character...
            AND R0, R0, #0      ; ...if that character is not a newline character...
            ADD R0, R0, x0A     ; ...do a new line anyways 
            OUT
            
            

            ; Prompt for the string.
GETEK_DONE  LEA R0, STR_PROMPT  ; Load the prompt.
            LEA R2, STRING      ; Load the address of the string into R2.
            PUTS                ; Print the prompt.

            ; Get and encrypt the string.
            ;
            ; R0: inputted char
            ; R1: offset
            ; R2; mem address counter for writing a string
            ; R3; encryption key
            ; R4; used as result of checking character against offset
            ; 
LOOP        GETC                ; While the user types characters...
            OUT                 ; ...echo the character...
            LD  R1, NEW_OFFSET 
            ADD R4, R0, R1      ; ...and the character...
            BRz DONE            ; ...is not the newline...
            
            ; LEA R1, END_STRING  ; load mem address of the end of the string block
            ; NOT R1, R1          ; negate it/ ADD 1
            ; ADD R1, R1, #1      ; ...
            
            ; ADD R1, R1, R2      ; if R1 is less than or equal, then the string is too long
            ; BRzp    LONG_DONE

            ;       Apply the encryption key, which is in R3, to the character,
            ;       which is in R0. Replace unprintable characters with '?'.
            
            ADD R5, R0, R3      ; ADD char in R0 to the key in R3
            
            LD  R1, INV1_OFFST  ; load the invalid below 33 offset into R1
            ADD R4, R5, R1      ; check if the character is invalid below ASCII 32
            BRzp #2             ; if character is valid in this regard, jump to next check
            LD  R0, ASCII_?     ; change char stored in R0 to '?'
            BRnzp   CHST
            
            LD  R1, INV2_OFFST  ; load the invalid above 126 offset into R1
            ADD R4, R5, R1      ; check if the character is invalid above ASCII 126
            BRn #2             ; if character is valid in this regard, jump to next check
            LD  R0, ASCII_?     ; change char stored in R0 to '?'
            BRnzp CHST
            
            LD  R1, NEW_OFFSET  ; load the newline offset into R1
            ADD R4, R5, R1      ; check if the character entered is a newline by adding it with offset
            BRz     DONE        ; break out of the loop if the character is a newline
            
            ADD R0, R0, R3      ; add the encryption offset to the inputted valid character 
            
CHST        STR R0, R2, #0      ; ...store the character...
            ADD R2, R2, #1      ; ...increment the address...
            BRnzp LOOP          ; ...loop back.

            ; Print the result.
; LONG_DONE   LD  R0  ASCII_NEWL
;             OUT
DONE        AND R4, R4, #0      ; Get the null char.
            STR R4, R2, #0      ; Store the null char.
            LEA R0, RES_PROMPT  ; Load the prompt.
            PUTS                ; Print the prompt.
            LEA R0, STRING      ; Load the string.
            PUTS                ; Print the string.
            HALT                ; Halt.

            ; constants:

NEW_OFFSET  .FILL x-0A
INT_OFFSET  .FILL x-30
INV1_OFFST  .FILL #-32
INV2_OFFST  .FILL #-125
ASCII_?     .FILL x3F
STR_SIZE    .FILL x32
ASCII_NEWL  .FILL x0A

KEY_PROMPT  .STRINGZ "Encryption key (0-9): "
STR_PROMPT  .STRINGZ "Unencrypted string: "
RES_PROMPT  .STRINGZ "Encrypted string: "
STRING      .BLKW #32
END_STRING  .BLKW #1
            .END
