#include "CartHub.h"  // CartHub Header File containing Function Prototypes
#include <iostream>   // For cout / cin
#include <iomanip>    // For setw and setprecision
#include <fstream>    // For saving the receipt
#include <sstream>    // For parsing the save file
#include <cctype>     // For toupper
#include <ctime>      // For the receipt date
#include <thread>     // For the delay function
#include <chrono>     // For the delay function

using namespace std;

// Initializes a new grocery item node for the doubly linked list.
Node::Node(string name) {
    itemName = name;
    quantity = 0;
    price = 0;
    next = nullptr;
    prev = nullptr;
};

// Initializes an empty CartHub doubly linked list.
CartHub::CartHub () {
    head = nullptr;
    tail = nullptr;

    // Automatically load saved data the moment CartHub is created!
    loadData();
};

// Appends a new item node to the end of the doubly linked list.
void CartHub::addItem(string name, string category, int quantity, double price) {
    Node* newNode = new Node(name);

    newNode->category = category;
    newNode->price = price;
    newNode->quantity = quantity;

    if (head == nullptr) {
        head = tail = newNode;
    }
    else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
}

// Safely deallocates memory for all nodes and resets the list to empty.
void CartHub::clearList() {
    Node* temp = head;

    while (temp != nullptr) {
        Node* nextNode = temp->next;
        delete temp;
        temp = nextNode;
    }

    head = nullptr;
    tail = nullptr;

    cout << "Cart has been completely cleared." << endl;
}

// Traverses the list to find an item by its 1-based index and removes it, updating neighboring pointers.
void CartHub::deleteItem(int targetItem) {
    Node* temp = head;
    int currentNumber = 1;

    while (temp != nullptr && currentNumber != targetItem) {
        temp = temp->next;
        currentNumber++;
    }

    if (temp == nullptr) {
        cout << "No items on list yet." << endl;
        return;
    }
    
    // Stand alone
    if (head == tail) { 
        head = tail = nullptr;  
    }
    // Target is at Head
    else if (temp == head) {
        head = temp->next; 
        head->prev = nullptr;
    }
    // Target is at Tail
    else if (temp == tail) {
        tail = temp->prev;
        tail->next = nullptr;
    }
    // Target is in the middle
    else {
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
    }

    cout << "Successfully removed " << temp->itemName << "." << endl;
}

// Renders the cart to the console. Formats differently based on mode ('D' for Draft, 'C' for Checkout, default for Shopping). Returns true if items exist.
bool CartHub::displayCart(char mode) {
    Node* temp = head;
    int counter = 0;

    if (temp == nullptr) {
        cout << "----------- Track your Grocery List here -----------" << endl;
        cout << "Your cart is currently empty." << endl;
        cout << "----------------------------------------------------" << endl;
        return false;
    }
    else {
        // Display Cart in Draft Mode
        if (mode == 'D') {
            cout << "----------- Track your Grocery List here -----------" << endl;
            cout << "Current Draft:" << endl;

            while (temp != nullptr) {
                counter += 1;
                string productName = temp->itemName + " (" + temp->category + ")";
                cout << left << setw(5) << counter << left << setw(30) << productName << right << setw(15) << " - Qty: " << temp->quantity << endl;
                temp = temp->next;
            }
            cout << "----------------------------------------------------" << endl;
        }
        // Display Cart in Checkout Mode
        else if (mode == 'C') { 
            cout << "***************** RECEIPT SUMMARY ******************" << endl;
            float totalPrice = 0;
            while (temp != nullptr) {
                counter += 1;
                string productName = temp->itemName + " (" + temp->category + ")";
                // Aligned specifically for a receipt look
                cout << left << setw(4) << counter << left << setw(25) << productName << "x" << left << setw(4) << temp->quantity << "PHP " << right << setw(10) << fixed << setprecision(2) << temp->price << endl;
                totalPrice += (temp->price * temp->quantity);
                temp = temp->next;
            }
            cout << "...................................................." << endl;
            cout << "TOTAL ITEMS: " << left << setw(10) << counter << "AMOUNT DUE: PHP " << fixed << setprecision(2) << totalPrice << endl;
        }
        // Display Cart in Normal Mode
        else {
            cout << "----------- Track your Grocery List here -----------" << endl;
            float totalPrice = 0;
            while (temp != nullptr) {
                counter += 1;
                string productName = temp->itemName + " (" + temp->category + ")";
                cout << left << setw(5) << counter << left << setw(20) << productName << setw(5) << " - Qty: " << left << setw(5) << temp->quantity << left << setw(10) << " - PHP " << fixed << setprecision(2) << temp->price << endl;
                totalPrice += (temp->price * temp->quantity);
                temp = temp->next;
            }
            cout << "----------------------------------------------------" << endl;
            cout << "TOTAL ITEMS: " << counter << " | " << "CURRENT TOTAL: PHP " << fixed << setprecision(2) << totalPrice << endl;
        }
        return true;
    }
}

