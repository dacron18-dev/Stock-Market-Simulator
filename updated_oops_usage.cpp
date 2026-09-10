#include <iostream>
#include <string>
#include <cctype>
using namespace std;


// ============================================================
// USER CLASS
// ============================================================

class User {

protected:
    // ENCAPSULATION:
    // Data members are kept inside the class.
    // 'protected' allows derived classes like Investor
    // to access these members.

    string name;
    string email;
    string phone;
    int userID;


public:

    // CONSTRUCTOR:
    // Automatically initializes the User object.
    User(string n, string e, string p, int id) {

        name = n;
        email = e;
        phone = p;
        userID = id;
    }


    // MEMBER FUNCTION
    void displayUserDetails() {

        cout << "\nUSER DETAILS \n";
        cout << "Name    : " << name << endl;
        cout << "Email   : " << email << endl;
        cout << "Phone   : " << phone << endl;
        cout << "User ID : " << userID << endl;
    }
};


// ============================================================
// PORTFOLIO CLASS
// ============================================================

class Portfolio {

private:

    // ENCAPSULATION:
    // 'value' is private, so it cannot be accessed directly
    // from outside the Portfolio class.

    double value;


public:

    // CONSTRUCTOR
    Portfolio(double v) {

        value = v;
    }


    // FRIEND FUNCTION
    //
    // OPERATOR OVERLOADING:
    // The '+' operator is overloaded for Portfolio objects.
    //
    // FRIEND FUNCTION:
    // This function is not a member of Portfolio,
    // but it can access private members of Portfolio.
    
    friend Portfolio operator+(const Portfolio& p1,
                               const Portfolio& p2);


    // MEMBER FUNCTION
    void display() {

        cout << "Portfolio Value: Rs. "
             << value << endl;
    }
};


// ============================================================
// FRIEND FUNCTION DEFINITION
// ============================================================

// OPERATOR OVERLOADING USING FRIEND FUNCTION
//
// When we write:
//
//     p1 + p2
//
// C++ calls:
//
//     operator+(p1, p2)
//
// This function accesses the private 'value' of both
// Portfolio objects.

Portfolio operator+(const Portfolio& p1,
                    const Portfolio& p2) {

    return Portfolio(p1.value + p2.value);
}


// ============================================================
// INVESTOR CLASS
// ============================================================

// INHERITANCE:
//
// Investor inherits from User.
//
// User = Base Class
// Investor = Derived Class
//
// Therefore Investor can use the protected members of User.

class Investor : public User {

private:

    // ENCAPSULATION:
    // virtualMoney is private and can only be accessed
    // through functions of Investor.

    double virtualMoney;


public:

    // CONSTRUCTOR
    //
    // Constructor of Investor calls the constructor
    // of the base class User.
    
    Investor(string n, string e, string p,
             int id, double money)
        : User(n, e, p, id) {

        virtualMoney = money;
    }


    // ========================================================
    // FUNCTION OVERLOADING
    // ========================================================

    // Same function name: verifyDetails()
    // Different number of parameters.

    void verifyDetails(string inputEmail) {

        if (inputEmail == email)
            cout << "Email verified successfully!" << endl;
        else
            cout << "Email verification failed!" << endl;
    }


    // FUNCTION OVERLOADING
    //
    // Same function name but two parameters instead of one.

    void verifyDetails(string inputEmail,
                       string inputPhone) {

        if (inputEmail == email &&
            inputPhone == phone)

            cout << "Email and phone verified successfully!"
                 << endl;

        else
            cout << "Verification failed!" << endl;
    }


    // ========================================================
    // FUNCTION OVERLOADING - buyStock()
    // ========================================================

    // First version:
    // Takes stock name, price and quantity.

    void buyStock(string stockName,
                  double price,
                  int quantity) {

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

        cout << "Stock       : "
             << stockName << endl;

        cout << "Price       : Rs. "
             << price << endl;

        cout << "Quantity    : "
             << quantity << endl;

        cout << "Total Cost  : Rs. "
             << cost << endl;


        // Check account balance

        if (cost <= virtualMoney) {

            virtualMoney -= cost;

            cout << "Transaction Status : SUCCESS"
                 << endl;

            cout << "Remaining Balance  : Rs. "
                 << virtualMoney << endl;
        }

        else {

            cout << "Transaction Status : FAILED"
                 << endl;

            cout << "Reason: Insufficient virtual money!"
                 << endl;

            cout << "Available Balance : Rs. "
                 << virtualMoney << endl;

            cout << "Required Amount   : Rs. "
                 << cost << endl;
        }
    }


    // FUNCTION OVERLOADING
    //
    // Same function name: buyStock()
    // But this version takes only stock name and quantity.

