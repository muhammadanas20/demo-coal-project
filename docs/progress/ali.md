# Weekly progress — Ali Raza (25K-0827)

Update this file before each weekly PR. Record actual work and test results; coordinate module contracts with the team.

| Week | Planned focus | Status | Build/test evidence, blockers, next action |
|---|---|---|---|
| 0 · 5–11 Oct | Clone/build starter; confirm shared contracts | Done (starter) | Starter built with JWasm syntax checks only; Windows build not yet run. |
| 1 · 12–18 Oct | Random numbers, menu, charges, sound, score, high score | Implemented; verified in Linux harness | All procedures in `src/ali/` are implemented and their stub lines are removed from `stubs_ali.inc`. JWasm `-coff` build: 0 errors, 0 warnings. Harness run (see below) passed. **Next:** build and play in Visual Studio on a Lab PC with the real Irvine32 library. |
| 2 · 19–25 Oct | [Use Ali's assigned guide] | Implemented | Covered by the Week 1 work above. |
| 3 · 26 Oct–1 Nov | [Use Ali's assigned guide] | Implemented | Covered by the Week 1 work above. |
| 4 · 2–8 Nov | [Use Ali's assigned guide] | Not started | Integration checks with Anas and Alyan on Windows. |
| 5 · 9–15 Nov | [Use Ali's assigned guide] | Not started | Lab-PC testing of menu, game-over screen, and `highscore.dat`. |
| 6 · 16–22 Nov | Integration, test fixes, report section | Not started | Report section in `docs/report/ali.md`. |

## Week 1 update (17 Oct 2026)

- **Completed:** `src/ali/random.inc` (LCG random numbers: `SeedRandom`, `Rand`), `sound.inc` (kernel32 `Beep` tones), `score.inc` (score, lives, level tables: fall delays 6,5,5,4,4,3,3,2,2 and spawn delays 14,12,11,10,9,8,7,6,5; level = score/200 + 1, capped at 9), `charges.inc` (falling depth charges; 1 in 5 charges is a hidden mine worth 25 points, a normal charge is worth 10), `screens.inc` (`PrintAt`, `WaitAnyKey`, menu with 1/2/3/ESC, instructions, game-over screen), `highscore.inc` (4-byte DWORD in `highscore.dat`, written only on a new record).
- **Bug fixed during testing:** `PrintAt` did not preserve DH/DL across `SetTextColor`; it now saves EDX around that call.
- **Build/test:** JWasm 2.x (built from Baron-von-Riedesel/JWasm) `-coff` for the Win32 target: 0 errors, 0 warnings. Undefined symbols are only Irvine32 and kernel32 names. A separate test harness (not in the repo) ran the game logic on Linux with a virtual clock: 2 games started, 1 game-over, ESC returned to the menu, pause and sonar exercised, and the invariant checks reported 0 violations (render consistency, level rule, score multiple of 5, frozen world during pause/sonar, hidden-mine draw rule, high-score file). The harness did not reach level 2 (max level 1), so the level-up path is not yet runtime-verified.
- **Blocked / risks:** The real Windows Irvine32 build and the sound output have not been run on a Windows machine yet.
- **Next:** Build `Project.sln` (Debug | Win32) on Windows, play through a full game, and check the high-score file and the game-over screen.