// Modifies a specific field (1: Price, 2: Quantity, 3: Category) of an item located by its 1-based index.
void CartHub::editItem(int targetNumber, int fieldToEdit, string userInput) {
    Node* temp = head;
    int currentNumber = 1;

    while (temp != nullptr && currentNumber != targetNumber) {
        temp = temp->next;
        currentNumber++;
    }

    if (temp == nullptr) {
        cout << "No items on list yet." << endl;
        return;
    }

    switch (fieldToEdit) {
        case 1: {
            try {
                double newPrice = stod(userInput);
                temp->price = newPrice;
                cout << "Successfully changed " << temp->itemName << " price." << endl;
            } catch (...) {
                cout << "Error: Invalid price entered. Please use numbers." << endl;
            }
            break;
        }
        case 2: {
            try {
                int newQuantity = stoi(userInput);
                temp->quantity = newQuantity;
                cout << "Successfully changed " << temp->itemName << " quantity." << endl;
            } catch (...) {
                cout << "Error: Invalid quantity entered. Please use whole numbers." << endl;
            }
            break;
        }
        case 3:
            temp->category = userInput;
            cout << "Successfully changed " << temp->itemName << " category." << endl;
    }
    
    cout << endl;
}

// Retrieves an item's string name using its 1-based index. Returns an empty string if not found.
string CartHub::findItemName(int targetItem) {
    Node* temp = head;
    int currentNumber = 1;
    string itemFound;

    while (temp != nullptr && currentNumber != targetItem) {
        temp = temp->next;
        currentNumber++;
    }

    if (temp == nullptr) {
        cout << "Item number not found." << endl << endl;
        return itemFound = "";
    }

    itemFound = temp->itemName;

    return itemFound;
}

// Packages and returns a specific item's data into a cartProperty struct using its 1-based index.
cartProperty CartHub::getItem(int targetItem) {
    Node* temp = head;
    int currentNumber = 1;
    cartProperty item;

    while (temp != nullptr && currentNumber != targetItem) {
        temp = temp->next;
        currentNumber++;
    }

    if (temp == nullptr) {
        cout << "Item number not found." << endl;
        item.itemName = "";
        return item;
    }
    
    item.itemName = temp->itemName;
    item.category = temp->category;
    item.quantity = temp->quantity;
    item.price = temp->price;

    return item;
}

// Calculates the final total, exports the cart contents to a formatted .txt file, and clears the cart.
void CartHub::generateReceipt(string receiptName) {
    // Create and open a text file named "<receiptName>.txt"
    ofstream receiptFile(receiptName + ".txt");

    // Safety check: Did the file actually open?
    if (!receiptFile.is_open()) {
        cout << "Error: Could not open file to save receipt!" << endl;
        return;
    }

    // Write the header to the FILE (notice we use receiptFile instead of cout)
    receiptFile << "************** CARTHUB RECEIPT SUMMARY ***************" << endl;

    string dateBuffer = formatDate("Week %U - %b, %Y");

    receiptFile << "----------------- " << dateBuffer << " ----------------" << endl;
    
    Node* temp = head;
    int counter = 0;
    double totalSpent = 0.0;

    // Walk down the list and write each item to the file
    while (temp != nullptr) {
        counter += 1;
        string productName = temp->itemName + " (" + temp->category + ")";
        receiptFile << left << setw(4) << counter << left << setw(25) << productName << "x" << left << setw(4) << temp->quantity << "PHP " << right << setw(10) << fixed << setprecision(2) << temp->price << endl;
        totalSpent += (temp->price * temp->quantity); // Keep a running total
        temp = temp->next;
    }

    // Write the footer and total
    receiptFile << "......................................................" << endl;
    receiptFile << "TOTAL ITEMS: " << left << setw(10) << counter << "AMOUNT DUE: PHP " << fixed << setprecision(2) << totalSpent << endl;
    receiptFile << "======================================================" << endl;

    // Close the file and save
    receiptFile.close(); 

    cout << "\nSuccess! Checkout complete. Receipt saved to " << receiptName << ".txt." << endl;
    clearList();
    deleteData();
}

