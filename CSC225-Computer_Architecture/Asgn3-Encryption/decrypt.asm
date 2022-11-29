; Decrypts a string using a Caesar cipher.
; CSC 225, Assignment 3
; Given code, Spring '22

            .ORIG x3000
            
            ;
            ; R0: inputted/current char
            ; R1: ASCII validation offset
            ; R2; mem address iterator for writing a string
            ; R3: current encryption key
            ; R4: used as result of checking character against offset
            ; R5: (decryption loop only) holds memory address of current character from input
            ;
            
            ; clear string memory locations for reusablilty
            LD  R2, STR_SIZE    ; create a counter of size length of string in memory
            AND R1, R1, #0      ; put null character ASCII value in R1
            LEA R0, STRING      ; clear input string memory locations
CLR_STRING  STR R1, R0, #0      ; ...
            ADD R0, R0, #1      ; increment memory address iterator
            ADD R2, R2, #-1     ; decrement counter
            BRp CLR_STRING
            
            LD  R2, STR_SIZE    ; create a counter of size length of string in memory
            AND R1, R1, #0      ; put null character ASCII value in R1
            LEA R0, DECSTR      ; clear input string memory locations
CLR_DECSTR  STR R1, R0, #0      ; ...
            ADD R0, R0, #1      ; increment memory address iterator
            ADD R2, R2, #-1     ; decrement counter
            BRp CLR_DECSTR 
            
            LEA R0, STR_PROMPT  ; Load the prompt.
            LEA R2, STRING      ; Load the address of the string into R2.
            PUTS                ; Print the prompt.

            
INPLOOP     LD  R1, NEW_OFFSET  ; load the newline offset into R1
            
            GETC                ; While the user types characters...
            OUT                 ; ...echo the character...
            
            ADD R4, R0, R1      ; check if the character entered is a newline by adding it with offset
            BRz     INPDONE     ; break out of the loop if the character is a newline
            
            STR R0, R2, #0      ; store character in R0 into mem adress stored in R2
            ADD R2, R2, #1      ; increment memory address for next character
            
            ; ADD R4, R5, R6      ; check to see if iterator has iterated past size 32. (not necessary assumed size<32)
            BRnzp INPLOOP         ; if not, loop back. else, break loop because max string size met
            
            
INPDONE     AND R3, R2, #0      ; clear R3
            ;ADD R3, R3, #-9     ; initialize decryption key counter
            

KEYLOOP     LEA R2, DECSTR      ; load the beginning of the decrypted string into the mem address iterator register

DECLOOP     LD  R5 O2I_OFFSET   ; load offset from output string to input string into R5
            ADD R5, R2, R5      ; set R5 to be the memory address or the current char from input string
            LDR R0, R5, #0      ; load in the current char from memory address stored in R5
            
            BRz DECDONE         ; if current character is null character, break loop
            
            ;       Apply the decryption key, which is in R3, to the character,
            ;       which is in R0. Replace unprintable characters with '?'.
            
            ADD R0, R0, R3      ; ADD char in R0 to the key in R3
            
            
            LD  R1, INV1_OFFST  ; load the invalid below 33 offset into R1
            ADD R4, R0, R1      ; check if the character is invalid below ASCII 32
            BRzp #2             ; if character is valid in this regard, jump to next check
            LD  R0, ASCII_?     ; change char stored in R0 to '?'
            BRnzp CH_STR
            
            LD  R1, INV2_OFFST  ; load the invalid above 126 offset into R1
            ADD R4, R0, R1      ; check if the character is invalid above ASCII 126
            BRnz #2             ; if character is valid in this regard, jump to next check
            LD  R0, ASCII_?     ; change char stored in R0 to '?'
            BRnzp CH_STR

            
CH_STR      STR R0, R2, #0      ; ...store the character...
            ADD R2, R2, #1      ; ...increment the address...
            BRnzp DECLOOP         ; ...loop back.

            ; Print the result.
DECDONE     AND R4, R4, #0      ; Get the null char.
            STR R4, R2, #0      ; Store the null char.
            LEA R0, RES_PROMPT  ; Load the prompt.
            PUTS                ; Print the prompt.
            
            NOT R0, R3          ; put the current encryption key into r0...
            ADD R0, R0 #1       ; ...
            LD  R1, INT_OFFSET  ; convert from int to ASCII code
            ADD R0, R0, R1      ; ...
            OUT                 ; print it
            
            LD R0, ASCII_:      ; load a ':' character into R0
            OUT
            
            LD R0, ASCII_SP     ; load ASCII value for a space character into R0
            OUT                 ; print the space
            
            LEA R0, DECSTR      ; Load the string.
            PUTS                ; Print the string.
            
            LD R0, ASCII_NEWL   ; load a newline character into R0
            OUT
            
            ADD R3, R3 #-1      ; decrement the key offset counter in R3
            ADD R6, R3, #9      
            BRzp KEYLOOP         ; if the key offset counter is greater than 0 break out of the loop
            HALT                ; Halt.

            ; constants:

NEW_OFFSET  .FILL x-0A
INT_OFFSET  .FILL x30
INV1_OFFST  .FILL #-32
INV2_OFFST  .FILL #-127
ASCII_?     .FILL x3F
ASCII_:     .FILL x3A
ASCII_SP    .FILL x20
ASCII_NEWL  .FILL x0A
O2I_OFFSET  .FILL #-33
STR_SIZE    .FILL #32

STR_PROMPT  .STRINGZ "Encrypted string: "
RES_PROMPT  .STRINGZ "Decryption key "
STRING      .BLKW #33
DECSTR      .BLKW #33
            .END
