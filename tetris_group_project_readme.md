# Tetris (C++ & SFML 3.0)

A classic, feature-rich Tetris implementation built in modern C++ using **SFML 3.0**. Developed as a Programming Fundamentals (PF) group project, this game recreates core Tetris mechanics—including ghost piece previews, piece holding, wall-kicks, and T-Spin detection—alongside multiple game modes, persistent high scores, save/load functionality, and an interactive falling-block credits screen.

## Team Credits

- **Faizan Ezad**
- **Alina Adnan**
- **Haseeb**

## Features

- **Three Distinct Game Modes:**
  - **Endless:** Classic survival gameplay with progressive speed scaling every 7 lines cleared until the stack reaches the top buffer rows.
  - **Marathon:** Race to clear **150 lines** to achieve victory while tracking your completion time.
  - **Timer (Ultra):** Score as many points as possible within a **5-minute (300-second)** countdown.
- **Core Tetris Mechanics:**
  - **Full 7-Tetromino Set:** `I`, `J`, `L`, `Z`, `S`, `O`, and `T` pieces with clockwise and counter-clockwise wall-kick rotation (`ShadowRotate`).
  - **Ghost Piece Preview:** Displays a translucent outline showing exact landing placement.
  - **Hold System (`C`):** Swap and store the active tetromino once per lock cycle.
  - **Next Piece Preview:** Displays the upcoming block in a dedicated side panel.
  - **T-Spin Detection & Scoring:** Detects 3-corner T-spin locks after a rotation input and awards bonus points for T-Spin 0, Single, Double, and Triple clears.
  - **Line-Clear Flash Animation:** Highlights completed rows in white for `0.5s` before collapsing the grid.
- **Persistence & Progression:**
  - **Per-Mode High Scores:** Automatically tracks and saves top scores for Marathon, Timer, and Endless modes in `highscore.txt`.
  - **Save & Resume State:** Returning to the Main Menu from Pause saves your exact board state, active/next/held pieces, score, level, and timer (`save_marathon.txt`, `save_timer.txt`, `save_endless.txt`) with one-click **Load** support.
- **Audio & UI:**
  - Looping 5-track background music playlist (`song1.wav` – `song5.wav`) with automatic pause/resume handling.
  - Dedicated **Controls** overlay accessible from both the Main Menu and Pause screen.
  - Interactive **Tetris-style Credits** screen where team member names fall and stack into the grid.

## Controls

| Key | Action |
| :--- | :--- |
| **Left / Right Arrow** | Move piece left / right (or cycle game modes on the Main Menu) |
| **Down Arrow** | Soft Drop |
| **Spacebar** | Hard Drop ($2 \times \text{rows dropped}$ bonus points) |
| **X** | Rotate Clockwise |
| **Z** | Rotate Counter-Clockwise |
| **C** | Hold / Swap Piece |
| **P** | Pause / Resume Game |
| **Enter** | Start Game (on Main Menu) |

## Scoring & Leveling

- **Level Progression:** Level increases by `1` for every **7 lines** cleared ($\text{Level} = \lfloor \text{Lines} / 7 \rfloor + 1$), accelerating automatic drop speed via $\text{delay} = (0.8 - (\text{Level} - 1) \times 0.007)^{\text{Level} - 1}$.
- **Hard Drop:** $2 \times \text{rows dropped}$

| Clear Type | Standard Points | T-Spin Points |
| :--- | :--- | :--- |
| **0 Lines (T-Spin No Line)** | — | $400 \times \text{Level}$ |
| **Single (1 Line)** | $100 \times \text{Level}$ | $800 \times \text{Level}$ |
| **Double (2 Lines)** | $300 \times \text{Level}$ | $1200 \times \text{Level}$ |
| **Triple (3 Lines)** | $500 \times \text{Level}$ | $1600 \times \text{Level}$ |
| **Tetris (4 Lines)** | $800 \times \text{Level}$ | — |

## Prerequisites & Required Assets

To build and run the project, ensure the following are available:

