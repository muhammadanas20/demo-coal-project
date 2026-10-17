# Weekly progress — Syed Alyan Hussain (25K-0648)

Update this file before each weekly PR. Record actual work and test results; coordinate module contracts with the team.

| Week | Planned focus | Status | Build/test evidence, blockers, next action |
|---|---|---|---|
| 0 · 5–11 Oct | Clone/build starter; confirm shared contracts | Done (starter) | Starter built with JWasm syntax checks only; Windows build not yet run. |
| 1 · 12–18 Oct | Rendering, HUD, explosions, sonar | Implemented; verified in Linux harness | All procedures in `src/alyan/` are implemented and their stub lines are removed from `stubs_alyan.inc`. JWasm `-coff` build: 0 errors, 0 warnings. Harness run (see below) passed. **Next:** build and play in Visual Studio on a Lab PC with the real Irvine32 library. |
| 2 · 19–25 Oct | [Use Alyan's assigned guide] | Implemented | Covered by the Week 1 work above. |
| 3 · 26 Oct–1 Nov | [Use Alyan's assigned guide] | Implemented | Covered by the Week 1 work above. |
| 4 · 2–8 Nov | [Use Alyan's assigned guide] | Not started | Integration checks with Anas and Ali on Windows. |
| 5 · 9–15 Nov | [Use Alyan's assigned guide] | Not started | Lab-PC check of flicker-free drawing and sonar timing. |
| 6 · 16–22 Nov | Integration, test fixes, report section | Not started | Report section in `docs/report/alyan.md`. |

## Week 1 update (17 Oct 2026)

- **Completed:** `src/alyan/render.inc` (diff-based drawing: `Render` compares `ScreenBuf` with `PrevBuf` cell by cell and writes only changed cells; `InvalidateScreen` fills `PrevBuf` with 0FFFFh to force a full redraw; the bottom-right cell is skipped to avoid scrolling), `hud.inc` (score, lives, level, sonar status; pause and sonar overlays), `explosion.inc` (5-frame explosion, characters `. + * # @`, max 5 active), `sonar.inc` (SPACE freezes the game for 2000 ms, draws hidden mines as `!`, then a 10000 ms cooldown).
- **Build/test:** JWasm `-coff` for the Win32 target: 0 errors, 0 warnings. A Linux test harness (not in the repo) ran the game with a virtual clock. It checked that the console output matched the back buffer on every frame, that no cell was out of range, that the world stayed frozen during pause and sonar, that the sonar duration (about 2 s) and cooldown (about 10 s) were respected, and that hidden mines were drawn only during sonar. The run had 27 sonar activations, 42 explosion frames, and 0 violations. One frame-timing sample was off by a few ms (informational only).
- **Blocked / risks:** The real Windows console (scrolling and flicker behaviour, colours) has not been checked on a Windows machine yet.
- **Next:** Check rendering and sonar on Windows in Visual Studio; confirm no flicker at the 80 ms frame rate.
