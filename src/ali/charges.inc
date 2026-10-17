;===============================================================
; charges.inc - Owner: Ali Raza (25K-0827)
; Procedures: SpawnCharge, UpdateCharges
; Depth charges appear on TOP_ROW at a random column, fall one row
; every fallDelay frames, and vanish at BOTTOM_ROW. About 1 in 5 is a
; hidden mine (hidden = 1): it still moves and hits, Alyan just does
; not draw it except during sonar.
;===============================================================

.code
;---------------------------------------------------------------
; SpawnCharge: called every frame while playing
;---------------------------------------------------------------
SpawnCharge PROC USES eax ecx esi
    dec  spawnTimer
    jnz  Done                    ; not time yet
    mov  al, spawnDelay
    mov  spawnTimer, al

    mov  esi, OFFSET Charges     ; find a free slot
    mov  ecx, MAX_CHARGE
FindSlot:
    cmp  (GameObject PTR [esi]).active, 0
    je   Found
    add  esi, TYPE GameObject
    loop FindSlot
    jmp  Done                    ; all slots in use

Found:
    mov  eax, MAX_COL - MIN_COL + 1    ; 78 possible columns
    call Rand                          ; 0..77
    add  al, MIN_COL                   ; 1..78 (never col 0 or 79)
    mov  (GameObject PTR [esi]).xPos, al
    mov  (GameObject PTR [esi]).yPos, TOP_ROW
    mov  (GameObject PTR [esi]).active, 1
    mov  (GameObject PTR [esi]).hidden, 0
    mov  eax, 5
    call Rand                          ; 0..4
    cmp  eax, 0                        ; 1 chance in 5 = 20 %
    jne  Done
    mov  (GameObject PTR [esi]).hidden, 1
Done:
    ret
SpawnCharge ENDP

;---------------------------------------------------------------
; UpdateCharges: moves every charge down one row when fallTimer expires
;---------------------------------------------------------------
UpdateCharges PROC USES eax ecx esi
    dec  fallTimer
    jnz  Done                    ; charges move every fallDelay frames
    mov  al, fallDelay
    mov  fallTimer, al
    mov  esi, OFFSET Charges
    mov  ecx, MAX_CHARGE
L1:
    cmp  (GameObject PTR [esi]).active, 0
    je   NextC
    inc  (GameObject PTR [esi]).yPos
    cmp  (GameObject PTR [esi]).yPos, BOTTOM_ROW
    jb   NextC
    mov  (GameObject PTR [esi]).active, 0    ; reached the sea bed
NextC:
    add  esi, TYPE GameObject
    loop L1
Done:
    ret
UpdateCharges ENDP
