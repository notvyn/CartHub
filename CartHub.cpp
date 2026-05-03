#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cctype>
#include <ctime>
using namespace std;

// enum cartField { name = 1, category, quantity, price };

struct cartProperty {
    string itemName;
    string category;
    int quantity;
    double price;
};

struct Node {
    string itemName;
    string category;
    int quantity;
    double price;
    Node* next;
    Node* prev;

    Node(string name) {
        itemName = name;
        // category = "";
        quantity = 0;
        price = 0;
        next = nullptr;
        prev = nullptr;
    }
};

class CartHub {
private:
    Node* head;
    Node* tail;

public:
    CartHub () {
        head = nullptr;
        tail = nullptr;
    }

    void addItem(string name, string category, int quantity, double price = 0.00) {
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

    bool displayCart(char mode = '\0') {
        Node* temp = head;
        int counter = 0;
        bool hasItems;

        if (temp == nullptr) {
            // << right << setw(41)
            cout << "Your cart is currently empty." << endl;
            return hasItems = false;
        }
        else {
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

                return hasItems = true;
            }
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
                return hasItems = true;
            }
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
                return hasItems = true;
            }
        }
    }

    string searchItem(int targetItem) {
        Node* temp = head;
        int currentNumber = 1;
        string itemFound;

        while (temp != nullptr && currentNumber != targetItem) {
            temp = temp->next;
            currentNumber++;
        }

        if (temp == nullptr) {
            cout << "No items on list yet." << endl;
            return itemFound = "";
        }

        itemFound = temp->itemName;

        return itemFound;
    }

    cartProperty getItem(int targetItem) {
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

    void editItem(int targetNumber, int fieldToEdit, string userInput) {
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
                double newPrice = stod(userInput);
                temp->price = newPrice;
                cout << "Successfully changed product price." << endl;
                break;
            }
            case 2: {
                int newQuantity = stoi(userInput);
                temp->quantity = newQuantity;
                cout << "Successfully changed product quantity." << endl;
                break;
            }
        }
        
        cout << endl;
    }

    void deleteItem(int targetItem) {
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
        
        if (head == tail) {
            head = tail = nullptr;
        }
        else if (temp == head) {
            head = temp->next;
            head->prev = nullptr;
        }
        else if (temp == tail) {
            tail = temp->prev;
            tail->next = nullptr;
        }
        else {
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
        }

        cout << "Successfully removed " << temp->itemName << "." << endl;

    }

    void clearList() {
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

    void generateReceipt(string receiptName) {
        // 1. Create and open a text file named "receipt.txt"
        ofstream receiptFile(receiptName + ".txt");

        // 2. Safety check: Did the file actually open?
        if (!receiptFile.is_open()) {
            cout << "Error: Could not open file to save receipt!" << endl;
            return;
        }

        // 3. Write the header to the FILE (notice we use receiptFile instead of cout)
        receiptFile << "************** CARTHUB RECEIPT SUMMARY ***************" << endl;

        time_t now = time(0);
        tm* ltm = localtime(&now);

        // 2. Create a character array to hold our formatted text
        char dateBuffer[80];

        // 3. Format the time! 
        strftime(dateBuffer, sizeof(dateBuffer), "Week %U - %b, %Y", ltm);

        receiptFile << "----------------- " << dateBuffer << " ----------------" << endl;
        
        Node* temp = head;
        int counter = 0;
        double totalSpent = 0.0;

        // 4. Walk down the train and write each car to the file
        while (temp != nullptr) {
            counter += 1;
            string productName = temp->itemName + " (" + temp->category + ")";
            receiptFile << left << setw(4) << counter << left << setw(25) << productName << "x" << left << setw(4) << temp->quantity << "PHP " << right << setw(10) << fixed << setprecision(2) << temp->price << endl;
            totalSpent += (temp->price * temp->quantity); // Keep a running total
            temp = temp->next;
        }

        // 5. Write the footer and total
        receiptFile << "......................................................" << endl;
        receiptFile << "TOTAL ITEMS: " << left << setw(10) << counter << "AMOUNT DUE: PHP " << fixed << setprecision(2) << totalSpent << endl;
        receiptFile << "======================================================" << endl;

        // 6. CRITICAL: Always close the file when you are done to save it!
        receiptFile.close(); 

        cout << "\nSuccess! Checkout complete. Receipt saved to " << receiptName << ".txt." << endl;
        clearList();
    }
};

void changeState(CartHub&);
void changeMode(char, CartHub&);
void createList(CartHub&);
void deleteList(CartHub&, char);
void editList(CartHub&);

