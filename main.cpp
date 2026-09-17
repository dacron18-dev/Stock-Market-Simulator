#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

// STOCK CLASS
// Represents a single stock in the market.
// Marked 'final' -> cannot be inherited from (advanced concept).
class Stock final {
private:
    // Encapsulation + Data Hiding
    string symbol;
    string companyName;
    double currentPrice;
    double previousPrice;

public:
    Stock(string s, string c, double p)
        : symbol(s), companyName(c), currentPrice(p), previousPrice(p) {}

    string getSymbol() const { return symbol; }
    string getCompanyName() const { return companyName; }
    double getPrice() const { return currentPrice; }
    double getPreviousPrice() const { return previousPrice; }

    void updatePrice(double newPrice) {
        if (newPrice > 0) {
            previousPrice = currentPrice;
            currentPrice = newPrice;
        }
    }

    void displayStock() const {
        cout << left << setw(10) << symbol
             << setw(25) << companyName
             << "Rs. " << fixed << setprecision(2)
             << currentPrice << endl;
    }
};

// TRANSACTION CLASS
// Records a single completed trade. Every buy/sell creates one
// of these, which is stored in the trader's transaction history.
class Transaction {
private:
    static int nextTransactionID;
    int transactionID;
    string type;      // "BUY" or "SELL"
    string symbol;
    int quantity;
    double price;
    string date;

public:
    Transaction(string t, string sym, int qty, double p, string d)
        : type(t), symbol(sym), quantity(qty), price(p), date(d) {
        transactionID = nextTransactionID++;
    }

    double getTotalValue() const { return price * quantity; }

    void displayTransaction() const {
        cout << left << setw(6) << transactionID
             << setw(6) << type
             << setw(10) << symbol
             << setw(10) << quantity
             << "Rs. " << fixed << setprecision(2) << setw(12) << getTotalValue()
             << date << endl;
    }
};
int Transaction::nextTransactionID = 1;

// HOLDING CLASS
// How many shares of a particular stock a trader currently owns.
class Holding {
private:
    Stock stock;
    int quantity;

public:
    // Initializer list used because Stock has no default constructor.
    Holding(Stock s, int q) : stock(s), quantity(q) {}

    Stock getStock() const { return stock; }
    string getSymbol() const { return stock.getSymbol(); }
    int getQuantity() const { return quantity; }
    double getValue() const { return stock.getPrice() * quantity; }

    void addQuantity(int q) { if (q > 0) quantity += q; }

    bool reduceQuantity(int q) {
        if (q <= 0 || q > quantity) return false;
        quantity -= q;
        return true;
    }

    void updatePrice(double newPrice) { stock.updatePrice(newPrice); }

    void displayHolding() const {
        cout << left << setw(10) << stock.getSymbol()
             << setw(25) << stock.getCompanyName()
             << setw(12) << quantity
             << "Rs. " << fixed << setprecision(2)
             << getValue() << endl;
    }
};

// PORTFOLIO CLASS
// Tracks all holdings AND the transaction history for a trader.
class Portfolio {
private:
    vector<Holding> holdings;          // Encapsulation: private, changed only via methods
    vector<Transaction> history;

public:
    void addHolding(Stock stock, int quantity) {
        for (auto &h : holdings) {
            if (h.getSymbol() == stock.getSymbol()) {
                h.addQuantity(quantity);
                h.updatePrice(stock.getPrice());
                return;
            }
        }
        holdings.push_back(Holding(stock, quantity));
    }

    bool sellHolding(string symbol, int quantity) {
        for (auto it = holdings.begin(); it != holdings.end(); ++it) {
            if (it->getSymbol() == symbol) {
                if (quantity > it->getQuantity()) return false;
                it->reduceQuantity(quantity);
                if (it->getQuantity() == 0) holdings.erase(it);
                return true;
            }
        }
        return false;
    }

    double getPortfolioValue() const {
        double total = 0;
        for (const auto &h : holdings) total += h.getValue();
        return total;
    }

    int getQuantity(string symbol) const {
        for (const auto &h : holdings) {
            if (h.getSymbol() == symbol) return h.getQuantity();
        }
        return 0;
    }

    void updateStockPrice(string symbol, double newPrice) {
        for (auto &h : holdings) {
            if (h.getSymbol() == symbol) { h.updatePrice(newPrice); return; }
        }
    }

    void recordTransaction(string type, string symbol, int qty, double price, string date) {
        history.push_back(Transaction(type, symbol, qty, price, date));
    }

