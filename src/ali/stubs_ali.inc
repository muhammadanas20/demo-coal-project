; stubs_ali.inc - Ali's stub macro (kept for future use).
; All Ali procedures (SeedRandom, Rand, ShowMenu, GameOverScreen,
; LoadHighScore, SaveHighScore, SpawnCharge, UpdateCharges, AddScore,
; LoseLife, ApplyLevel, PlayFireSound, PlayExplosionSound, PlaySonarSweep,
; PlayLifeLostSound, PlayGameOverSound) are now implemented in
; random.inc, screens.inc, highscore.inc, charges.inc, score.inc and
; sound.inc, so no STUB lines are invoked here.
ALI_STUB MACRO procName
    procName PROC
        ret
    procName ENDP
ENDM

PURGE ALI_STUB