    void buyStock(string stockName,
                  int quantity) {

        double price;


        cout << "\nEnter current price of "
             << stockName << ": ";

        cin >> price;


        // Calls the other overloaded buyStock()
        buyStock(stockName, price, quantity);
    }


    // MEMBER FUNCTION
    // Displays the investor's virtual balance.

    void displayBalance() {

        cout << "\nAvailable Virtual Money: Rs. "
             << virtualMoney << endl;
    }
};


// ============================================================
// PHONE VALIDATION FUNCTION
// ============================================================

// This is a normal function outside a class.
// It checks whether the phone number contains exactly
// 10 digits.

bool isValidPhone(string phone) {

    if (phone.length() != 10)
        return false;


    // Check every character

    for (char ch : phone) {

        if (!isdigit(ch))
            return false;
    }


    return true;
}


// ============================================================
// MAIN FUNCTION
// ============================================================

int main() {

    string name, email, phone;
    int userID;
    double money;


    cout << "======================================" << endl;
    cout << "       STOCK MARKET SIMULATOR" << endl;
    cout << "======================================" << endl;


    // ========================================================
    // USER REGISTRATION
    // ========================================================

    cout << "\nEnter your name: ";
    getline(cin, name);


    cout << "Enter your email: ";
    getline(cin, email);


    // PHONE VALIDATION

    do {

        cout << "Enter your 10-digit phone number: ";
        cin >> phone;


        if (!isValidPhone(phone)) {

            cout << "Invalid phone number!" << endl;

            cout << "Phone number must contain exactly "
                 << "10 digits.\n";
        }

    } while (!isValidPhone(phone));


    cout << "Enter your User ID: ";
    cin >> userID;


    // VALIDATE VIRTUAL MONEY

    do {

        cout << "Enter your virtual money: Rs. ";
        cin >> money;


        if (money < 0) {

            cout << "Balance cannot be negative!"
                 << endl;
        }

    } while (money < 0);


    // ========================================================
    // OBJECT CREATION
    // ========================================================

    // OBJECT:
    // 'investor' is an object of the Investor class.

    Investor investor(
        name,
        email,
        phone,
        userID,
        money
    );


    // ========================================================
    // USER DETAILS
    // ========================================================

    investor.displayUserDetails();


    // ========================================================
    // FUNCTION OVERLOADING - verifyDetails()
    // ========================================================

    string inputEmail;
    string inputPhone;


    cout << "\nVERIFICATION" << endl;


    // Calls verifyDetails(string)

    cout << "Enter email for verification: ";
    cin >> inputEmail;

    investor.verifyDetails(inputEmail);


    // Calls verifyDetails(string, string)

    cout << "\nEnter email again: ";
    cin >> inputEmail;


    cout << "Enter phone number: ";
    cin >> inputPhone;


    investor.verifyDetails(
        inputEmail,
        inputPhone
    );


    // ========================================================
    // STOCK PURCHASE
    // ========================================================

    cout << "\nBUY STOCK" << endl;


    string stockName;
    double price;
    int quantity;


    cout << "Enter stock name: ";
    cin >> stockName;


    cout << "Enter stock price: Rs. ";
    cin >> price;


    cout << "Enter quantity: ";
    cin >> quantity;


    // Calls buyStock(string, double, int)

    investor.buyStock(
        stockName,
        price,
        quantity
    );


    // Show updated balance

    investor.displayBalance();


    // ========================================================
    // SECOND PURCHASE
    // ========================================================

    cout << "\nSECOND PURCHASE" << endl;


    cout << "Enter another stock name: ";
    cin >> stockName;


    cout << "Enter quantity: ";
    cin >> quantity;


    // FUNCTION OVERLOADING
    //
    // Calls buyStock(string, int)
    // instead of buyStock(string, double, int).

    investor.buyStock(
        stockName,
        quantity
    );


    investor.displayBalance();


    // ========================================================
    // PORTFOLIO
    // ========================================================

    cout << "\nPORTFOLIO" << endl;


    double value1, value2;


    cout << "Enter value of Portfolio 1: Rs. ";
    cin >> value1;


    cout << "Enter value of Portfolio 2: Rs. ";
    cin >> value2;


    // OBJECT CREATION

    Portfolio p1(value1);
    Portfolio p2(value2);


    // ========================================================
    // OPERATOR OVERLOADING USING FRIEND FUNCTION
    // ========================================================

    // '+' operator has been overloaded for Portfolio.
    //
    // p1 + p2
    //
    // internally calls:
    //
    // operator+(p1, p2)
    //
    // The friend function combines their values.

    Portfolio p3 = p1 + p2;


    cout << "\nCombined Portfolio:" << endl;

    p3.display();


    // ========================================================
    // END
    // ========================================================

    cout << "\n======================================" << endl;
    cout << "     THANK YOU FOR USING SIMULATOR" << endl;
    cout << "======================================" << endl;


    return 0;
}
