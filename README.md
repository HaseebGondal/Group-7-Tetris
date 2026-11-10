<div align="center">

# Group 7 — Tetris

### A 2D Tetris game built with C++ and SFML

**Mid-Project Evaluation · Fall 2026**

[Repository](https://github.com/HaseebGondal/Group-7-Tetris)

</div>

---

## About the Project

Group 7 is developing a 2D version of **Tetris** using C++ and the SFML graphics library. The aim is to recreate the core gameplay while applying programming concepts such as 2D vectors, functions, collision detection, game loops, and event handling.

This repository represents the **current development stage for the mid-project evaluation**. The game is a work in progress: the core board and falling-block mechanics are being developed, while additional gameplay systems and improvements are still to come.

## Current Development Status

The current prototype has a working game window and a 20 × 10 playfield. Tetrominoes can spawn, fall, move horizontally, rotate, interact with the board boundaries and previously placed blocks, and become part of the stored game grid when they land.

### Implemented so far

- **Game window and playfield:** SFML window with a 20-row × 10-column grid.
- **Tetromino shapes:** Representation of all seven standard pieces — I, J, L, O, S, T, and Z.
- **Piece generation:** Random selection of the current and upcoming pieces.
- **Automatic falling:** The active piece moves down over time.
- **Horizontal movement:** The player can move the active piece left and right.
- **Rotation:** The active piece can rotate, with horizontal offset checks to help it fit near walls.
- **Collision detection:** Checks against the playfield boundaries and occupied cells.
- **Block locking:** When a piece can no longer descend, its occupied cells are stored in the game grid.
- **Line clearing logic:** Completed rows are detected and removed, with the rows above shifted down.
- **Next-piece preview:** The upcoming tetromino is displayed separately from the main playfield.

### Still in development

The following are areas for further development and refinement. They should not be considered completed features at this evaluation stage.

- Scoring system and score display
- Level progression and balanced difficulty
- Game-over detection and restart functionality
- Additional gameplay and interface improvements
- Testing, debugging, and overall polish

The feature list may change as development continues.

## Controls

| Key | Action |
| --- | --- |
| `←` or `A` | Move the active piece left |
| `→` or `D` | Move the active piece right |
| `Space` | Rotate the active piece |

The active piece falls automatically. Closing the game window exits the application.

## Technologies Used

| Technology | Purpose |
| --- | --- |
| **C++** | Game logic, data handling, and core implementation |
| **SFML** | Window management, rendering, shapes, and keyboard input |
| **Visual Studio** | Development environment and project build |

## How the Current Prototype Works

The playfield is represented using a two-dimensional `std::vector`. Each cell stores whether it is empty or occupied by a block. Tetrominoes are also represented using two-dimensional vectors, allowing the program to check their occupied cells during movement and rotation.

The main game loop handles keyboard events, updates the falling piece, checks collisions, stores landed pieces in the board, checks for completed rows, and draws the board and active pieces to the window.

Keeping landed blocks in the main grid is important: when a new piece appears, previously placed blocks remain visible and continue to affect collision detection.

## Project Structure

```text
Group-7-Tetris/
│
├── Dependencies/
│   └── SFML/                 # SFML dependency files
│
├── Game.slnx                # Visual Studio solution
├── Game.vcxproj             # C++ project configuration
├── Game.vcxproj.filters     # Visual Studio file filters
├── main.cpp                 # Current game implementation
├── .gitignore
│
└── ...                      # Generated Visual Studio build files
```

The main game logic is currently written in `main.cpp`. The project may be reorganized into multiple source and header files as development progresses.

## Getting Started

### Requirements

- Windows
- Visual Studio with the **Desktop development with C++** workload
- The SFML dependency included in the repository

### Build and Run

1. Clone the repository:

   ```bash
   git clone https://github.com/HaseebGondal/Group-7-Tetris.git
   ```

2. Open the downloaded project folder.

3. Open `Game.slnx` in a compatible version of Visual Studio. If your Visual Studio version does not support `.slnx`, open `Game.vcxproj`.

4. Select the **x64** platform and a configuration such as **Debug**.

5. Build the project using **Build → Build Solution**.

6. Run it using **Debug → Start Without Debugging** (`Ctrl + F5`).

The Visual Studio project is configured around the SFML dependency in `Dependencies/SFML`. If you change the build platform or configuration, check that the SFML include, library, and runtime DLL paths still match that configuration.

## Team Members

This project is being developed collaboratively by **Group 7**.

| Member | Role |
| --- | --- |
| **Haseeb** | Group member |
| **Faizan** | Group member |
| **Alina** | Group member |

Individual work and progress can be followed through each member's commits and contributions in the repository.

## Project Progress

This README documents the state of the project at the **mid-project evaluation**. Development is ongoing, and the repository will be updated as new features are implemented, tested, and integrated.

---

*Developed as a group project to practise C++, problem-solving, data structures, and graphics programming with SFML.*