void displayMainMenu(CartHub& myCart) {
    cout << "\n=============== WELCOME TO CARTHUB ! ===============" << endl;
    myCart.displayCart();
    cout << "====================================================" << endl; 
    cout << "[P] Plan Grocery List (Add/Delete Items)" << endl;
    cout << "[S] Start Shopping (Update Quantity/Prices)" << endl;
    cout << "[C] Checkout & Save Receipt" << endl;
    cout << "[E] Empty Cart (Start Over)" << endl;
    cout << "[Q] Quit Application" << endl;
    cout << "----------------------------------------------------" << endl;
}

void showPlanMode(CartHub& myCart) {
    cout << "\n=============== PLANNING GROCERY LIST ==============" << endl;
    myCart.displayCart('D');
    cout << "----------------------------------------------------" << endl;
    cout << left << setw(25) << "[A] Fast-Add Items" << right << setw(25) << "[D] Delete a Mistake" << endl;
    cout << left << setw(25) << "[S] Drive to Store ->" << right << setw(25) << "[B] Back to Menu" << endl;      
    cout << "----------------------------------------------------" << endl;
}

void showShoppingMode(CartHub& myCart) {
    cout << "\n=================== LIVE SHOPPING ==================" << endl;
    myCart.displayCart();
    cout << "----------------------------------------------------" << endl;
    cout << left << setw(25) << "[U] Update an Item" << right << setw(25) << "[R] Return to Shelf" << endl;
    cout << left << setw(25) << "[P] <- Back to Planning" << right << setw(25) << "[C] Checkout" << endl;      
    cout << "----------------------------------------------------" << endl;
}

void showCheckoutMode(CartHub& myCart) {
    cout << "\n=================== CHECKING OUT ===================" << endl;
    myCart.displayCart('C');
    cout << "----------------------------------------------------" << endl;
    cout << left << setw(25) << "[U] Update an Item" << right << setw(25) << "[R] Return to Shelf" << endl;
    cout << left << setw(25) << "[S] <- Back to Shopping" << right << setw(25) << "[C] Confirm & Pay" << endl;      
    cout << "----------------------------------------------------" << endl;
}

void changeState(CartHub& myCart) {
    char choice;
    while (true) {
        displayMainMenu(myCart);
        cout << "What would you like to do?: ";
        cin >> choice;
        choice = toupper(choice);
        
        if (choice == 'P') {
            changeMode('P', myCart);
        }
        else if (choice == 'S') {
            changeMode('S', myCart);
        }
        else if (choice == 'C') {
            changeMode('C', myCart);
        }
        else if (choice == 'E') {
            myCart.clearList();
        }
        else if (choice == 'Q') {
            cout << "Exiting CartHub. Goodbye!" << endl;
            break;
        }
    }
}

void changeMode(char mode, CartHub& myCart) {
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
            else if (currentMode == 'S' && (action == 'U' || action == 'P' || action == 'R' || action == 'C')) break;
            else if (currentMode == 'C' && (action == 'U' || action == 'S' || action == 'R' || action == 'C')) break;
            
            cout << "Invalid command. Please try again." << endl;
        }

        cout << endl;
        
        if (currentMode == 'P') {
            if (action == 'A') createList(myCart);
            else if (action == 'D') deleteList(myCart, 'D');
            else if (action == 'S') currentMode = 'S';
            else if (action == 'B') return;
        } 
        else if (currentMode == 'S') {
            if (action == 'U') editList(myCart);
            else if (action == 'P') currentMode = 'P';
            else if (action == 'R') deleteList(myCart, 'R');
            else if (action == 'C') currentMode = 'C';
        }
        else if (currentMode == 'C') {
            if (action == 'U') editList(myCart);
            else if (action == 'S') currentMode = 'S';
            else if (action == 'R') deleteList(myCart, 'R');
            else if (action == 'C') {
                cout << "--- FINALIZING PURCHASE ---" << endl;
                
                // 1. Get the current system time
                time_t now = time(0);
                tm* ltm = localtime(&now);

                // 2. Create a character array to hold our formatted text
                char dateBuffer[80];

                // 3. Format the time! 
                strftime(dateBuffer, sizeof(dateBuffer), "Receipt_Week_%U_%b_%Y", ltm);

                // 4. Convert the char array back into a standard C++ string
                string receiptName(dateBuffer);
                
                // 5. Save the file and exit!
                myCart.generateReceipt(receiptName);
                return;
            }
        }
    }
}

