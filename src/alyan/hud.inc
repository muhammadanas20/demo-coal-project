;===============================================================
; hud.inc - Owner: Syed Alyan Hussain (25K-0648)
; Procedures: PutNumber, DrawHUD
; Status bar on row 0:
;   SCORE 00035   LIVES 3   LEVEL 1   SONAR READY
; plus a centred "PAUSED" text while paused.
;===============================================================

.data
strScore    BYTE "SCORE ", 0
strLives    BYTE "LIVES ", 0
strLevel    BYTE "LEVEL ", 0
strSonar    BYTE "SONAR ", 0
strActive   BYTE "ACTIVE", 0
strReady    BYTE "READY ", 0
strPaused   BYTE "PAUSED", 0
numBuf      BYTE 6 DUP(0)               ; 5 digits + terminating 0

.code
;---------------------------------------------------------------
; PutNumber: writes EAX as 5 decimal digits (leading zeros)
; Receives: EAX = value, DH = row, DL = col, BL = colour
; DX and AH are restored by PutString; EBX is preserved.
;---------------------------------------------------------------
PutNumber PROC USES eax ebx ecx edx esi edi
    push edx                      ; keep row/col: DIV overwrites EDX
    mov  edi, OFFSET numBuf + 4   ; fill digits from the right
    mov  esi, 5
    mov  ecx, 10
DigitLoop:
    xor  edx, edx
    div  ecx                      ; EAX = EAX / 10, EDX = last digit
    add  dl, '0'
    mov  [edi], dl
    dec  edi
    dec  esi
    jnz  DigitLoop
    pop  edx                      ; row/col back
    mov  esi, OFFSET numBuf
    mov  ah, bl                   ; colour
    call PutString
    ret
PutNumber ENDP

;---------------------------------------------------------------
; DrawHUD: SCORE, LIVES, LEVEL, SONAR status (and PAUSED)
; Every position and colour is set right before each write,
; because GetMseconds and the console routines change registers.
;---------------------------------------------------------------
DrawHUD PROC USES eax ebx ecx edx esi
    ; SCORE 00035
    mov  dh, HUD_ROW
    mov  dl, 1
    mov  ah, ATTR_HUD
    mov  esi, OFFSET strScore
    call PutString
    mov  dh, HUD_ROW
    mov  dl, 7
    mov  eax, score
    mov  bl, ATTR_HUD
    call PutNumber

    ; LIVES 3
    mov  dh, HUD_ROW
    mov  dl, 17
    mov  ah, ATTR_HUD
    mov  esi, OFFSET strLives
    call PutString
    mov  dh, HUD_ROW
    mov  dl, 23
    mov  al, lives
    add  al, '0'
    mov  ah, ATTR_HUD
    call PutCell

    ; LEVEL 1
    mov  dh, HUD_ROW
    mov  dl, 29
    mov  ah, ATTR_HUD
    mov  esi, OFFSET strLevel
    call PutString
    mov  dh, HUD_ROW
    mov  dl, 35
    mov  al, level
    add  al, '0'
    mov  ah, ATTR_HUD
    call PutCell

    ; SONAR: ACTIVE / READY / countdown
    mov  dh, HUD_ROW
    mov  dl, 41
    mov  ah, ATTR_HUD
    mov  esi, OFFSET strSonar
    call PutString

    mov  dh, HUD_ROW
    mov  dl, 48
    cmp  gameState, ST_SONAR
    je   SonarOn
    call GetMseconds                   ; EAX = now
    cmp  eax, sonarReadyAt
    jae  SonarReady

    ; cooling down: seconds left = ceil((readyAt - now) / 1000)  -> 10..1
    mov  ebx, sonarReadyAt
    sub  ebx, eax
    add  ebx, 999
    mov  eax, ebx
    xor  edx, edx
    mov  ecx, 1000
    div  ecx                           ; EAX = seconds left
    cmp  eax, 10
    jb   OneDigit
    mov  dh, HUD_ROW                   ; "10"
    mov  dl, 48
    mov  al, '1'
    mov  ah, ATTR_HUD
    call PutCell
    mov  dh, HUD_ROW
    mov  dl, 49
    mov  al, '0'
    mov  ah, ATTR_HUD
    call PutCell
    jmp  PauseText
OneDigit:
    add  al, '0'
    mov  dh, HUD_ROW
    mov  dl, 48
    mov  ah, ATTR_HUD
    call PutCell
    jmp  PauseText

SonarOn:
    mov  ah, ATTR_HUD
    mov  esi, OFFSET strActive
    call PutString
    jmp  PauseText

SonarReady:
    mov  dh, HUD_ROW
    mov  dl, 48
    mov  ah, ATTR_HUD
    mov  esi, OFFSET strReady
    call PutString

PauseText:
    cmp  gameState, ST_PAUSED
    jne  Done
    mov  dh, 12
    mov  dl, 37
    mov  ah, ATTR_HUD
    mov  esi, OFFSET strPaused
    call PutString
Done:
    ret
DrawHUD ENDP