// Scans for an existing item by name. If found, prompts the user to overwrite the quantity to prevent duplicate list entries.
bool CartHub::handleDuplicate(string targetItem) {
    Node* temp = head;

    while (temp != nullptr) {
        if (temp->itemName == targetItem) {
            cout << "\nNotice: " << targetItem << " is already in your cart (Current Qty: " << temp->quantity << ")." << endl;
            
            int newTotalQuantity; 
            while (true) {
                cout << "What should the NEW total quantity be? (Type 0 to cancel): ";
                cin >> newTotalQuantity;
                cin.ignore();
                
                if (cin.fail() || newTotalQuantity < 0) {
                    cin.clear();
                    cout << "Invalid input. Please enter a valid number." << endl;
                } else {
                    break;
                }
            }

            if (newTotalQuantity > 0) {
                temp->quantity = newTotalQuantity; 
                cout << "Success! Quantity updated to " << temp->quantity << " " << targetItem << "(s).";
            } else {
                cout << "Update cancelled. Quantity remains at " << temp->quantity << ".";
            }
            
            waitForEnter();
            cout << endl;
            return true; // We found a duplicate and handled it!
        }
        temp = temp->next;
    }

    return false; // It is a brand new item!
}

// Wipes all saved grocery list data by overwriting the save file with an empty file.
void CartHub::deleteData() {
    ofstream outFile("carthub_save.txt");

    if (outFile.is_open()) {
        outFile.close();
        cout << "Save data has been successfully cleared." << endl;
    } else {
        cout << "Error: Could not access the save file." << endl;
    }
}

// Reads the save file and rebuilds the linked list on startup.
void CartHub::loadData() {
    ifstream inFile("carthub_save.txt");

    // If the file doesn't exist (like on the very first time running the app), just quietly return.
    if (!inFile.is_open()) {
        return; 
    }

    string line;
    // Read the file line by line
    while (getline(inFile, line)) {
        stringstream ss(line);
        string name, category, qtyStr, priceStr;

        // Chop the line into pieces using the pipe '|' as a cutting point
        getline(ss, name, '|');
        getline(ss, category, '|');
        getline(ss, qtyStr, '|');
        getline(ss, priceStr, '|');

        // Safety check: Make sure the line wasn't blank
        if (!name.empty()) {
            try {
                int quantity = stoi(qtyStr);
                double price = stod(priceStr);
                
                // Rebuild the list
                addItem(name, category, quantity, price);
            } catch (...) {
                // If the save file got corrupted, skip that line
                continue; 
            }
        }
    }

    inFile.close();
}

// Flattens the linked list and saves it to a text file.
void CartHub::saveData() {
    ofstream outFile("carthub_save.txt");

    if (!outFile.is_open()) {
        cout << "Error: Could not create save file!" << endl;
        return;
    }

    Node* temp = head;
    while (temp != nullptr) {
        // Write the data separated by pipes '|'
        outFile << temp->itemName << "|"
                << temp->category << "|"
                << temp->quantity << "|"
                << temp->price << endl;
        temp = temp->next;
    }

    outFile.close();
}

// ==================================
// Global User Interface (UI) Methods
// ==================================

// Renders the primary application navigation menu.
void displayMainMenu(CartHub& myCart) {
    cout << "\n=============== WELCOME TO CARTHUB ! ===============" << endl;
    myCart.displayCart();
    cout << "====================================================" << endl; 
    cout << "[P] Plan Grocery List (Add/Delete Items)" << endl;
    cout << "[S] Start Shopping (Update Quantity/Prices)" << endl;
    cout << "[C] Checkout & Save Receipt" << endl;
    cout << "[E] Empty Cart (Start Over)" << endl;
    cout << "[Q] Save & Quit Application" << endl;
    cout << "----------------------------------------------------" << endl;
}

// Retrieves the current system time and returns it as a formatted string based on the provided strftime format code.
string formatDate(const char* format) {
    time_t now = time(0);
    tm* ltm = localtime(&now);

    char dateBuffer[80];
    strftime(dateBuffer, sizeof(dateBuffer), format, ltm);

    return dateBuffer;
}

