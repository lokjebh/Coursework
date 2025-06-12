## Minesweeper (Qt GUI)

A simple version of the classic game Minesweeper implemented in **C++**, leveraging **Qt** as a graphical framework, with development on a Unix/Linux System.
This version faithfully reproduces the classic Minesweeper game with a clean GUI, mouse interactions, flagging machanics, and animated reset button.

---

### Features
- Adjustable grid size and mine count (default: 16x30 grid, 99 mines)
- Left-click to uncover cells
- Right-click to flag/unflag suspected mines
- Auto-reveal empty areas recursively
- Game-over detection with mine reveal
- Smiley-face reset button with icon states (pressed, win, fail)
- Simple and responsive UI layout using Qt layouts and frames

---

### Build Instructions

#### Requirements
- Qt 5 or 6 (Tested on Qt 5.15+)
- A C++ compiler (e.g. `g++`)
- `make`

#### Build Steps
1. **Clone or download the project**
   ```
   git clone https://github.com/lokjebh/Coursework/minesweeper-qt.git
   cd minesweeper-qt
   ```
2. **Compile the project**
   ```
   qmake Minwsweeper.pro
   make
   ```
3. **Run the application**
   ```
   ./Minesweeper
   ```

---

### Packaging
Package this project with:
```
tar -czvf Minesweeper.tar.gz Minesweeper/
```
Extract and build with:
```
tar -xzvf Minesweeper.tar.gz
```

---

### Acknowledgments
- Developed using the Qt framework
- Icons adapted from classic Minesweeper themes

