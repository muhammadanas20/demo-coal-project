; stubs_alyan.inc - Alyan's stub macro (kept for future use).
; All Alyan procedures (InitSystem, InvalidateScreen, BuildFrame, Render,
; UpdateExplosions, StartExplosion, StartSonar, UpdateSonar, DrawHUD,
; DrawExplosions) are now implemented in render.inc, hud.inc,
; explosion.inc and sonar.inc, so no STUB lines are invoked here.
ALYAN_STUB MACRO procName
    procName PROC
        ret
    procName ENDP
ENDM

PURGE ALYAN_STUB
