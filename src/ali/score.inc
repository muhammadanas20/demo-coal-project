;===============================================================
; score.inc - score, lives, levels         Owner: Ali Raza (25K-0827)
; Procedures: AddScore, ApplyLevel, LoseLife
; Level = score / 200 + 1 (maximum MAX_LEVEL = 9).
; Higher level = smaller delay numbers = faster game.
;===============================================================

.data
; lookup tables indexed by (level - 1)
FallDelayTable  BYTE 6, 5, 5, 4, 4, 3, 3, 2, 2
SpawnDelayTable BYTE 14, 12, 11, 10, 9, 8, 7, 6, 5
MAX_LEVEL = LENGTHOF FallDelayTable

.code
;---------------------------------------------------------------
; AddScore: EAX = points (10 for a visible charge, 25 for a mine)
; Adds the points and recalculates the level.
;---------------------------------------------------------------
AddScore PROC USES eax ecx edx
    add  score, eax
    mov  eax, score
    xor  edx, edx                ; EDX must be 0 before DIV
    mov  ecx, 200
    div  ecx                     ; EAX = score / 200
    inc  eax                     ; level = score/200 + 1
    cmp  eax, MAX_LEVEL
    jbe  LevelOk
    mov  eax, MAX_LEVEL
LevelOk:
    cmp  al, level
    je   Done                    ; level unchanged
    mov  level, al
    call ApplyLevel
Done:
    ret
AddScore ENDP

;---------------------------------------------------------------
; ApplyLevel: copies the delays for the current level
; (called by InitGame and by AddScore when the level changes)
;---------------------------------------------------------------
ApplyLevel PROC USES eax ebx
    movzx ebx, level
    dec  ebx                     ; table index 0..8
    mov  al, FallDelayTable[ebx]
    mov  fallDelay, al
    mov  al, SpawnDelayTable[ebx]
    mov  spawnDelay, al
    ret
ApplyLevel ENDP

;---------------------------------------------------------------
; LoseLife: called by CheckCollisions when a charge hits the sub
; Plays the sound, removes a life, and ends the game at zero lives.
;---------------------------------------------------------------
LoseLife PROC USES eax
    call PlayLifeLostSound       ; sound.inc
    dec  lives
    jnz  StillAlive
    mov  gameState, ST_GAMEOVER
    ret
StillAlive:
    mov  subX, 40                ; respawn in the middle
    ret
LoseLife ENDP
