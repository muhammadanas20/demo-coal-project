;===============================================================
; random.inc - Owner: Ali Raza (25K-0827)
; Procedures: SeedRandom, Rand
; Linear congruential generator: seed = seed * 1103515245 + 12345
;===============================================================

.data
seed        DWORD 0

.code
;---------------------------------------------------------------
; SeedRandom: seed from the clock so every run is different
; (called by Alyan's InitSystem)
;---------------------------------------------------------------
SeedRandom PROC USES eax
    call GetMseconds
    mov  seed, eax
    ret
SeedRandom ENDP

;---------------------------------------------------------------
; Rand: Receives EAX = range (must be > 0)
;       Returns  EAX = random number 0 .. range-1
;---------------------------------------------------------------
Rand PROC USES ebx ecx edx
    mov  ebx, eax                ; save range
    mov  eax, seed
    mov  ecx, 1103515245         ; LCG multiplier
    mul  ecx                     ; EDX:EAX = seed * a
    add  eax, 12345              ; + c  (mod 2^32 happens automatically)
    mov  seed, eax
    shr  eax, 16                 ; upper bits are the most random
    xor  edx, edx
    div  ebx                     ; EDX = EAX mod range
    mov  eax, edx
    ret
Rand ENDP
