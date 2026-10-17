# Final report section — Ali Raza (25K-0827)

## Responsibilities and implementation

I implemented the random-number generator, sound, scoring and levels, falling depth charges (including hidden mines), the menus and screens, and high-score saving.

- **Random numbers** (`src/ali/random.inc`): a linear congruential generator. `SeedRandom` sets the seed at start-up and `Rand` advances it (`seed = seed * a + c`), returning the value in EAX; `div` reduces the value to a range.
- **Charges and mines** (`src/ali/charges.inc`): `UpdateCharges` moves each charge down by one row every `fallDelay` frames. `SpawnCharge` creates a new charge every `spawnDelay` frames. One charge in five is a hidden mine: it still moves and can hit the submarine, but it is only drawn during sonar. A hit on a normal charge is worth 10 points and a hit on a mine is worth 25.
- **Score and levels** (`src/ali/score.inc`): `AddScore` adds points (every award is a multiple of 5) and updates the level as level = score / 200 + 1. The fall and spawn delay tables have 9 entries, so the level is capped at 9. `LoseLife` removes a life, respawns the submarine at column 40, and sets `ST_GAMEOVER` when lives reach zero.
- **Sound** (`src/ali/sound.inc`): short kernel32 `Beep` tones for fire, explosion, life lost, sonar sweep, and game over.
- **Screens** (`src/ali/screens.inc`): the title menu (1 start, 2 instructions, 3 or ESC exit), the instructions page, and the game-over screen with the final score and a "NEW HIGH SCORE" line when a record is set. `PrintAt` writes straight to the console, so it does not use the game buffer.
- **High score** (`src/ali/highscore.inc`): `highscore.dat` holds one 4-byte DWORD. `LoadHighScore` reads it at start-up; `SaveHighScore` writes it only when the score is higher than the stored value. A missing file is treated as a score of 0.

## Design decisions

- Hidden mines are a data flag (`hidden`) set when a charge is created. Collision code treats them like normal charges, and only the renderer decides whether to show them. This keeps the rule in one place.
- The level tables are small `BYTE` arrays indexed by `level - 1`, which makes the difficulty easy to tune.
- The screens save and restore registers with `USES`, and `PrintAt` preserves DX across `SetTextColor`, so callers do not have to depend on register values from that call.

## Testing evidence

- **Assembler:** JWasm (Baron-von-Riedesel/JWasm, built from source on Linux) with `-coff`, Win32 target: 0 errors, 0 warnings. All undefined symbols are Irvine32 or kernel32 routines.
- **Logic test (Linux, virtual clock, test-only harness):** 2 games started, 1 game-over, ESC returned to the menu, two pauses, and 27 sonar activations. Invariant checks: 0 violations for the level rule, score multiples of 5, frozen world during pause and sonar, hidden-mine draw rule, and high-score file contents (value 10 written and read back). Limitation: the highest level reached was 1, so the level-up path has not been exercised yet.
- **Still to do on Windows:** run the game in Visual Studio 2022 (Debug | Win32), check the sounds, check the game-over screen and `highscore.dat`, and record screenshots for the final report.

## Integration and reflection

I found and fixed a register-clobbering bug in `PrintAt` while testing: the cursor position was lost when `SetTextColor` ran. I also learned that a menu or game-over loop needs a clear rule for who reads the keyboard. In testing, the game's input handler drained scripted keys before the game-over screen's wait loop could read them, so the test harness now feeds keys only during timed waits.

## AI-use / other-source disclosure

This section was drafted with AI assistance (Arena.ai Agent Mode). The Irvine32 library and the course implementation guide are used as provided. The Linux test harness and JWasm build are not part of the submitted project.
