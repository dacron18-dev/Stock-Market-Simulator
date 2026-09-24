#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
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

// MARKET CLASS
// Maintains every stock available to trade, looks stocks up by
// symbol, updates prices, and tracks whether trading is open.
class Market {
private:
    vector<Stock> stocks;
    bool marketOpen;

public:
    Market() : marketOpen(true) {}

    void addStock(const Stock &s) {
        stocks.push_back(s);
    }

    // Returns a pointer so the caller can update the actual stock
    // stored in the market, not a copy of it. nullptr if not found.
    Stock* findStock(const string &symbol) {
        for (auto &s : stocks) {
            if (s.getSymbol() == symbol) return &s;
        }
        return nullptr;
    }

    const vector<Stock>& getAllStocks() const {
        return stocks;
    }

    bool updateStockPrice(const string &symbol, double newPrice) {
        Stock* s = findStock(symbol);
        if (!s) return false;
        s->updatePrice(newPrice);
        return true;
    }

    void openMarket() {
        marketOpen = true;
        cout << "\nMarket is now OPEN for trading.\n";
    }

    void closeMarket() {
        marketOpen = false;
        cout << "\nMarket is now CLOSED. Trading is paused.\n";
    }

    bool isOpen() const {
        return marketOpen;
    }

    void displayMarket() const {
        cout << "\nMARKET (" << (marketOpen ? "OPEN" : "CLOSED") << ")\n";
        cout << left << setw(5) << "#" << setw(10) << "Symbol"
             << setw(25) << "Company" << "Price" << endl;
        cout << "\n";
        int i = 1;
        for (const auto &s : stocks) {
            cout << left << setw(5) << i++;
            s.displayStock();
        }
    }
};

// MARKET EVENT CLASS
// Represents something that happens to the market as a whole (a
// rally, a crash, a volatile session...) and knows how to apply
// its effect to every stock's price.
enum class MarketEventType { BULL_RUN, BEAR_CRASH, VOLATILITY_SPIKE, STABLE_DRIFT };

class MarketEvent {
private:
    MarketEventType type;
    double impactFactor;   // multiplier applied to each stock's price

public:
    MarketEvent(MarketEventType t, double impact) : type(t), impactFactor(impact) {}

    string getTypeString() const {
        switch (type) {
            case MarketEventType::BULL_RUN:         return "BULL RUN";
            case MarketEventType::BEAR_CRASH:       return "BEAR CRASH";
            case MarketEventType::VOLATILITY_SPIKE: return "VOLATILITY SPIKE";
            case MarketEventType::STABLE_DRIFT:     return "STABLE DRIFT";
        }
        return "UNKNOWN";
    }

    double getImpactFactor() const { return impactFactor; }

    // Applies the impact factor to every stock in the market, with a
    // small amount of per-stock random jitter so prices don't all move
    // by exactly the same percentage.
    void applyToMarket(Market &market) const {
        for (const auto &s : market.getAllStocks()) {
            double jitter = 1.0 + ((rand() % 21 - 10) / 1000.0);   // +/- 1%
            double newPrice = s.getPrice() * impactFactor * jitter;
            if (newPrice < 1.0) newPrice = 1.0;   // guard against a non-positive price
            market.updateStockPrice(s.getSymbol(), newPrice);
        }
    }

    void displayEvent() const {
        cout << "\nMARKET EVENT: " << getTypeString()
             << " (impact x" << fixed << setprecision(3) << impactFactor << ")\n";
    }
};

// ORDER CLASS
// Wraps a single buy or sell request: what's being traded, at what
// price/quantity, whether it's valid, and its current status.
enum class OrderType { BUY, SELL };
enum class OrderStatus { PENDING, EXECUTED, CANCELLED, REJECTED };

class Order {
private:
    static int nextOrderID;
    int orderID;
    OrderType type;
    Stock stock;
    int quantity;
    double orderPrice;      // price captured at the moment the order was placed
    OrderStatus status;

public:
    Order(OrderType t, Stock s, int qty)
        : type(t), stock(s), quantity(qty), orderPrice(s.getPrice()), status(OrderStatus::PENDING) {
        orderID = nextOrderID++;
    }