1. **Compiler & IDE:** Visual Studio 2022 (or newer) with **ISO C++17 Standard (`/std:c++17`)** or later enabled (`x64` architecture).
2. **SFML Library:** **SFML 3.0.0** (`x64` static libraries).
3. **Runtime Assets** (must be placed in the working directory alongside the executable or project root):
   - `Tetris-logo.PNG` — Game logo and window icon
   - `PressStart2P-Regular.ttf` — Retro pixel arcade font (UI text also uses `C:/Windows/Fonts/arial.ttf`)
   - `song1.wav`, `song2.wav`, `song3.wav`, `song4.wav`, `song5.wav` — Background soundtrack files

## Building & Running in Visual Studio (Windows x64)

This project is configured for **static linking** against **SFML 3.0** (`x64`) so the resulting executable runs standalone without requiring external SFML DLLs.

### 1. C/C++ General & Preprocessor Settings (All Configurations)

1. Open **Project Properties** (`Alt + F7`) and set **Platform** to **x64**.
2. Go to **C/C++** $\rightarrow$ **General** $\rightarrow$ **Additional Include Directories** and add your SFML `include` folder (e.g., `C:\SFML-3.0.0\include`).
3. Go to **C/C++** $\rightarrow$ **Language** $\rightarrow$ **C++ Language Standard** and select **ISO C++17 Standard (`/std:c++17`)** (required for `<filesystem>`).
4. Go to **C/C++** $\rightarrow$ **Preprocessor** $\rightarrow$ **Preprocessor Definitions** (under **All Configurations**) and add:

```text
SFML_STATIC
```

### 2. Linker Library Directory (All Configurations)

- Go to **Linker** $\rightarrow$ **General** $\rightarrow$ **Additional Library Directories** (under **All Configurations**) and add your SFML `lib` folder (e.g., `C:\SFML-3.0.0\lib`).

### 3. Linker Input Dependencies

#### Debug (`x64`)

Set **Configuration** to **Debug**, go to **Linker** $\rightarrow$ **Input** $\rightarrow$ **Additional Dependencies**, and paste:

```text
sfml-graphics-s-d.lib;sfml-window-s-d.lib;sfml-audio-s-d.lib;sfml-system-s-d.lib;freetype.lib;harfbuzzd.lib;opengl32.lib;winmm.lib;gdi32.lib;flac.lib;vorbisenc.lib;vorbisfile.lib;vorbis.lib;ogg.lib;%(AdditionalDependencies)
```

*(Optional)* To silence harmless `LNK4099` missing `.pdb` warnings from SFML's bundled static dependencies in Debug mode, add `/ignore:4099` under **Linker** $\rightarrow$ **Command Line** $\rightarrow$ **Additional Options**.

#### Release (`x64`)

Set **Configuration** to **Release**, go to **Linker** $\rightarrow$ **Input** $\rightarrow$ **Additional Dependencies**, and paste:

```text
sfml-graphics-s.lib;sfml-window-s.lib;sfml-audio-s.lib;sfml-system-s.lib;freetype.lib;harfbuzz.lib;opengl32.lib;winmm.lib;gdi32.lib;flac.lib;vorbisenc.lib;vorbisfile.lib;vorbis.lib;ogg.lib;%(AdditionalDependencies)
```

### 4. Build and Run

- Select either **Debug** or **Release** (`x64`) in the toolbar and press `Ctrl + F5` to build and launch the game.

## Project Structure

```text
Group-7-Tetris/
├── main.cpp                  # Complete game loop, physics, collision, UI, and state management
├── Tetris-logo.PNG           # Title screen logo and window icon
├── PressStart2P-Regular.ttf  # Arcade header font
├── song1.wav - song5.wav     # Looping background music tracks
├── highscore.txt             # Auto-generated high scores [Marathon, Timer, Endless]
├── save_marathon.txt         # Auto-generated Marathon mode save state
├── save_timer.txt            # Auto-generated Timer mode save state
├── save_endless.txt          # Auto-generated Endless mode save state
└── README.md                 # Project documentation
```