    void displayPortfolio() const {
        if (holdings.empty()) {
            cout << "\nYour portfolio is empty.\n";
            return;
        }
        cout << "\nYOUR PORTFOLIO\n";
        cout << left << setw(10) << "Symbol"
             << setw(25) << "Company"
             << setw(12) << "Quantity"
             << "Value" << endl;
        cout << "\n";
        for (const auto &h : holdings) h.displayHolding();
        cout << "\n";
        cout << "Total Portfolio Value: Rs. "
             << fixed << setprecision(2) << getPortfolioValue() << endl;
    }

    void displayHistory() const {
        if (history.empty()) {
            cout << "\nNo transactions yet.\n";
            return;
        }
        cout << "\nTRANSACTION HISTORY\n";
        cout << left << setw(6) << "ID" << setw(6) << "Type"
             << setw(10) << "Symbol" << setw(10) << "Qty"
             << setw(16) << "Amount" << "Date" << endl;
        for (const auto &t : history) t.displayTransaction();
    }

    // Operator Overloading: portfolio + newHolding instead of calling addHolding() directly.
    friend Portfolio operator+(const Portfolio &p, const Holding &h);
};

Portfolio operator+(const Portfolio &p, const Holding &h) {
    Portfolio result = p;
    result.addHolding(h.getStock(), h.getQuantity());
    return result;
}

// USER CLASS (base)
class User {
protected:
    string name;
    string email;
    string phone;
    int userID;
    static int nextUserID;   // shared by all Users

public:
    User(string n, string e, string p) : name(n), email(e), phone(p) {
        userID = nextUserID++;
    }

    // virtual -> allows runtime polymorphism / overriding in Trader
    virtual void showDetails() const {
        cout << "\nUSER DETAILS\n";
        cout << "Name : " << name << endl;
        cout << "Email : " << email << endl;
        cout << "Phone : " << phone << endl;
        cout << "User ID : " << userID << endl;
    }

    int getUserID() const { return userID; }

    virtual ~User() {}
};
int User::nextUserID = 1001;

// TRADER CLASS (derived from User)
//  User
//    |
//    v
//  Trader
class Trader : public User {
private:
    double virtualMoney;
    Portfolio portfolio;
    static int traderCount;   // shared by all Traders

public:
    Trader(string n, string e, string p, double money)
        : User(n, e, p), virtualMoney(money) {
        traderCount++;
    }

    // Function Overloading (compile-time polymorphism)
    void verifyDetails(string emailInput) {
        cout << (emailInput == email
                 ? "Email verified successfully.\n"
                 : "Email verification failed.\n");
    }

    void verifyDetails(string emailInput, string phoneInput) {
        cout << ((emailInput == email && phoneInput == phone)
                 ? "Email and phone verified successfully.\n"
                 : "Verification failed.\n");
    }

    void buyStock(const Stock &stock, int quantity, string date) {
        if (quantity <= 0) { cout << "Invalid quantity.\n"; return; }

        double totalCost = stock.getPrice() * quantity;
        if (totalCost > virtualMoney) {
            cout << "\nInsufficient virtual balance!\n";
            cout << "Required : Rs. " << totalCost << endl;
            cout << "Available: Rs. " << virtualMoney << endl;
            return;
        }

        virtualMoney -= totalCost;
        Holding newHolding(stock, quantity);
        portfolio = portfolio + newHolding;              // operator overloading
        portfolio.recordTransaction("BUY", stock.getSymbol(), quantity, stock.getPrice(), date);

        cout << "\nStock purchased successfully!\n";
        cout << "Stock : " << stock.getSymbol() << endl;
        cout << "Quantity : " << quantity << endl;
        cout << "Amount : Rs. " << totalCost << endl;
        cout << "Remaining Balance: Rs. " << virtualMoney << endl;
    }

    void sellStock(const Stock &stock, int quantity, string date) {
        if (quantity <= 0) { cout << "Invalid quantity.\n"; return; }

        int ownedQuantity = portfolio.getQuantity(stock.getSymbol());
        if (ownedQuantity == 0) {
            cout << "\nYou do not own " << stock.getSymbol() << ".\n";
            return;
        }
        if (quantity > ownedQuantity) {
            cout << "\nYou cannot sell " << quantity << " shares.\n";
            cout << "You only own " << ownedQuantity << " shares.\n";
            return;
        }

        double saleAmount = stock.getPrice() * quantity;
        bool sold = portfolio.sellHolding(stock.getSymbol(), quantity);

        if (sold) {
            virtualMoney += saleAmount;
            portfolio.recordTransaction("SELL", stock.getSymbol(), quantity, stock.getPrice(), date);
            cout << "\nStock sold successfully!\n";
            cout << "Stock : " << stock.getSymbol() << endl;
            cout << "Quantity : " << quantity << endl;
            cout << "Received : Rs. " << saleAmount << endl;
            cout << "New Balance: Rs. " << virtualMoney << endl;
        }
    }