// Interactive loop for typing item names, detecting duplicates, selecting categories, and defining quantities.
void promptAddItems(CartHub& myCart) {
    while (true) {
        bool hasItems = myCart.displayCart('D');
        cout << "\n******** ADDING ITEMS (Type '0' for name to stop) ********" << endl;

        string itemName, category;
        int quantity;

        cout << "Enter product name: ";
        getline(cin >> ws, itemName);
        itemName[0] = toupper(static_cast<unsigned char>(itemName[0]));

        bool isDuplicate = myCart.handleDuplicate(itemName);

        if (isDuplicate) {
            continue;
        }
        
        if (itemName == "0") {
            cout << "List Completed. Proceed to Live Shopping." << endl;
            break;
        }

        category = selectCategory();
        
        cout << "Enter quantity: ";
        cin >> quantity;
        cin.ignore();

        if (cin.fail() || quantity <= 0) {
            cin.clear();
            cout << "Invalid quantity. Please enter a valid number greater than 0." << endl;
        }

        // Add to List
        myCart.addItem(itemName, category, quantity);
        cout << "Successfully added " << quantity << " " << itemName << " on " << category << " category." << endl << endl;
    }
}

// Simulates a processing delay, generates a time-stamped filename, and triggers the receipt generation.
void promptCheckoutCart(CartHub& myCart) {
    cout << "--- FINALIZING PURCHASE ---" << endl;
    delay(1000); // Pause for 1 second

    cout << "Validating cart items..." << endl;
    delay(800); // Pause for 0.8 second

    bool hasItem = myCart.displayCart();

    if (!hasItem) {
        cout << "No item found in List. Please add items first. ";
        waitForEnter();
        cout << endl;
        return;
    }
    
    string dateBuffer = formatDate("Receipt_Week_%U_%b_%Y");

    // Convert the char array back into a standard C++ string
    string receiptName(dateBuffer);

    cout << "Generating " << receiptName << ".txt..." << endl;
    delay(1500); // Pause for 1.5 second
    
    // Save the file and exit
    myCart.generateReceipt(receiptName);
}

// Interactive loop that asks the user to select an item by index, choose a property to change, and updates it.
void promptEditItem(CartHub& myCart) {
    while (true) {    
        bool hasItems = myCart.displayCart();
        cout << "\n******** EDITING ITEMS (Type 'x' to cancel) *******" << endl;

        if (hasItems) {
            cout << "Which item number do you want to edit?: ";
            
            string targetItem;
            cin >> targetItem;

            if (targetItem == "x" || targetItem == "X") {
                cout << "Exiting editing mode." << endl;
                break;
            }
            
            int targetNumber;
            
            try {
                targetNumber = stoi(targetItem); 
            } 
            catch (...) {
                cout << "Error: Invalid input. Please enter a valid number." << endl;
                continue;
            }

            string itemFound = myCart.findItemName(targetNumber);

            if (!(itemFound == "")) {
                cartProperty selectedItem = myCart.getItem(targetNumber);

                cout << "--- Editing: " << selectedItem.itemName << " (" << selectedItem.category << ") ---" << endl;
                cout << "Current Price: PHP " << fixed << setprecision(2) << selectedItem.price << " | Qty: " << selectedItem.quantity << endl << endl; 

                int choice;

                cout << "What would you like to update?" << endl;
                cout << "[1] Price | [2] Quantity | [3] Category" << endl;

                do {
                    cout << "Choice (1-3): ";
                    cin >> choice;
                    cin.ignore();
                } while (choice < 1 || choice > 3);

                string userInput;

                switch (choice) {
                    case 1:
                        cout << "Change " << selectedItem.itemName << " price to: ";
                        getline(cin, userInput);
                        myCart.editItem(targetNumber, choice, userInput);
                        break;
                    case 2:
                        cout << "Change " << selectedItem.itemName << " quantity to: ";
                        getline(cin, userInput);
                        myCart.editItem(targetNumber, choice, userInput);
                        break;
                    case 3:
                        cout << "Change " << selectedItem.itemName << " category to: ";
                        userInput = selectCategory();
                        myCart.editItem(targetNumber, choice, userInput);
                        break;
                }
            }
        }
        else {
            cout << "Please add items first. ";
            waitForEnter();

            cout << endl;
            break;
        }
    }
}

