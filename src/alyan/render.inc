;===============================================================
; render.inc - Owner: Syed Alyan Hussain (25K-0648)
; Procedures: InitSystem, PutCell, PutString, ClearBuffer,
;             InvalidateScreen, DrawBorders, BuildFrame, Render
; Double-buffered 80 x 25 console rendering:
;   ScreenBuf = frame being built this tick
;   PrevBuf   = what is already on the console
; Only cells that differ between the two are written, so the
; screen does not flicker.
;===============================================================

.data
titleStr    BYTE "Depth Charge - Submarine Defense with Sonar Detection", 0
cursorInfo  CONSOLE_CURSOR_INFO <25, 0>     ; dwSize, bVisible = 0 (hide cursor)

.code
;---------------------------------------------------------------
; InitSystem: one-time setup (called by main)
;   seeds the random generator, sets the window title, hides cursor
;---------------------------------------------------------------
InitSystem PROC USES eax
    call SeedRandom                          ; Ali: random.inc
    INVOKE SetConsoleTitle, ADDR titleStr
    INVOKE GetStdHandle, STD_OUTPUT_HANDLE
    INVOKE SetConsoleCursorInfo, eax, ADDR cursorInfo
    ret
InitSystem ENDP

;---------------------------------------------------------------
; PutCell: ScreenBuf[row*80 + col] = AX      (memory only)
; Receives: DH = row, DL = col, AL = char, AH = colour
;---------------------------------------------------------------
PutCell PROC USES ebx ecx
    movzx ebx, dh
    imul  ebx, ebx, SCREEN_W     ; ebx = row * 80
    movzx ecx, dl
    add   ebx, ecx               ; ebx = row*80 + col (cell index)
    mov   ScreenBuf[ebx*2], ax   ; *2 because each cell is a WORD
    ret
PutCell ENDP

;---------------------------------------------------------------
; PutString: writes a 0-terminated string into ScreenBuf
; Receives: ESI = offset of string, DH = row, DL = col, AH = colour
; DL is restored on return (the caller's position is unchanged).
;---------------------------------------------------------------
PutString PROC USES eax edx esi
NextChar:
    mov  al, [esi]
    cmp  al, 0
    je   Done
    call PutCell
    inc  dl
    inc  esi
    jmp  NextChar
Done:
    ret
PutString ENDP

;---------------------------------------------------------------
; ClearBuffer: fills ScreenBuf with blanks (space, light grey)
;---------------------------------------------------------------
ClearBuffer PROC USES eax ecx edi
    cld
    mov  ax, (ATTR_TEXT SHL 8) OR ' '    ; 0720h
    mov  edi, OFFSET ScreenBuf
    mov  ecx, SCREEN_W * SCREEN_H
    rep  stosw
    ret
ClearBuffer ENDP

;---------------------------------------------------------------
; InvalidateScreen: fills PrevBuf with 0FFFFh so the next Render
; redraws every cell (call after Clrscr / menus / new game)
;---------------------------------------------------------------
InvalidateScreen PROC USES eax ecx edi
    cld
    mov  ax, 0FFFFh
    mov  edi, OFFSET PrevBuf
    mov  ecx, SCREEN_W * SCREEN_H
    rep  stosw
    ret
InvalidateScreen ENDP

;---------------------------------------------------------------
; DrawBorders: '=' lines on row 1 and row 24 (cols 0..78)
; Column 79 is skipped: writing the bottom-right cell scrolls.
;---------------------------------------------------------------
DrawBorders PROC USES eax edx
    mov  al, CH_BORDER
    mov  ah, ATTR_BORDER
    mov  dl, 0
B1:
    mov  dh, 1
    call PutCell
    mov  dh, BOTTOM_ROW
    call PutCell
    inc  dl
    cmp  dl, SCREEN_W - 1
    jb   B1
    ret
DrawBorders ENDP

;---------------------------------------------------------------
; BuildFrame: paints the whole next frame into ScreenBuf
;   clear -> status bar -> borders -> sub -> torpedoes
;   -> charges/mines -> explosions
;---------------------------------------------------------------
BuildFrame PROC USES eax ecx edx esi
    call ClearBuffer
    call DrawHUD                             ; hud.inc
    call DrawBorders

    ; submarine
    mov  dh, SUB_ROW
    mov  dl, subX
    mov  al, CH_SUB
    mov  ah, ATTR_SUB
    call PutCell

    ; torpedoes (Anas)
    mov  esi, OFFSET Torpedoes
    mov  ecx, MAX_TORP
T1:
    cmp  (GameObject PTR [esi]).active, 0
    je   TN
    mov  dh, (GameObject PTR [esi]).yPos
    mov  dl, (GameObject PTR [esi]).xPos
    mov  al, CH_TORP
    mov  ah, ATTR_TORP
    call PutCell
TN:
    add  esi, TYPE GameObject
    loop T1

    ; charges (Ali) and hidden mines
    mov  esi, OFFSET Charges
    mov  ecx, MAX_CHARGE
C1:
    cmp  (GameObject PTR [esi]).active, 0
    je   CN
    mov  dh, (GameObject PTR [esi]).yPos
    mov  dl, (GameObject PTR [esi]).xPos
    cmp  (GameObject PTR [esi]).hidden, 0
    je   Visible
    cmp  gameState, ST_SONAR
    jne  CN                                  ; hidden and no sonar: NOT drawn
    mov  al, CH_MINE                         ; (it still exists in memory)
    mov  ah, ATTR_MINE
    jmp  DrawIt
Visible:
    mov  al, CH_CHARGE
    mov  ah, ATTR_CHARGE
DrawIt:
    call PutCell
CN:
    add  esi, TYPE GameObject
    loop C1

    call DrawExplosions                      ; explosion.inc
    ret
BuildFrame ENDP

;---------------------------------------------------------------
; Render: writes only the cells that changed since the last frame
; ebx = cell index 0..1998. ebx is saved around each console call
; because the console routines are allowed to change registers.
;---------------------------------------------------------------
Render PROC USES eax ebx ecx edx esi edi
    xor  ebx, ebx
NextCell:
    cmp  ebx, SCREEN_W * SCREEN_H - 1        ; never touch the bottom-right
    jae  CopyFrame                           ; cell (it would scroll)
    mov  ax, ScreenBuf[ebx*2]
    cmp  ax, PrevBuf[ebx*2]
    je   Skip                                ; unchanged -> no console call

    ; row = index / 80, col = index mod 80
    push ebx
    mov  eax, ebx
    xor  edx, edx
    mov  ecx, SCREEN_W
    div  ecx                                 ; EAX = row, EDX = col
    mov  dh, al
    call Gotoxy                              ; DH = row, DL = col
    pop  ebx

    push ebx
    movzx eax, BYTE PTR ScreenBuf[ebx*2 + 1] ; high byte = colour
    call SetTextColor
    pop  ebx

    push ebx
    mov  al, BYTE PTR ScreenBuf[ebx*2]       ; low byte = character
    call WriteChar
    pop  ebx
Skip:
    inc  ebx
    jmp  NextCell

CopyFrame:
    cld                                      ; PrevBuf = ScreenBuf
    mov  esi, OFFSET ScreenBuf
    mov  edi, OFFSET PrevBuf
    mov  ecx, (SCREEN_W * SCREEN_H) / 2      ; 2 WORDs per DWORD
    rep  movsd
    ret
Render ENDP
