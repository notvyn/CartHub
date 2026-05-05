// CartHub.h
#pragma once // Prevents the computer from reading the menu twice

#include <string>
using namespace std;

// ==========================================
// DATA STRUCTURES
// ==========================================

// A lightweight container used to safely pass an item's details 
// without exposing the actual linked list memory pointers.
struct cartProperty {
    string itemName;
    string category;
    int quantity;
    double price;
};

// The core building block of the doubly linked list.
struct Node {
    string itemName;
    string category;
    int quantity;
    double price;
    Node* next;
    Node* prev;

    // Initializes a node with default numeric values and null pointers.
    Node(string name);
};

// ==========================================
// CORE APPLICATION LOGIC
// ==========================================

// The linked list manager handling all CRUD (Create, Read, Update, Delete) operations.
class CartHub {
private:
    Node* head;
    Node* tail;

public:
    // Initializes an empty list.
    CartHub();

    // Appends a new item to the end of the linked list.
    void addItem(string name, string category, int quantity, double price = 0.00);
    
    // Safely deallocates all node memory and resets list pointers.
    void clearList();
    
    // Removes a node based on its 1-based index position.
    void deleteItem(int targetItem);
    
    // Renders the list to the console. Mode 'D' = Draft, 'C' = Receipt, Default = Shop.
    bool displayCart(char mode = '\0');
    
    // Updates a specific property (1: Price, 2: Qty, 3: Category) of an item by index.
    void editItem(int targetNumber, int fieldToEdit, string userInput);
    
    // Retrieves just the string name of an item at the target index.
    string findItemName(int targetItem);
    
    // Extracts full item details by index and bundles them into a cartProperty struct.
    cartProperty getItem(int targetItem);
    
    // Computes final totals, writes data to a formatted text file, and clears the cart.
    void generateReceipt(string receiptName);
    
    // Checks if an item exists. If true, prompts user to overwrite quantity instead of adding.
    bool handleDuplicate(string targetItem);

    // ==========================================
    // DATA PERSISTENCE
    // ========================================== 
    
    // Wipes all saved grocery list data by overwriting the save file with an empty file.
    void deleteData();

    // Reads the save file and rebuilds the linked list on startup.
    void loadData();

    // Flattens the linked list and saves it to a text file.
    void saveData();
};

// ==========================================
// USER INTERFACE & MENUS
// ==========================================

// Displays the primary welcome screen and root navigation options.
void displayMainMenu(CartHub& myCart);

string formatDate(const char* format);

// Interactive loop for naming items, checking duplicates, selecting categories, and setting quantities.
void promptAddItems(CartHub& myCart);

// Triggers the timed checkout sequence and generates the final receipt file.
void promptCheckoutCart(CartHub& myCart);

// Interactive loop for modifying existing item properties via index selection.
void promptEditItem(CartHub& myCart);

// Interactive loop for removing list items ('D' for Plan Mode, 'R' for Shop Mode).
void promptRemoveItem(CartHub& myCart, char mode);

// The root application lifecycle loop.
void runMainMenu(CartHub& myCart);

// Master state machine managing loops within specific rooms (Plan, Shop, Checkout).
void runActiveMode(char mode, CartHub& myCart);

// Renders the 10-item category UI, validates numeric input, and returns the category string.
string selectCategory();

// UI Wrappers that print the headers and hotkey options for specific rooms.
void showPlanMode(CartHub& myCart);
void showShoppingMode(CartHub& myCart);
void showCheckoutMode(CartHub& myCart);

// ==========================================
// UTILITY FUNCTIONS
// ==========================================

// Suspends program execution for a precise number of milliseconds.
void delay(int milliseconds);

// Freezes the console and waits for the user to hit the Enter key.
void waitForEnter();