    int getOrderID() const { return orderID; }
    string getSymbol() const { return stock.getSymbol(); }
    int getQuantity() const { return quantity; }
    OrderStatus getStatus() const { return status; }

    string getTypeString() const { return type == OrderType::BUY ? "BUY" : "SELL"; }

    string getStatusString() const {
        switch (status) {
            case OrderStatus::PENDING:   return "PENDING";
            case OrderStatus::EXECUTED:  return "EXECUTED";
            case OrderStatus::CANCELLED: return "CANCELLED";
            case OrderStatus::REJECTED:  return "REJECTED";
        }
        return "UNKNOWN";
    }

    // Checks the order against the trader's available cash (for BUY)
    // or owned quantity (for SELL). Marks the order REJECTED if invalid.
    bool validate(double availableCash, int ownedQuantity) {
        if (quantity <= 0) { status = OrderStatus::REJECTED; return false; }

        if (type == OrderType::BUY) {
            if (orderPrice * quantity > availableCash) {
                status = OrderStatus::REJECTED;
                return false;
            }
        } else {
            if (quantity > ownedQuantity) {
                status = OrderStatus::REJECTED;
                return false;
            }
        }
        return true;
    }

    // Called once the caller (Trader) has actually applied the cash/
    // holding change. Only a PENDING order can be executed.
    bool execute() {
        if (status != OrderStatus::PENDING) return false;
        status = OrderStatus::EXECUTED;
        return true;
    }

    // Only a PENDING order can be cancelled; an already-executed or
    // rejected order cannot be undone this way.
    bool cancel() {
        if (status != OrderStatus::PENDING) return false;
        status = OrderStatus::CANCELLED;
        return true;
    }

