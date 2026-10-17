# Run Depth Charge in Visual Studio 2022 (using your own fork)

This guide shows how to get the game running on a Windows PC with Visual Studio 2022 and how to keep your work in your own GitHub fork. Use it on a lab PC or your own Windows machine.

Upstream project: <https://github.com/muhammadanas20/Submarine-Defense-Game-with-Sonar-Detection>

## 1. What you need

| Item | Notes |
|---|---|
| Windows 10 or 11 (64-bit) | The game is 32-bit (Win32). |
| Visual Studio 2022 (17.x) | Workload **Desktop development with C++**. |
| MASM build customization | In the Visual Studio Installer, open the **Individual components** tab and tick **MASM** (named **C++ MASM** in some versions). It is needed to assemble `.asm` files. |
| Irvine32 library | The course library by Kip Irvine. Your lab usually provides it. Put these files in `C:\Irvine`: `Irvine32.inc`, `Irvine32.lib`, `Macros.inc`, `SmallWin.inc`, `VirtualKeys.inc`, and the other `.inc` files that came with it. |
| Git and GitHub account | Git for Windows, and a GitHub account to create the fork. |

> **Irvine32 location:** the project looks for Irvine in `C:\Irvine`. If your lab keeps it elsewhere, see [section 6](#6-if-irvine32-is-in-a-different-folder).

## 2. Fork the project on GitHub

1. Sign in to GitHub and open the upstream project link above.
2. Click **Fork** (top right), keep the default owner (your account), and click **Create fork**.
3. You now have `https://github.com/<your-username>/Submarine-Defense-Game-with-Sonar-Detection`.

Optional, with the GitHub CLI (`gh`):

```powershell
gh repo fork muhammadanas20/Submarine-Defense-Game-with-Sonar-Detection --clone=false
```

## 3. Clone your fork

Open **Git Bash** or **PowerShell** in the folder where you keep projects:

```powershell
git clone https://github.com/<your-username>/Submarine-Defense-Game-with-Sonar-Detection.git
cd Submarine-Defense-Game-with-Sonar-Detection
git remote add upstream https://github.com/muhammadanas20/Submarine-Defense-Game-with-Sonar-Detection.git
git fetch upstream
```

Create your own branch for your work (use your own name in the branch):

```powershell
git checkout -b week1-<yourname>
```

## 4. Open and build in Visual Studio

1. Double-click **`Project.sln`** in the cloned folder (or open Visual Studio and choose **File > Open > Project/Solution**).
2. In the toolbar, set the configuration to **Debug** and the platform to **x86** (Win32). The platform must be **Win32**, not x64.
3. Choose **Build > Build Solution** (Ctrl+Shift+B). The **Output** window should end with `0 error(s)`.
4. Choose **Debug > Start Without Debugging** (Ctrl+F5). The console opens at the main menu:

```text
          D E P T H   C H A R G E

            1 - Start game
            2 - Instructions
            3 - Exit

            High score: 0
```

If the build fails, see [section 7](#7-troubleshooting).

Build output goes to `bin\Win32\Debug\` and `obj\Win32\Debug\` in the solution folder. These folders are git-ignored; do not commit them.

## 5. Controls

| Key | Action |
|---|---|
| **A** or **←** | Move the submarine left |
| **D** or **→** | Move the submarine right |
| **W** | Fire a torpedo |
| **SPACE** | Sonar: shows hidden mines for 2 seconds (10-second cooldown) |
| **P** | Pause / resume |
| **ESC** | Back to the menu |
| **1 / 2 / 3** | Menu: start / instructions / exit |

A depth charge hit costs one life (you start with 3). A torpedo hitting a charge scores 10 points; a hidden mine scores 25. Every 200 points raises the level, which makes charges fall faster.

## 6. If Irvine32 is in a different folder

Do **not** commit changes to `Project.vcxproj` for this. Change the settings only on your own PC:

1. In Solution Explorer, right-click **Project** and choose **Properties**.
2. Set the configuration to **Debug** and the platform to **Win32** (repeat for Release if you use it).
3. **Configuration Properties > MASM > General > Include Paths**: change `C:\Irvine` to your folder.
4. **Configuration Properties > Linker > General > Additional Library Directories**: change `C:\Irvine` to your folder.
5. Click **OK** and build again.

If your lab's Irvine copy has a different library file name, update **Linker > Input > Additional Dependencies** the same way.

## 7. Troubleshooting

| Message | Likely cause and fix |
|---|---|
| `cannot open include file 'Irvine32.inc'` | Irvine32 is not in the include path. Check `C:\Irvine` or section 6. |
| `LNK1104: cannot open file 'Irvine32.lib'` | The library is missing or in another folder. Copy `Irvine32.lib` to `C:\Irvine` or update the linker path. |
| `error A2006: undefined symbol` for a `_Something` name | A procedure is missing or its file is not included. Pull the latest upstream changes (section 8). |
| `MASM` files are not compiled | The **MASM** build customization is not installed. Re-run the Visual Studio Installer and add it, then reopen the project. |
| Build works in Debug but not in Release | Use **Debug | Win32** for testing. Release needs the same paths set. |
| Screen flickers or text is shifted | Use a full-size console window (at least 80 × 25) and do not resize it while playing. |
| Game does not start after pressing a key | Check that the console window has focus. Keys are read from the console, not from the mouse. |

## 8. Keep your fork up to date and send a pull request

Follow the team's rules from `CONTRIBUTING.md`:

- Stage only the files you own. **Do not use `git add .`**
- Do not commit changes to `Project.*` files.
- Each member edits only their own module folder and their own `docs/progress/<name>.md` and `docs/report/<name>.md`.

Typical weekly steps:

```powershell
git fetch upstream
git merge upstream/main          # or: git rebase upstream/main
# make your changes, build, and test in Visual Studio
git add src\alyan docs\progress\alyan.md docs\report\alyan.md   # only your own paths
git commit -m "Week 1 - Alyan - rendering, HUD, sonar"
git push origin week1-<yourname>
```

Then open a pull request from your fork's branch to the upstream `main` branch on GitHub, or with:

```powershell
gh pr create --repo muhammadanas20/Submarine-Defense-Game-with-Sonar-Detection --base main --head <your-username>:week1-<yourname>
```

Use the PR template (`.github/PULL_REQUEST_TEMPLATE.md`) and state what you built and tested.

## 9. Checklist before you demo

- [ ] Builds with **Debug | Win32** and **0 errors**.
- [ ] Menu, game, pause (P), sonar (SPACE), and ESC all work.
- [ ] Game over shows the final score and saves `highscore.dat` when you beat the record.
- [ ] You can explain your own module.
