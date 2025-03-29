# 🎲 Multithreaded Ludo Game with SFML GUI

A C++ implementation of the classic Ludo game featuring **multithreading, SFML graphics, and OS synchronization concepts** (mutexes, semaphores). Designed for Windows/Linux with a focus on operating system principles.



## 📂 Project Structure
```plaintext
src/
├── core/
│   ├── dice.cpp/h           # Thread-safe dice rolling
│   ├── player.cpp/h         # Player thread logic
│   ├── grid.cpp/h           # Board initialization
│   ├── GameLogic.cpp/h      # Main game rules
│   └── utility.cpp/h        # Helper functions
├── ui/
│   ├── WindowManager.cpp/h  # SFML rendering
│   └── messages.cpp/h       # UI text/notifications
├── main.cpp                 # Entry point
└── Makefile
```

## 📌 Key Features
- **4-Player Multithreaded Gameplay**: Each player is managed by a separate thread.
- **Thread-Safe Operations**:  
  - `gridMutex` and `diceMutex` prevent race conditions during grid/dice access.
  - Semaphores (`turnSemaphore`) enforce turn-based synchronization.
  - Thread cancellation for inactive players
- **Dynamic Thread Cancellation**:  
  - Master thread terminates inactive players (20 turns without a hit) or winners.
- **Modular Design**: Separate classes for `Player`, `Dice`, and `Grid`.
- SFML GUI
  - Visual board with token movements

## 🛠️ Technologies Used
- **Language**: C++ (with pthread library)
- **Concepts**:  
  - Mutual Exclusion (`mutex`)  
  - Resource Sharing (Global variables)  
  - Thread Synchronization (`semaphore`)  
  - Thread Cancellation (`pthread_cancel`)  

## ⚙️ Installation & Usage
### Prerequisites
- Linux OS
- SFML library (libsfml-graphics, libsfml-window, libsfml-system)
- GCC/Clang with C++17 support

### Steps:
1. Clone the repository:
   ```bash
   git clone https://github.com/fawad57/Ludo-Game-OS-Project.git
   cd Ludo-Game-OS-Project
   ```
2. Compile and run:
   ```bash
   make          # Build project
   ./ludo_game   # Run executable
   ```
3. Clean Build
   ```Copy
   make clean    # Remove object files
   ```

### 📌 Makefile Details
```makefile
Copy
CXX = g++
CXXFLAGS = -std=c++17 -pthread -lsfml-graphics -lsfml-window -lsfml-system
SRC = main.cpp grid.cpp dice.cpp player.cpp utility.cpp messages.cpp WindowManager.cpp GameLogic.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = ludo_game
```
   

## 📜 Operating System Concepts Implemented
| Concept          | Implementation Example                          |
|------------------|-------------------------------------------------|
| **Mutual Exclusion** | `gridMutex` locks the board during token movement. |
| **Resource Sharing** | Global `diceValue` shared across player threads.  |
| **Thread Sync**      | `turnSemaphore` ensures players take turns.       |
| **Thread Cancellation** | Master thread kills inactive players after 20 turns. |


## 🎯 Challenges & Solutions
- **Race Conditions** → Fixed with `mutex` locks.  
- **Turn Management** → Solved using semaphores.  
- **Thread Starvation** → Master thread monitors inactivity.  

## 👥 Contributors
- **Fawad Humayun** (221-2548): Core game logic, grid initialization.  
- **Areeba Nisar** (221-8770): Multithreading, synchronization.  

📜 License
MIT License. See LICENSE for details.

```Copy

### Why This Works?
1. **SFML Integration Highlighted**  
   - Clearly mentions the GUI dependency upfront.
2. **File Structure Visualization**  
   - Tree format helps contributors navigate code.
3. **Makefile Ready**  
   - Users can copy-paste build commands directly.
4. **OS Concepts Linked to Files**  
   - Maps `dice.cpp` → `mutex`, `player.cpp` → threads, etc.
