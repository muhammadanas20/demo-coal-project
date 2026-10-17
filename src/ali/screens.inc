;===============================================================
; screens.inc - Owner: Ali Raza (25K-0827)
; Procedures: PrintAt, WaitAnyKey, ShowMenu, ShowInstructions,
;             GameOverScreen
; These screens draw straight to the console (not through the
; game's ScreenBuf). Before every PrintAt, EAX holds the colour.
;===============================================================

.data
strTitle    BYTE "D E P T H   C H A R G E", 0
strOpt1     BYTE "1 - Start game", 0
strOpt2     BYTE "2 - Instructions", 0
strOpt3     BYTE "3 - Exit", 0
strHigh     BYTE "High score: ", 0

strHowTo    BYTE "HOW TO PLAY", 0
strInst1    BYTE "A / Left arrow     move the submarine left", 0
strInst2    BYTE "D / Right arrow    move the submarine right", 0
strInst3    BYTE "W                  fire a torpedo", 0
strInst4    BYTE "SPACE              sonar: reveals hidden mines for 2 seconds", 0
strInst5    BYTE "P                  pause / resume        ESC  back to menu", 0
strInst6    BYTE "Torpedo hits a depth charge = 10 points, a hidden mine = 25 points", 0
strInst7    BYTE "Every 200 points = next level (faster).  You have 3 lives.", 0

strOver     BYTE "G A M E   O V E R", 0
strFinal    BYTE "Final score: ", 0
strNewHigh  BYTE "*** NEW HIGH SCORE! ***", 0
strAnyKey   BYTE "Press any key to continue", 0

.code
;---------------------------------------------------------------
; PrintAt: EAX = colour, DH = row, DL = col, ESI = 0-terminated string
;---------------------------------------------------------------
PrintAt PROC USES edx
    push edx                     ; keep DH/DL: SetTextColor must not be
    call SetTextColor            ; relied on to leave DX alone
    pop  edx
    call Gotoxy                  ; DH = row, DL = col
    mov  edx, esi
    call WriteString
    ret
PrintAt ENDP

;---------------------------------------------------------------
; WaitAnyKey: waits for a key without using 100 % CPU
;---------------------------------------------------------------
WaitAnyKey PROC USES eax
L1:
    mov  eax, 30
    call Delay
    call ReadKey
    jz   L1
    ret
WaitAnyKey ENDP

;---------------------------------------------------------------
; ShowInstructions: the how-to-play screen
;---------------------------------------------------------------
ShowInstructions PROC USES eax edx esi
    mov  eax, ATTR_TEXT
    call SetTextColor
    call Clrscr

    mov  eax, ATTR_HUD
    mov  dh, 3
    mov  dl, 34
    mov  esi, OFFSET strHowTo
    call PrintAt

    mov  eax, ATTR_TEXT
    mov  dh, 6
    mov  dl, 8
    mov  esi, OFFSET strInst1
    call PrintAt

    mov  eax, ATTR_TEXT
    mov  dh, 7
    mov  dl, 8
    mov  esi, OFFSET strInst2
    call PrintAt

    mov  eax, ATTR_TEXT
    mov  dh, 8
    mov  dl, 8
    mov  esi, OFFSET strInst3
    call PrintAt

    mov  eax, ATTR_TEXT
    mov  dh, 9
    mov  dl, 8
    mov  esi, OFFSET strInst4
    call PrintAt

    mov  eax, ATTR_TEXT
    mov  dh, 10
    mov  dl, 8
    mov  esi, OFFSET strInst5
    call PrintAt

    mov  eax, ATTR_TEXT
    mov  dh, 13
    mov  dl, 8
    mov  esi, OFFSET strInst6
    call PrintAt

    mov  eax, ATTR_TEXT
    mov  dh, 14
    mov  dl, 8
    mov  esi, OFFSET strInst7
    call PrintAt

    mov  eax, ATTR_TEXT
    mov  dh, 18
    mov  dl, 27
    mov  esi, OFFSET strAnyKey
    call PrintAt

    call WaitAnyKey
    ret
ShowInstructions ENDP

;---------------------------------------------------------------
; ShowMenu: sets gameState to ST_PLAYING (1 = start) or ST_EXIT
; (3 or ESC = exit). Option 2 shows the instructions and returns here.
;---------------------------------------------------------------
ShowMenu PROC USES eax edx esi
    call ReadKeyFlush            ; forget keys pressed during the game
Redraw:
    mov  eax, ATTR_TEXT
    call SetTextColor
    call Clrscr

    mov  eax, ATTR_HUD
    mov  dh, 6
    mov  dl, 26
    mov  esi, OFFSET strTitle
    call PrintAt

    mov  eax, ATTR_TEXT
    mov  dh, 10
    mov  dl, 31
    mov  esi, OFFSET strOpt1
    call PrintAt

    mov  eax, ATTR_TEXT
    mov  dh, 11
    mov  dl, 31
    mov  esi, OFFSET strOpt2
    call PrintAt

    mov  eax, ATTR_TEXT
    mov  dh, 12
    mov  dl, 31
    mov  esi, OFFSET strOpt3
    call PrintAt

    mov  eax, ATTR_TEXT
    mov  dh, 15
    mov  dl, 31
    mov  esi, OFFSET strHigh
    call PrintAt
    mov  eax, highScore          ; printed right after the label
    call WriteDec

WaitKey:
    mov  eax, 30
    call Delay                   ; keeps the CPU free while waiting
    call ReadKey
    jz   WaitKey
    cmp  al, '1'
    je   StartGame
    cmp  al, '2'
    je   DoInstr
    cmp  al, '3'
    je   Quit
    cmp  al, 1Bh                 ; ESC also exits from the menu
    je   Quit
    jmp  WaitKey

StartGame:
    mov  gameState, ST_PLAYING
    ret
DoInstr:
    call ShowInstructions
    jmp  Redraw
Quit:
    mov  gameState, ST_EXIT
    ret
ShowMenu ENDP

;---------------------------------------------------------------
; GameOverScreen: plays the sound, saves a new high score, shows
; the final score (and NEW HIGH SCORE if it was a record).
;---------------------------------------------------------------
GameOverScreen PROC USES eax edx esi
    call PlayGameOverSound
    call SaveHighScore           ; updates highScore if it was beaten

    mov  eax, ATTR_TEXT
    call SetTextColor
    call Clrscr

    mov  eax, lightRed + (black * 16)
    mov  dh, 8
    mov  dl, 33
    mov  esi, OFFSET strOver
    call PrintAt

    mov  eax, ATTR_TEXT
    mov  dh, 11
    mov  dl, 30
    mov  esi, OFFSET strFinal
    call PrintAt
    mov  eax, score              ; printed right after the label
    call WriteDec

    mov  eax, score
    cmp  eax, 0
    je   NoRecord
    cmp  eax, highScore          ; equal -> this score is the record
    jne  NoRecord
    mov  eax, ATTR_HUD
    mov  dh, 13
    mov  dl, 27
    mov  esi, OFFSET strNewHigh
    call PrintAt
NoRecord:
    mov  eax, 700                ; ignore keys still held from play
    call Delay
    call ReadKeyFlush
    mov  eax, ATTR_TEXT
    mov  dh, 16
    mov  dl, 27
    mov  esi, OFFSET strAnyKey
    call PrintAt
    call WaitAnyKey
    ret
GameOverScreen ENDP
