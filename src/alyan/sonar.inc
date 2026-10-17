;===============================================================
; sonar.inc - Owner: Syed Alyan Hussain (25K-0648)
; Procedures: StartSonar, UpdateSonar
; Sonar (SPACE) freezes the game for SONAR_MS (2 s) and draws hidden
; mines as '!'. After that a SONAR_COOLDOWN_MS (10 s) cooldown starts.
;===============================================================

.code
;---------------------------------------------------------------
; StartSonar: called by HandleInput when SPACE is pressed and the
; cooldown has finished. GameLoop then calls only UpdateSonar.
;---------------------------------------------------------------
StartSonar PROC USES eax
    mov  gameState, ST_SONAR       ; freezes all updates in GameLoop
    call GetMseconds
    mov  sonarStart, eax
    mov  sonarPlayed, 0
    ret
StartSonar ENDP

;---------------------------------------------------------------
; UpdateSonar: called every frame while gameState = ST_SONAR
;   first frame: draw the mines and play the sweep sound once
;   after SONAR_MS: back to ST_PLAYING and start the cooldown
;---------------------------------------------------------------
UpdateSonar PROC USES eax
    cmp  sonarPlayed, 0
    jne  CheckTime
    call BuildFrame                ; show the '!' marks first
    call Render
    call PlaySonarSweep            ; Ali: sound.inc (~250 ms, game is frozen)
    mov  sonarPlayed, 1

CheckTime:
    call GetMseconds
    sub  eax, sonarStart           ; elapsed = now - start
    cmp  eax, SONAR_MS
    jb   StillOn
    mov  gameState, ST_PLAYING     ; back to normal play
    call GetMseconds
    add  eax, SONAR_COOLDOWN_MS    ; cooldown ends at now + 10 s
    mov  sonarReadyAt, eax
StillOn:
    ret
UpdateSonar ENDP
