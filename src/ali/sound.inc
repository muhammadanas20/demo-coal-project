;===============================================================
; sound.inc - Owner: Ali Raza (25K-0827)
; Procedures: PlayFireSound, PlayExplosionSound, PlayLifeLostSound,
;             PlayGameOverSound, PlaySonarSweep
; Uses the Windows Beep(frequency, duration) API from kernel32.
; In-game sounds are kept very short (<= 30 ms) so play never stutters.
;===============================================================

Beep PROTO, dwFreq:DWORD, dwDuration:DWORD    ; kernel32 Beep (not in Irvine headers)

;---------------------------------------------------------------
; mPlaySound: plays one tone. Beep changes EAX/ECX/EDX, so all
; registers are saved with PUSHAD / POPAD.
;---------------------------------------------------------------
mPlaySound MACRO freq, dur
    pushad
    INVOKE Beep, freq, dur
    popad
ENDM

.code
PlayFireSound PROC
    mPlaySound 1200, 20          ; high and short
    ret
PlayFireSound ENDP

PlayExplosionSound PROC
    mPlaySound 150, 30           ; low
    ret
PlayExplosionSound ENDP

PlayLifeLostSound PROC
    mPlaySound 300, 120
    mPlaySound 200, 180
    ret
PlayLifeLostSound ENDP

PlayGameOverSound PROC USES ebx ecx
    mov  ebx, 600
    mov  ecx, 4
L1: mPlaySound ebx, 150      ; 600, 500, 400, 300 Hz (falling)
    sub  ebx, 100
    loop L1
    ret
PlayGameOverSound ENDP

PlaySonarSweep PROC USES ebx ecx
    mov  ebx, 400
    mov  ecx, 5
L1: mPlaySound ebx, 50       ; 400, 500, 600, 700, 800 Hz (rising)
    add  ebx, 100
    loop L1
    ret
PlaySonarSweep ENDP