void createList(CartHub& myCart) {
    while (true) {
        bool hasItems = myCart.displayCart('D');
        cout << "\n******** ADDING ITEMS (Type '0' for name to stop) ********" << endl;

        string itemName, category;
        int quantity;

        cout << "Enter product name: ";
        getline(cin, itemName);
        
        if (itemName == "0") {
            cout << "List Completed. Proceed to Live Shopping." << endl;
            break;
        }

        cout << "Enter category: ";
        getline(cin, category);
        
        cout << "Enter quantity: ";
        cin >> quantity;
        cin.ignore();

        itemName[0] = toupper(static_cast<unsigned char>(itemName[0]));
        category[0] = toupper(static_cast<unsigned char>(category[0]));        

        // Add to List
        myCart.addItem(itemName, category, quantity);
        cout << "Successfully added " << quantity << " " << itemName << " on " << category << " category." << endl << endl;
    }
}

void viewList(CartHub& myCart) {
    while (true) {
        myCart.displayCart();
        cout << "Press Enter to exit... ";

        string input;
        getline(cin, input); // Waits for the user to press Enter

        cout << endl;
        break;
    }
}

void editList(CartHub& myCart) {
    while (true) {    
        bool hasItems = myCart.displayCart();
        cout << "\n******** EDITING ITEMS (Type 'x' to cancel) *******" << endl;

        if (hasItems) {
            cout << "Which item number do you want to edit?: ";
            
            string targetItem;
            cin >> targetItem;

            if (targetItem == "x" || targetItem == "X") {
                cout << "Exiting editing mode." << endl;
                showShoppingMode(myCart);
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

            string itemFound = myCart.searchItem(targetNumber);

            if (!(itemFound == "")) {
                cartProperty selectedItem = myCart.getItem(targetNumber);

                cout << "--- Editing: " << selectedItem.itemName << " (" << selectedItem.category << ") ---" << endl;
                cout << "Current Price: PHP " << fixed << setprecision(2) << selectedItem.price << " | Qty: " << selectedItem.quantity << endl << endl; 

                int choice;

                cout << "What would you like to update?" << endl;
                cout << "[1] Price" << endl;
                cout << "[2] Quantity" << endl;

                do {
                    cout << "Choice: ";
                    cin >> choice;
                } while (choice < 1 || choice > 2);

                string userInput;
                getline(cin, userInput);

                switch (choice) {
                    case 1:
                        cout << "Change price to: ";
                        getline(cin, userInput);
                        myCart.editItem(targetNumber, choice, userInput);
                        break;
                    case 2:
                        cout << "Change quantity to: ";
                        getline(cin, userInput);
                        myCart.editItem(targetNumber, choice, userInput);
                        break;                        
                }
            }
        }
        else {
            cout << "Please add items first. ";
            string input;
            getline(cin, input); // Waits for the user to press Enter

            cout << endl;
            break;
        }
    }
}

void deleteList(CartHub& myCart, char mode) {
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
                    showPlanMode(myCart);
                }
                else if (mode == 'R') {
                    cout << "Cancelled Return..." << endl;
                    showShoppingMode(myCart);
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

            string itemFound = myCart.searchItem(targetNumber);

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
            string input;
            getline(cin, input); // Waits for the user to press Enter

            cout << endl;
            break;
        }
    }
}

void checkoutList(CartHub& myCart) {
    while (true) {
        bool hasItems = myCart.displayCart();
        
        if (hasItems) {
            cout << "Proceed to check out? (Y/N): ";

            string checkout;
            getline(cin, checkout);

            if (toupper(checkout[0]) == 'N') {
                cout << "Exiting checkout mode." << endl;
                break;
            }
            else if (toupper(checkout[0]) == 'Y') {
                string receiptName;
                cout << "Enter Receipt Name: ";
                getline(cin, receiptName);

                myCart.generateReceipt(receiptName); // Print the file!
                break; // Exit the loop
            }
            else {
                cout << "Invalid input. Please enter Y or N." << endl;
            }
        }
        else {
            // If the cart is empty, don't trap them in a checkout loop!
            cout << "Your cart is empty. Returning to main menu." << endl;
            break; 
        }
    }
}

int main() {
    CartHub myCart;

    myCart.addItem("Milk", "Beverage", 1);
    myCart.addItem("Egg", "Food", 6);
    myCart.addItem("Chicken", "Food", 1);

    changeState(myCart);

    // userAction(myCart);

    
    

    // myCart.displayCart();

    // myCart.editItem("Egg");

    // myCart.displayCart();

    // myCart.deleteItem("Milk");

    // myCart.displayCart();

    // myCart.deleteItem("Egg");
    // myCart.deleteItem("Chicken");

    // myCart.displayCart();
}