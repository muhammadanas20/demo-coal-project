;===============================================================
; explosion.inc - Owner: Syed Alyan Hussain (25K-0648)
; Procedures: StartExplosion, UpdateExplosions, DrawExplosions
; Explosion slots are in the shared Explosions array; their
; remaining lifetime (EXPL_FRAMES .. 1) is in ExplTimer.
;===============================================================

.data
; animation frames in time order: first frame '.', last frame '@'
ExplChars   BYTE '.', '+', '*', '#', '@'
ExplColors  BYTE yellow + (black*16), yellow + (black*16), lightRed + (black*16), lightRed + (black*16), white + (black*16)

.code
;---------------------------------------------------------------
; StartExplosion: AL = column, AH = row
; Uses the first free slot. If all slots are busy the effect is skipped.
;---------------------------------------------------------------
StartExplosion PROC USES ebx esi
    mov  esi, OFFSET Explosions
    xor  ebx, ebx                             ; ebx = slot index
Find:
    cmp  ebx, MAX_EXPL
    jae  Done                                 ; no free slot
    cmp  (GameObject PTR [esi + ebx*TYPE GameObject]).active, 0
    je   Found
    inc  ebx
    jmp  Find
Found:
    mov  (GameObject PTR [esi + ebx*TYPE GameObject]).xPos, al
    mov  (GameObject PTR [esi + ebx*TYPE GameObject]).yPos, ah
    mov  (GameObject PTR [esi + ebx*TYPE GameObject]).active, 1
    mov  ExplTimer[ebx], EXPL_FRAMES
Done:
    ret
StartExplosion ENDP

;---------------------------------------------------------------
; UpdateExplosions: count every live explosion down one frame;
; a slot whose timer reaches 0 becomes free.
;---------------------------------------------------------------
UpdateExplosions PROC USES ebx esi
    mov  esi, OFFSET Explosions
    xor  ebx, ebx
L1:
    cmp  (GameObject PTR [esi + ebx*TYPE GameObject]).active, 0
    je   NextE
    dec  ExplTimer[ebx]
    jnz  NextE
    mov  (GameObject PTR [esi + ebx*TYPE GameObject]).active, 0
NextE:
    inc  ebx
    cmp  ebx, MAX_EXPL
    jb   L1
    ret
UpdateExplosions ENDP

;---------------------------------------------------------------
; DrawExplosions: draws each live explosion with the frame that
; matches its age: frame index = EXPL_FRAMES - timer (0..4)
;---------------------------------------------------------------
DrawExplosions PROC USES eax ebx ecx edx esi
    mov  esi, OFFSET Explosions
    xor  ebx, ebx
L1:
    cmp  (GameObject PTR [esi + ebx*TYPE GameObject]).active, 0
    je   NextE
    mov  ecx, EXPL_FRAMES
    movzx eax, ExplTimer[ebx]                 ; 1..5
    sub  ecx, eax                             ; table index 0..4
    mov  al, ExplChars[ecx]
    mov  ah, ExplColors[ecx]
    mov  dh, (GameObject PTR [esi + ebx*TYPE GameObject]).yPos
    mov  dl, (GameObject PTR [esi + ebx*TYPE GameObject]).xPos
    call PutCell
NextE:
    inc  ebx
    cmp  ebx, MAX_EXPL
    jb   L1
    ret
DrawExplosions ENDP
