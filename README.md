# CartHub: A Multi-Mode Grocery Planner and Receipt Generator
**Created By: John Calvin Samson, CS-1203**
\
*Final Project for the Course CC 103 | Data Structures and Algorithms*

## 📖 Project Overview
CartHub is a lightweight, high-performance C++ console application designed to manage grocery lists, track live shopping expenses, and generate checkout receipts.

Under the hood, CartHub is powered by a custom Doubly Linked List data structure, ensuring efficient memory management and rapid CRUD (Create, Read, Update, Delete) operations without relying on standard library vectors or arrays.

## 🔄 The State Machine Workflow
The application operates through three distinct states to mirror a real-world shopping trip:
1.  **Planning Mode**: Draft your initial list; items default to a 0.00 price until you are ready to shop.
2.  **Shopping Mode**: Update quantities and enter live prices as you pick up items from the shelves.
3.  **Checkout Mode**: Perform a final review of your expenses and export a formatted, timestamped receipt.

## ✨ Features
* **Three-Phase Architecture**: Seamlessly transition between Planning Mode (Drafting), Live Shopping Mode (Updating prices/quantities), and Checkout Mode.
* **Data Persistence**: Automatically saves your grocery list to `carthub_save.txt` on exit and reloads it on startup. You never lose your list!
* **Smart Duplicate Handling**: Scans for existing items and intelligently merges quantities instead of creating duplicate entries.
* **Receipt Generation**: Calculates final totals and exports a beautifully formatted .txt receipt with a week-and-year timestamp.
* **Robust Input Validation**: Bulletproof UI menus that catch typos, prevent infinite loops, and handle string-to-number conversions safely.

## 🧠 Technical Deep Dive: Why Doubly Linked Lists?
Standard arrays or vectors require $O(n)$ time to shift elements during deletion. By implementing a custom Doubly Linked List, CartHub achieves:
*   **Efficient Deletion**: Removing a "mistake" from the middle of the list only requires updating neighboring pointers.
*   **Dynamic Memory**: Nodes are allocated and deallocated at runtime, keeping the memory footprint minimal.
*   **Seamless Traversal**: Supports forward and backward navigation, facilitating easy rendering of UI lists.

## 🛠️ Technical Details
* **Language**: `C++` (Standard C++11 or higher)
* **Architecture**: Object-Oriented Programming (OOP) with a modular 3-file system (`main.cpp`, `CartHub.cpp`, `CartHub.h`).
* **Data Structure**: Custom Doubly Linked List (Node and cartProperty structs).
* **Libraries Used**: `<iostream>`, `<fstream>`, `<sstream>`, `<iomanip>`, `<thread>`, `<chrono>`, `<ctime>`.

## 📄 Sample Receipt Output
```text
************** CARTHUB RECEIPT SUMMARY ***************
----------------- Week 19 - May, 2026 ----------------
1   Apples (Produce)       x2  pcs   PHP      30.00
2   Milk (Dairy)           x1  pk    PHP      95.00
......................................................
TOTAL ITEMS: 2         AMOUNT DUE: PHP 155.00
======================================================
```

## 🚀 How to Compile and Run
If you are using a standard GCC/MinGW terminal, you can compile the application by linking the source files together:

```Bash
# 1. Compile the program
g++ main.cpp CartHub.cpp -o CartHubApp

# 2. Run the executable
./CartHubApp
```

## 🎮 Controls
* **[P]** Plan Grocery List (Add/Delete items, set categories).
* **[S]** Start Shopping (Update live prices as you put items in your physical cart).
* **[C]** Checkout (Review totals and generate your receipt).
* **[E]** Empty Cart (Wipe the list and start over).
* **[Q]** Save & Quit.

