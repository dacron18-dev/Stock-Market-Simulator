#include <iostream>
#include <string>
#include <cctype>
using namespace std;

class User {
protected:
    string name;
    string email;
    string phone;
    int userID;

public:
    User(string n, string e, string p, int id) {
        name = n;
        email = e;
        phone = p;
        userID = id;
    }

    void displayUserDetails() {
        cout << "\nUSER DETAILS \n";
        cout << "Name    : " << name << endl;
        cout << "Email   : " << email << endl;
        cout << "Phone   : " << phone << endl;
        cout << "User ID : " << userID << endl;
    }
};


class Portfolio {
private:
    double value;

public:
    Portfolio(double v) {
        value = v;
    }

    // Operator Overloading
    Portfolio operator+(Portfolio p) {
        return Portfolio(value + p.value);
    }

    void display() {
        cout << "Portfolio Value: Rs. " << value << endl;
    }
};


class Investor : public User {
private:
    double virtualMoney;

public:
    // Constructor
    Investor(string n, string e, string p, int id, double money)
        : User(n, e, p, id) {
        virtualMoney = money;
    }

    // Function Overloading
    void verifyDetails(string inputEmail) {
        if (inputEmail == email)
            cout << "Email verified successfully!" << endl;
        else
            cout << "Email verification failed!" << endl;
    }

    void verifyDetails(string inputEmail, string inputPhone) {
        if (inputEmail == email && inputPhone == phone)
            cout << "Email and phone verified successfully!" << endl;
        else
            cout << "Verification failed!" << endl;
    }


    // Buy stock using price and quantity
    void buyStock(string stockName, double price, int quantity) {

        if (price <= 0) {
            cout << "Invalid stock price!" << endl;
            return;
        }

        if (quantity <= 0) {
            cout << "Invalid quantity!" << endl;
            return;
        }

        double cost = price * quantity;

        cout << "\nTRANSACTION" << endl;
        cout << "Stock       : " << stockName << endl;
        cout << "Price       : Rs. " << price << endl;
        cout << "Quantity    : " << quantity << endl;
        cout << "Total Cost  : Rs. " << cost << endl;

        // Check account balance
        if (cost <= virtualMoney) {

            virtualMoney -= cost;

            cout << "Transaction Status : SUCCESS" << endl;
            cout << "Remaining Balance  : Rs. "
                 << virtualMoney << endl;

        } else {

            cout << "Transaction Status : FAILED" << endl;
            cout << "Reason: Insufficient virtual money!" << endl;

            cout << "Available Balance : Rs. "
                 << virtualMoney << endl;

            cout << "Required Amount   : Rs. "
                 << cost << endl;
        }
    }


    // Function Overloading
    // Buy stock when price is not entered separately
    // Here we use a default market price for demonstration
    void buyStock(string stockName, int quantity) {

        double price;

        cout << "\nEnter current price of " << stockName << ": ";
        cin >> price;

        buyStock(stockName, price, quantity);
    }


    // Display current balance
    void displayBalance() {
        cout << "\nAvailable Virtual Money: Rs. "
             << virtualMoney << endl;
    }
};


// Function to validate phone number
bool isValidPhone(string phone) {

    // Phone number must have exactly 10 digits
    if (phone.length() != 10)
        return false;

    // Every character must be a digit
    for (char ch : phone) {
        if (!isdigit(ch))
            return false;
    }

    return true;
}


int main() {

    string name, email, phone;
    int userID;
    double money;

    cout << "STOCK MARKET SIMULATOR" << endl;


    // USER REGISTRATION

    cout << "\nEnter your name: ";
    getline(cin, name);

    cout << "Enter your email: ";
    getline(cin, email);


    // Phone validation
    do {
        cout << "Enter your 10-digit phone number: ";
        cin >> phone;

        if (!isValidPhone(phone)) {
            cout << "Invalid phone number!" << endl;
            cout << "Phone number must contain exactly 10 digits.\n";
        }

    } while (!isValidPhone(phone));


    cout << "Enter your User ID: ";
    cin >> userID;


    // Validate virtual money
    do {
        cout << "Enter your virtual money: Rs. ";
        cin >> money;

        if (money < 0) {
            cout << "Balance cannot be negative!" << endl;
        }

    } while (money < 0);


    // Create Investor
    Investor investor(name, email, phone, userID, money);


    // USER DETAILS 

    investor.displayUserDetails();


    // VERIFICATION

    string inputEmail;
    string inputPhone;

    cout << "\nVERIFICATION" << endl;

    cout << "Enter email for verification: ";
    cin >> inputEmail;

    investor.verifyDetails(inputEmail);


    cout << "\nEnter email again: ";
    cin >> inputEmail;

    cout << "Enter phone number: ";
    cin >> inputPhone;

    investor.verifyDetails(inputEmail, inputPhone);


    //STOCK PURCHASE

    cout << "\n BUY STOCK" << endl;

    string stockName;
    double price;
    int quantity;

    cout << "Enter stock name: ";
    cin >> stockName;

    cout << "Enter stock price: Rs. ";
    cin >> price;

    cout << "Enter quantity: ";
    cin >> quantity;

    investor.buyStock(stockName, price, quantity);


    // Show updated balance
    investor.displayBalance();


    // SECOND PURCHASE 

    cout << "\nSECOND PURCHASE" << endl;

    cout << "Enter another stock name: ";
    cin >> stockName;

    cout << "Enter quantity: ";
    cin >> quantity;

    // Function overloading
    investor.buyStock(stockName, quantity);


    // Show updated balance
    investor.displayBalance();


    //  PORTFOLIO 

    cout << "\nPORTFOLIO" << endl;

    double value1, value2;

    cout << "Enter value of Portfolio 1: Rs. ";
    cin >> value1;

    cout << "Enter value of Portfolio 2: Rs. ";
    cin >> value2;

    Portfolio p1(value1);
    Portfolio p2(value2);

    // Operator overloading
    Portfolio p3 = p1 + p2;

    cout << "\nCombined Portfolio:" << endl;
    p3.display();


    cout << " THANK YOU FOR USING SIMULATOR" << endl;
    

    return 0;
}

