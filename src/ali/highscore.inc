;===============================================================
; highscore.inc - file handling            Owner: Ali Raza (25K-0827)
; Procedures: LoadHighScore, SaveHighScore
; The best score is stored as one DWORD (4 bytes) in highscore.dat.
; highscore.dat is created at runtime and is ignored by Git.
;===============================================================

.data
hsFileName  BYTE "highscore.dat", 0
hsHandle    DWORD ?

.code
;---------------------------------------------------------------
; LoadHighScore: called once by main. A missing file (first run)
; means the high score is 0.
;---------------------------------------------------------------
LoadHighScore PROC USES eax ecx edx
    mov  edx, OFFSET hsFileName
    call OpenInputFile                 ; EAX = handle or INVALID_HANDLE_VALUE
    cmp  eax, INVALID_HANDLE_VALUE
    je   NoFile                        ; first run: no file yet
    mov  hsHandle, eax
    mov  edx, OFFSET highScore         ; read the 4 bytes straight into highScore
    mov  ecx, SIZEOF highScore
    call ReadFromFile                  ; EAX = handle (still)
    mov  eax, hsHandle
    call CloseFile
    ret
NoFile:
    mov  highScore, 0
    ret
LoadHighScore ENDP

;---------------------------------------------------------------
; SaveHighScore: writes the file only when the score beat the record
;---------------------------------------------------------------
SaveHighScore PROC USES eax ecx edx
    mov  eax, score
    cmp  eax, highScore
    jbe  Done                          ; not a new record
    mov  highScore, eax
    mov  edx, OFFSET hsFileName
    call CreateOutputFile              ; creates or overwrites the file
    cmp  eax, INVALID_HANDLE_VALUE
    je   Done
    mov  hsHandle, eax
    mov  edx, OFFSET highScore
    mov  ecx, SIZEOF highScore
    call WriteToFile                   ; EAX = handle
    mov  eax, hsHandle
    call CloseFile
Done:
    ret
SaveHighScore ENDP