    void displayBalance() const {
        cout << "\nVirtual Balance: Rs. "
             << fixed << setprecision(2) << virtualMoney << endl;
    }

    void displayAccountSummary() const {
        double portfolioValue = portfolio.getPortfolioValue();
        double totalAccountValue = virtualMoney + portfolioValue;
        showDetails();
        cout << "\n ACCOUNT SUMMARY\n";
        cout << "Available Balance : Rs. " << fixed << setprecision(2) << virtualMoney << endl;
        cout << "Portfolio Value : Rs. " << portfolioValue << endl;
        cout << "Total Account Value: Rs. " << totalAccountValue << endl;
    }

    void displayPortfolio() const { portfolio.displayPortfolio(); }
    void displayHistory() const { portfolio.displayHistory(); }

    void updatePortfolioPrice(string symbol, double newPrice) {
        portfolio.updateStockPrice(symbol, newPrice);
    }

    // Overriding the base class's virtual function (runtime polymorphism)
    void showDetails() const override {
        cout << "\nTRADER DETAILS\n";
        cout << "Name : " << name << endl;
        cout << "Email : " << email << endl;
        cout << "Phone : " << phone << endl;
        cout << "User ID : " << userID << endl;
    }

    static void showTraderCount() {
        cout << "Total Traders Registered: " << traderCount << endl;
    }

    // Operator Overloading + Friend Function: lets us do `cout << trader;`
    friend ostream& operator<<(ostream &out, const Trader &trader) {
        out << "Trader Name: " << trader.name << endl;
        out << "Trader ID: " << trader.userID << endl;
        out << "Available Money: Rs. " << fixed << setprecision(2) << trader.virtualMoney << endl;
        return out;
    }
};
int Trader::traderCount = 0;

// HELPER FUNCTION
bool isValidPhone(string phone) {
    if (phone.length() != 10) return false;
    for (char c : phone) {
        if (c < '0' || c > '9') return false;
    }
    return true;
}