    void displayOrder() const {
        cout << left << setw(6) << orderID
             << setw(6) << getTypeString()
             << setw(10) << stock.getSymbol()
             << setw(10) << quantity
             << "Rs. " << fixed << setprecision(2) << setw(12) << orderPrice
             << getStatusString() << endl;
    }
};
int Order::nextOrderID = 1;

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
    vector<Order> orderHistory;
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
        Order order(OrderType::BUY, stock, quantity);

        bool valid = order.validate(virtualMoney, portfolio.getQuantity(stock.getSymbol()));
        if (!valid) {
            if (quantity <= 0) {
                cout << "Invalid quantity.\n";
            } else {
                double totalCost = stock.getPrice() * quantity;
                cout << "\nInsufficient virtual balance!\n";
                cout << "Required : Rs. " << totalCost << endl;
                cout << "Available: Rs. " << virtualMoney << endl;
            }
            orderHistory.push_back(order);
            return;
        }

        double totalCost = stock.getPrice() * quantity;
        virtualMoney -= totalCost;
        Holding newHolding(stock, quantity);
        portfolio = portfolio + newHolding;              // operator overloading
        portfolio.recordTransaction("BUY", stock.getSymbol(), quantity, stock.getPrice(), date);
        order.execute();
        orderHistory.push_back(order);

        cout << "\nStock purchased successfully!\n";
        cout << "Stock : " << stock.getSymbol() << endl;
        cout << "Quantity : " << quantity << endl;
        cout << "Amount : Rs. " << totalCost << endl;
        cout << "Remaining Balance: Rs. " << virtualMoney << endl;
    }

    void sellStock(const Stock &stock, int quantity, string date) {
        int ownedQuantity = portfolio.getQuantity(stock.getSymbol());
        Order order(OrderType::SELL, stock, quantity);

        bool valid = order.validate(virtualMoney, ownedQuantity);
        if (!valid) {
            if (quantity <= 0) {
                cout << "Invalid quantity.\n";
            } else if (ownedQuantity == 0) {
                cout << "\nYou do not own " << stock.getSymbol() << ".\n";
            } else {
                cout << "\nYou cannot sell " << quantity << " shares.\n";
                cout << "You only own " << ownedQuantity << " shares.\n";
            }
            orderHistory.push_back(order);
            return;
        }

        double saleAmount = stock.getPrice() * quantity;
        bool sold = portfolio.sellHolding(stock.getSymbol(), quantity);

        if (sold) {
            virtualMoney += saleAmount;
            portfolio.recordTransaction("SELL", stock.getSymbol(), quantity, stock.getPrice(), date);
            order.execute();
            cout << "\nStock sold successfully!\n";
            cout << "Stock : " << stock.getSymbol() << endl;
            cout << "Quantity : " << quantity << endl;
            cout << "Received : Rs. " << saleAmount << endl;
            cout << "New Balance: Rs. " << virtualMoney << endl;
        } else {
            // Defensive: shouldn't happen since validate() already checked
            // ownedQuantity, but leaves the order correctly accounted for.
            order.cancel();
        }
        orderHistory.push_back(order);
    }

    void displayOrderHistory() const {
        if (orderHistory.empty()) {
            cout << "\nNo orders placed yet.\n";
            return;
        }
        cout << "\nORDER HISTORY\n";
        cout << left << setw(6) << "ID" << setw(6) << "Type"
             << setw(10) << "Symbol" << setw(10) << "Qty"
             << setw(16) << "Price" << "Status" << endl;
        for (const auto &o : orderHistory) o.displayOrder();
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

// SIMULATION CLASS
// Owns the simulation clock: starts/stops the simulation, and on
// each tick generates a random MarketEvent and applies it to the
// connected Market.
class Simulation {
private:
    Market &market;   // the market this simulation is connected to
    bool running;
    int tickCount;

    MarketEvent generateMarketEvent() {
        int roll = rand() % 4;
        switch (roll) {
            case 0: return MarketEvent(MarketEventType::BULL_RUN, 1.05);
            case 1: return MarketEvent(MarketEventType::BEAR_CRASH, 0.95);
            case 2: return MarketEvent(MarketEventType::VOLATILITY_SPIKE,
                                        1.0 + ((rand() % 21 - 10) / 100.0));
            default: return MarketEvent(MarketEventType::STABLE_DRIFT,
                                         1.0 + ((rand() % 5 - 2) / 1000.0));
        }
    }

public:
    Simulation(Market &m) : market(m), running(false), tickCount(0) {}

    void start() {
        running = true;
        cout << "\nSimulation started.\n";
    }

    void stop() {
        running = false;
        cout << "\nSimulation stopped.\n";
    }

    bool isRunning() const { return running; }
    int getTickCount() const { return tickCount; }

    // Generates one random market event, applies it to the market,
    // and advances the simulation clock by one tick.
    void advanceTick() {
        if (!running) {
            cout << "\nSimulation is not running. Start it first.\n";
            return;
        }

        MarketEvent event = generateMarketEvent();
        event.applyToMarket(market);
        tickCount++;

        cout << "\n--- Tick " << tickCount << " ---";
        event.displayEvent();
        market.displayMarket();
    }
};

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
    srand(static_cast<unsigned int>(time(nullptr)));

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

    // Market now owns every stock; nothing else holds a loose Stock variable.
    Market market;
    market.addStock(Stock("TCS", "Tata Consultancy Services", 3500));
    market.addStock(Stock("INFY", "Infosys", 1800));
    market.addStock(Stock("RELIANCE", "Reliance Industries", 2900));
    market.addStock(Stock("HDFC", "HDFC Bank", 1650));
    market.addStock(Stock("WIPRO", "Wipro Limited", 550));

    // A fixed "today" for simulated transaction dates; swap for real dates later.
    string today = "17-09-2026";

    // Simulation is connected to this one market and drives its price ticks.
    Simulation simulation(market);

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
        cout << "10. View Order History\n";
        cout << "11. Open Market (Admin)\n";
        cout << "12. Close Market (Admin)\n";
        cout << "13. Start Simulation (Admin)\n";
        cout << "14. Stop Simulation (Admin)\n";
        cout << "15. Advance Simulation Tick (Admin)\n";
        cout << "16. Exit\n";
        cout << "\n";
        cout << "Enter your choice: ";

        int choice;
        if (!(cin >> choice)) {
            cout << "\nInvalid input! Please enter a number from 1 to 16.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
            case 1:
                market.displayMarket();
                break;

            case 2: {
                if (!market.isOpen()) { cout << "\nMarket is closed. Trading is paused.\n"; break; }

                const vector<Stock> &stocks = market.getAllStocks();
                cout << "\nSelect Stock:\n";
                for (size_t i = 0; i < stocks.size(); i++) {
                    cout << (i + 1) << ". " << stocks[i].getSymbol() << "\n";
                }
                int stockChoice, quantity;
                cout << "Enter stock number: ";
                if (!(cin >> stockChoice)) { cout << "Invalid input.\n"; cin.clear(); cin.ignore(10000, '\n'); break; }
                if (stockChoice < 1 || stockChoice > (int)stocks.size()) { cout << "Invalid stock selection.\n"; break; }
                cout << "Enter quantity: ";
                if (!(cin >> quantity)) { cout << "Invalid quantity.\n"; cin.clear(); cin.ignore(10000, '\n'); break; }

                trader.buyStock(stocks[stockChoice - 1], quantity, today);
                break;
            }

            case 3: {
                if (!market.isOpen()) { cout << "\nMarket is closed. Trading is paused.\n"; break; }

                const vector<Stock> &stocks = market.getAllStocks();
                cout << "\nSelect Stock to Sell:\n";
                for (size_t i = 0; i < stocks.size(); i++) {
                    cout << (i + 1) << ". " << stocks[i].getSymbol() << "\n";
                }
                int stockChoice, quantity;
                cout << "Enter stock number: ";
                if (!(cin >> stockChoice)) { cout << "Invalid input.\n"; cin.clear(); cin.ignore(10000, '\n'); break; }
                if (stockChoice < 1 || stockChoice > (int)stocks.size()) { cout << "Invalid stock selection.\n"; break; }
                cout << "Enter quantity: ";
                if (!(cin >> quantity)) { cout << "Invalid quantity.\n"; cin.clear(); cin.ignore(10000, '\n'); break; }

                trader.sellStock(stocks[stockChoice - 1], quantity, today);
                break;
            }

            case 4:
                trader.displayPortfolio();
                break;

            case 5:
                trader.displayBalance();
                break;

            case 6: {
                const vector<Stock> &stocks = market.getAllStocks();
                cout << "\nSelect stock:\n";
                for (size_t i = 0; i < stocks.size(); i++) {
                    cout << (i + 1) << ". " << stocks[i].getSymbol() << "\n";
                }
                int stockChoice;
                double newPrice;
                cout << "Enter stock number: ";
                if (!(cin >> stockChoice)) { cout << "Invalid input.\n"; cin.clear(); cin.ignore(10000, '\n'); break; }
                if (stockChoice < 1 || stockChoice > (int)stocks.size()) { cout << "Invalid stock selection.\n"; break; }
                cout << "Enter new simulated price: ";
                if (!(cin >> newPrice)) { cout << "Invalid price.\n"; cin.clear(); cin.ignore(10000, '\n'); break; }
                if (newPrice <= 0) { cout << "Price must be greater than 0.\n"; break; }

                string symbol = stocks[stockChoice - 1].getSymbol();
                market.updateStockPrice(symbol, newPrice);
                trader.updatePortfolioPrice(symbol, newPrice);
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
                trader.displayOrderHistory();
                break;

            case 11:
                market.openMarket();
                break;

            case 12:
                market.closeMarket();
                break;

            case 13:
                simulation.start();
                break;

            case 14:
                simulation.stop();
                break;

            case 15: {
                simulation.advanceTick();
                // Keep the trader's portfolio in sync with the new prices.
                for (const auto &s : market.getAllStocks()) {
                    trader.updatePortfolioPrice(s.getSymbol(), s.getPrice());
                }
                break;
            }

            case 16:
                cout << "\nThank you for using the Virtual Stock Market Simulator!\n";
                return 0;

            default:
                cout << "\nInvalid choice! Please select between 1 and 16.\n";
        }
    }

    return 0;
}