// Interactive loop that asks the user for an item index, confirms their choice, and deletes the item.
void promptRemoveItem(CartHub& myCart, char mode) {
    while (true) {
        bool hasItems;

        if (mode == 'D') {
            hasItems = myCart.displayCart('D');
            cout << "\n******** DELETING ITEMS (Type 'x' to cancel) *******" << endl;
        }
        else if (mode == 'R') {
            hasItems = myCart.displayCart();
            cout << "\n******** RETURNING ITEMS (Type 'x' to cancel) *******" << endl;
        }

        if (hasItems) {
            if (mode == 'D') { 
                cout << "Which item number do you want to delete? ";
            }
            else if (mode == 'R') {
                cout << "Which item number do you want to return? ";
            }

            string targetItem;
            getline(cin, targetItem);

            if (targetItem == "x" || targetItem == "X") {
                if (mode == 'D') {
                    cout << "Cancelled Deleting..." << endl;
                    break;
                }
                else if (mode == 'R') {
                    cout << "Cancelled Return..." << endl;
                    break;
                }
                
                break;
            }

            int targetNumber;
            
            try {
                targetNumber = stoi(targetItem); 
            } 
            catch (...) {
                cout << "Invalid input. Please enter a valid number." << endl;
                continue;
            }

            string itemFound = myCart.findItemName(targetNumber);

            if (!(itemFound == "")) {
                char confirm;
                
                while (true) {
                    cout << "Remove " << itemFound << "? (Y/N): ";
                    cin >> confirm;
                    confirm = toupper(confirm);

                    if (confirm == 'Y' || confirm == 'N') {
                        break;
                    }
                    else {
                        cout << "Invalid input. Please try again.";
                        continue;
                    }
                }

                if (confirm == 'Y') {
                    myCart.deleteItem(targetNumber);
                }
                else {
                    cout << "Removing Cancelled. " << endl;
                    continue;
                }
            }
        }
        else {
            cout << "Please add items first. ";
            waitForEnter();
            cout << endl;
            break;
        }
    }
}

// Master state machine loop that controls user navigation and actions inside the Plan, Shop, and Checkout rooms.
void runActiveMode(char mode, CartHub& myCart) {
    char currentMode = toupper(mode), action;

    while (true) {
        if (currentMode == 'P') showPlanMode(myCart);
        else if (currentMode == 'S') showShoppingMode(myCart);
        else if (currentMode == 'C') showCheckoutMode(myCart);

        while (true) {
            cout << "What would you like to do?: ";
            cin >> action;
            cin.ignore();
            action = toupper(action);

            if (currentMode == 'P' && (action == 'A' || action == 'S' || action == 'D' || action == 'B')) break;
            else if (currentMode == 'S' && (action == 'U' || action == 'P' || action == 'R' || action == 'C' || action == 'B')) break;
            else if (currentMode == 'C' && (action == 'U' || action == 'S' || action == 'R' || action == 'C' || action == 'B')) break;
            
            cout << "Invalid command. Please try again." << endl;
        }

        cout << endl;
        
        if (currentMode == 'P') {
            if (action == 'A') promptAddItems(myCart);
            else if (action == 'D') promptRemoveItem(myCart, 'D');
            else if (action == 'S') currentMode = 'S';
            else if (action == 'B') return;
        } 
        else if (currentMode == 'S') {
            if (action == 'U') promptEditItem(myCart);
            else if (action == 'P') currentMode = 'P';
            else if (action == 'R') promptRemoveItem(myCart, 'R');
            else if (action == 'C') currentMode = 'C';
            else if (action == 'B') return;
        }
        else if (currentMode == 'C') {
            if (action == 'U') promptEditItem(myCart);
            else if (action == 'S') currentMode = 'S';
            else if (action == 'R') promptRemoveItem(myCart, 'R');
            else if (action == 'C') promptCheckoutCart(myCart);
            else if (action == 'B') return;
        }
    }
}