// MAIN
int main() {
    cout << "\n";
    cout << " VIRTUAL STOCK MARKET SIMULATOR\n";
    cout << "\n";

    string name, email, phone;
    double initialMoney;

    cout << "\nEnter your name: ";
    getline(cin, name);
    cout << "Enter your email: ";
    getline(cin, email);

    while (true) {
        cout << "Enter your 10-digit phone number: ";
        cin >> phone;
        if (isValidPhone(phone)) break;
        cout << "Invalid phone number. Please try again.\n";
    }

    while (true) {
        cout << "Enter initial virtual money: ";
        if (cin >> initialMoney) {
            if (initialMoney > 0) break;
            cout << "Amount must be greater than 0.\n";
        } else {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    // Demonstrates inheritance: Trader is derived from User.
    Trader trader(name, email, phone, initialMoney);
    cout << "\nAccount created successfully!\n";
    cout << "Your User ID is: " << trader.getUserID() << endl;
    Trader::showTraderCount();

    // Market stock objects (will become a proper Market class in the next step)
    Stock tcs("TCS", "Tata Consultancy Services", 3500);
    Stock infy("INFY", "Infosys", 1800);
    Stock reliance("RELIANCE", "Reliance Industries", 2900);
    Stock hdfc("HDFC", "HDFC Bank", 1650);
    Stock wipro("WIPRO", "Wipro Limited", 550);

    // A fixed "today" for simulated transaction dates; swap for real dates later.
    string today = "17-09-2026";

    while (true) {
        cout << "\n\n";
        cout << " MAIN MENU\n";
        cout << "\n";
        cout << "1. View Market\n";
        cout << "2. Buy Stock\n";
        cout << "3. Sell Stock\n";
        cout << "4. View Portfolio\n";
        cout << "5. View Balance\n";
        cout << "6. Update Stock Price\n";
        cout << "7. Account Summary\n";
        cout << "8. Verify Details\n";
        cout << "9. View Transaction History\n";
        cout << "10. Exit\n";
        cout << "\n";
        cout << "Enter your choice: ";

        int choice;
        if (!(cin >> choice)) {
            cout << "\nInvalid input! Please enter a number from 1 to 10.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
            case 1:
                cout << "\nMARKET \n";
                cout << left << setw(10) << "Symbol" << setw(25) << "Company" << "Price" << endl;
                cout << "\n";
                tcs.displayStock();
                infy.displayStock();
                reliance.displayStock();
                hdfc.displayStock();
                wipro.displayStock();
                break;

            case 2: {
                int stockChoice, quantity;
                cout << "\nSelect Stock:\n1. TCS\n2. INFY\n3. RELIANCE\n4. HDFC\n5. WIPRO\n";
                cout << "Enter stock number: ";
                if (!(cin >> stockChoice)) { cout << "Invalid input.\n"; cin.clear(); cin.ignore(10000, '\n'); break; }
                cout << "Enter quantity: ";
                if (!(cin >> quantity)) { cout << "Invalid quantity.\n"; cin.clear(); cin.ignore(10000, '\n'); break; }

                switch (stockChoice) {
                    case 1: trader.buyStock(tcs, quantity, today); break;
                    case 2: trader.buyStock(infy, quantity, today); break;
                    case 3: trader.buyStock(reliance, quantity, today); break;
                    case 4: trader.buyStock(hdfc, quantity, today); break;
                    case 5: trader.buyStock(wipro, quantity, today); break;
                    default: cout << "Invalid stock selection.\n";
                }
                break;
            }

            case 3: {
                int stockChoice, quantity;
                cout << "\nSelect Stock to Sell:\n1. TCS\n2. INFY\n3. RELIANCE\n4. HDFC\n5. WIPRO\n";
                cout << "Enter stock number: ";
                if (!(cin >> stockChoice)) { cout << "Invalid input.\n"; cin.clear(); cin.ignore(10000, '\n'); break; }
                cout << "Enter quantity: ";
                if (!(cin >> quantity)) { cout << "Invalid quantity.\n"; cin.clear(); cin.ignore(10000, '\n'); break; }

                switch (stockChoice) {
                    case 1: trader.sellStock(tcs, quantity, today); break;
                    case 2: trader.sellStock(infy, quantity, today); break;
                    case 3: trader.sellStock(reliance, quantity, today); break;
                    case 4: trader.sellStock(hdfc, quantity, today); break;
                    case 5: trader.sellStock(wipro, quantity, today); break;
                    default: cout << "Invalid stock selection.\n";
                }
                break;
            }

            case 4:
                trader.displayPortfolio();
                break;

            case 5:
                trader.displayBalance();
                break;

            case 6: {
                int stockChoice;
                double newPrice;
                cout << "\nSelect stock:\n1. TCS\n2. INFY\n3. RELIANCE\n4. HDFC\n5. WIPRO\n";
                cout << "Enter stock number: ";
                if (!(cin >> stockChoice)) { cout << "Invalid input.\n"; cin.clear(); cin.ignore(10000, '\n'); break; }
                cout << "Enter new simulated price: ";
                if (!(cin >> newPrice)) { cout << "Invalid price.\n"; cin.clear(); cin.ignore(10000, '\n'); break; }
                if (newPrice <= 0) { cout << "Price must be greater than 0.\n"; break; }

                switch (stockChoice) {
                    case 1: tcs.updatePrice(newPrice); trader.updatePortfolioPrice("TCS", newPrice); break;
                    case 2: infy.updatePrice(newPrice); trader.updatePortfolioPrice("INFY", newPrice); break;
                    case 3: reliance.updatePrice(newPrice); trader.updatePortfolioPrice("RELIANCE", newPrice); break;
                    case 4: hdfc.updatePrice(newPrice); trader.updatePortfolioPrice("HDFC", newPrice); break;
                    case 5: wipro.updatePrice(newPrice); trader.updatePortfolioPrice("WIPRO", newPrice); break;
                    default: cout << "Invalid stock selection.\n";
                }
                break;
            }

            case 7:
                trader.displayAccountSummary();
                break;

            case 8: {
                int mode;
                cout << "\n1. Verify email only\n2. Verify email + phone\n";
                cout << "Choose: ";
                if (!(cin >> mode)) { cout << "Invalid input.\n"; cin.clear(); cin.ignore(10000, '\n'); break; }
                cin.ignore();
                if (mode == 1) {
                    string emailInput;
                    cout << "Enter email: ";
                    getline(cin, emailInput);
                    trader.verifyDetails(emailInput);
                } else if (mode == 2) {
                    string emailInput, phoneInput;
                    cout << "Enter email: ";
                    getline(cin, emailInput);
                    cout << "Enter phone: ";
                    getline(cin, phoneInput);
                    trader.verifyDetails(emailInput, phoneInput);
                } else {
                    cout << "Invalid option.\n";
                }
                break;
            }

            case 9:
                trader.displayHistory();
                break;

            case 10:
                cout << "\nThank you for using the Virtual Stock Market Simulator!\n";
                return 0;

            default:
                cout << "\nInvalid choice! Please select between 1 and 10.\n";
        }
    }

    return 0;
}
