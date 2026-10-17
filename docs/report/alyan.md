# Final report section — Syed Alyan Hussain (25K-0648)

## Responsibilities and implementation

I implemented rendering, the HUD, explosions, and sonar.

- **Rendering** (`src/alyan/render.inc`): the game draws into `ScreenBuf`. `BuildFrame` fills it with the borders, the HUD, the submarine, torpedoes, charges, and explosions. `Render` compares `ScreenBuf` with `PrevBuf` cell by cell and writes only the cells that changed, so the screen does not flicker. `InvalidateScreen` sets `PrevBuf` to an impossible value to force a full redraw, which is used after a new game starts. The bottom-right cell is skipped so the console does not scroll.
- **HUD** (`src/alyan/hud.inc`): score, lives, level, and sonar status on row 0, plus a centred "PAUSED" text while paused.
- **Explosions** (`src/alyan/explosion.inc`): up to 5 explosions at a time. Each lasts 5 frames, shown as `. + * # @`. The timer in `ExplTimer` counts down, and the frame index is `EXPL_FRAMES - timer`.
- **Sonar** (`src/alyan/sonar.inc`): SPACE starts sonar. The game freezes for `SONAR_MS` (2000 ms), hidden mines are drawn as `!`, and then the game returns to play. A `SONAR_COOLDOWN_MS` (10000 ms) cooldown starts after each sonar, and SPACE is ignored during cooldown.

## Design decisions

- Drawing only changed cells keeps the 80 ms frame loop smooth on a classic console.
- Sonar is a state (`ST_SONAR`) rather than a flag, so the game loop skips all updates while it is active. This matches how pause is handled.
- Explosion lifetime is stored as a countdown per slot, which avoids extra arrays for frame numbers.

## Testing evidence

- **Assembler:** JWasm `-coff`, Win32 target: 0 errors, 0 warnings.
- **Logic test (Linux, virtual clock, test-only harness):** the console output matched the back buffer on every frame; no out-of-range values; the world was frozen during pause and sonar; the sonar duration and cooldown were correct across 27 activations; 42 explosion frames were seen; hidden mines were drawn only during sonar. 0 violations. One frame-timing sample was off by a few ms (informational).
- **Still to do on Windows:** check colours, flicker, and sonar timing in the Visual Studio console; add screenshots for the final report.

## Integration and reflection

The main integration issue was the cursor position in `PrintAt`, which belongs to Ali's screen code. I also confirmed that `Render` must not depend on register values from console calls. Testing showed that sonar and pause must both stop the world updates, and the shared `gameState` variable makes that simple.

## AI-use / other-source disclosure

This section was drafted with AI assistance (Arena.ai Agent Mode). The Irvine32 library and the course implementation guide are used as provided. The Linux test harness and JWasm build are not part of the submitted project.
