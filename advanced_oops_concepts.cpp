#include <iostream>
#include <string>
using namespace std;

// Parent class
class User {
protected:
    string name;
    int userId;

public:
    User(string n, int id) {
        name = n;
        userId = id;
    }

    virtual void showDetails() {
        cout << "Name: " << name << endl;
        cout << "User ID: " << userId << endl;
    }
};


// Trader class
class Trader : public User {

private:
    double money;

    // Shared by all traders
    static int traderCount;

public:

    Trader(string n, int id, double m)
        : User(n, id) {
        money = m;
        traderCount++;
    }

    // Overloading << operator
    friend ostream& operator<<(ostream& out, Trader& trader) {

        out << "Trader Name: " << trader.name << endl;
        out << "Trader ID: " << trader.userId << endl;
        out << "Available Money: Rs. " << trader.money << endl;

        return out;
    }

    // Static function
    static void showTraderCount() {
        cout << "Total Traders: " << traderCount << endl;
    }

    void showDetails() override {
        cout << "Trader Name: " << name << endl;
        cout << "Trader ID: " << userId << endl;
        cout << "Available Money: Rs. " << money << endl;
    }
};


// Static variable definition
int Trader::traderCount = 0;


// Stock class
class Stock final {

private:
    string stockName;
    double stockPrice;

public:

    Stock(string name, double price) {
        stockName = name;
        stockPrice = price;
    }

    void showStock() {
        cout << "Stock Name: " << stockName << endl;
        cout << "Stock Price: Rs. " << stockPrice << endl;
    }
};


// Transaction class
class Transaction {

private:
    int transactionId;
    double amount;

public:

    Transaction(int id, double amt) {
        transactionId = id;
        amount = amt;
    }

    // Nested class
    class TransactionDetails {

    public:
        string type;
        string date;

        void showDetails() {
            cout << "Transaction Type: " << type << endl;
            cout << "Transaction Date: " << date << endl;
        }
    };

    void showTransaction() {
        cout << "Transaction ID: " << transactionId << endl;
        cout << "Transaction Amount: Rs. "
             << amount << endl;
    }
};


int main() {

    cout << "===== STOCK MARKET SIMULATOR =====" << endl;
    cout << endl;


    // Creating traders
    Trader trader1("Rahul", 101, 50000);
    Trader trader2("Aman", 102, 75000);


    // Displaying trader using << operator
    cout << "----- Trader Details -----" << endl;
    cout << trader1;


    cout << endl;


    // Display total number of traders
    cout << "----- Trader Count -----" << endl;
    Trader::showTraderCount();


    cout << endl;


    // Creating a stock
    cout << "----- Stock Details -----" << endl;

    Stock reliance("RELIANCE", 1450.50);

    reliance.showStock();


    cout << endl;


    // Creating transaction details
    cout << "----- Transaction Details -----" << endl;

    Transaction::TransactionDetails details;

    details.type = "BUY";
    details.date = "17-09-2026";

    details.showDetails();


    cout << endl;


    // Object slicing
    cout << "----- Object Slicing -----" << endl;

    Trader trader3("Priya", 103, 60000);

    User user = trader3;

    user.showDetails();


    return 0;
}
