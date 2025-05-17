# Virtual Memory Simulator 



## Project Setup and Execution Guide

### Overview
This project consists of three main components:
- Operating System (`os`)
- Memory Management Unit (`mmu`)
- Application (`my_app`)

You will need to compile and run each component in separate terminal windows.

---

### Compilation Instructions

Open **three separate terminals** and compile the files using the following commands:

### 1. Compile Operating System
```bash
g++ OperatingSystems.cpp -o os
```
### 2. Compile MMU
```bash
g++ MMU.cpp VMAlgorithms.cpp -o mmu -lrt
```
### 3. Compile Application
```bash
g++ -o my_app main.cpp -I/usr/include/qt6 -lQt6Widgets -lQt6Gui -lQt6Core -fPIC
```
## Running the program

### Terminal 1: Run Operating System
```bash
./os
```
### Terminal 2: Run MMU
```bash
./mmu
```
### Terminal 3: Run Application
```bash
./my_app &
```