// The root application loop that keeps the program running and routes the user to different active modes.
void runMainMenu(CartHub& myCart) {
    char choice;
    while (true) {
        displayMainMenu(myCart);
        cout << "What would you like to do?: ";
        cin >> choice;
        choice = toupper(choice);
        
        if (choice == 'P') {
            runActiveMode('P', myCart);
        }
        else if (choice == 'S') {
            runActiveMode('S', myCart);
        }
        else if (choice == 'C') {
            runActiveMode('C', myCart);
        }
        else if (choice == 'E') {
            myCart.clearList();
        }
        else if (choice == 'Q') {
            cout << "\nSaving your grocery list..." << endl;
            delay(800); // pause for 0.8 second
            
            myCart.saveData();
            
            cout << "Successfully saved to carthub_save.txt." << endl;
            delay(500);
            
            cout << "Exiting CartHub. Thank you and Goodbye!" << endl;
            break;
        }
    }
}

// Renders a 10-option numbered menu for grocery categories, validates the input, and returns the chosen category string.
string selectCategory() {
    int catChoice;
    string category;
    
    cout << "\nSelect Category:" << endl;
    cout << left << setw(30) << "[1] Produce (Fruits & Veg)" << setw(30) << "[2] Meat & Seafood" << setw(30) << "[3] Dairy & Eggs" << endl;
    cout << left << setw(30) << "[4] Bakery & Bread" << setw(30) << "[5] Pantry & Dry Goods" << setw(30) << "[6] Beverages" << endl;
    cout << left << setw(30) << "[7] Snacks & Sweets" << setw(30) << "[8] Personal Care & Hygiene" << setw(30) << "[9] Household & Cleaning" << endl;
    cout << "[10] Custom / Other" << endl;
    
    while (true) {
        cout << "Choice (1-10): ";
        cin >> catChoice;
        
        if (cin.fail() || catChoice < 1 || catChoice > 10) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid choice. Please enter a number between 1 and 10." << endl;
        } else {
            cin.ignore(10000, '\n');
            break;
        }
    }

    // Map the number to the string
    switch (catChoice) {
        case 1: category = "Produce"; break;
        case 2: category = "Meat"; break;
        case 3: category = "Dairy"; break;
        case 4: category = "Bakery"; break;
        case 5: category = "Pantry"; break;
        case 6: category = "Beverage"; break;
        case 7: category = "Snacks"; break;
        case 8: category = "Hygiene"; break;
        case 9: category = "Household"; break;
        case 10: 
            cout << "Type custom category name: ";
            getline(cin >> ws, category);
            category[0] = toupper(static_cast<unsigned char>(category[0])); 
            break;
    }
    
    return category;
}

// Renders the header and available hotkeys for the Planning room.
void showPlanMode(CartHub& myCart) {
    cout << "\n=============== PLANNING GROCERY LIST ==============" << endl;
    myCart.displayCart('D');
    cout << "----------------------------------------------------" << endl;
    cout << left << setw(25) << "[A] Fast-Add Items" << right << setw(25) << "[D] Delete a Mistake" << endl;
    cout << left << setw(25) << "[S] Drive to Store ->" << right << setw(25) << "[B] Back to Menu" << endl;      
    cout << "----------------------------------------------------" << endl;
}

// Renders the header and available hotkeys for the Live Shopping room.
void showShoppingMode(CartHub& myCart) {
    cout << "\n=================== LIVE SHOPPING ==================" << endl;
    myCart.displayCart();
    cout << "----------------------------------------------------" << endl;
    cout << left << setw(25) << "[U] Update an Item" << right << setw(25) << "[R] Return to Shelf" << endl;
    cout << left << setw(25) << "[P] <- Back to Planning" << right << setw(25) << "[C] Checkout" << endl;      
    cout << "----------------------------------------------------" << endl;
}

// Renders the header and available hotkeys for the Checkout room.
void showCheckoutMode(CartHub& myCart) {
    cout << "\n=================== CHECKING OUT ===================" << endl;
    myCart.displayCart('C');
    cout << "----------------------------------------------------" << endl;
    cout << left << setw(25) << "[U] Update an Item" << right << setw(25) << "[R] Return to Shelf" << endl;
    cout << left << setw(25) << "[S] <- Back to Shopping" << right << setw(25) << "[C] Confirm & Pay" << endl;      
    cout << "----------------------------------------------------" << endl;
}

// ==========================================
// Helper Utility Methods
// ==========================================

// Pauses the execution thread for a specified number of milliseconds.
void delay(int milliseconds) {
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

// Freezes the console and waits for the user to press the Enter key before continuing.
void waitForEnter() {
    string input;
    getline(cin, input);
}
