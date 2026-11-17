#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

<<<<<<< HEAD
// CLASS AND OBJECTS

// Stock is a class that represents a stock in the market.
// Objects like tcs, infy, hdfc etc. are created from this class.

class Stock
{
=======

// 
// CLASS AND OBJECTS
// 
// Stock is a class that represents a stock in the market.
// Objects like tcs, infy, hdfc etc. are created from this class.

class Stock {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
private:
    // Encapsulation + Data Hiding
    // These variables cannot be accessed directly from outside.
    string symbol;
    string companyName;
    double currentPrice;

public:
<<<<<<< HEAD
    // Constructor
    Stock(string s, string c, double p)
    {
=======

    // Constructor
    Stock(string s, string c, double p) {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
        symbol = s;
        companyName = c;
        currentPrice = p;
    }

    // Getters are used to access private data safely.
<<<<<<< HEAD
    string getSymbol() const
    {
        return symbol;
    }

    string getCompanyName() const
    {
        return companyName;
    }

    double getPrice() const
    {
        return currentPrice;
    }

    void updatePrice(double newPrice)
    {
        if (newPrice > 0)
        {
=======
    string getSymbol() const {
        return symbol;
    }

    string getCompanyName() const {
        return companyName;
    }

    double getPrice() const {
        return currentPrice;
    }

    void updatePrice(double newPrice) {
        if (newPrice > 0) {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
            currentPrice = newPrice;
        }
    }

<<<<<<< HEAD
    void displayStock() const
    {
=======
    void displayStock() const {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
        cout << left << setw(10) << symbol
             << setw(25) << companyName
             << "Rs. " << fixed << setprecision(2)
             << currentPrice << endl;
    }
};

<<<<<<< HEAD
// HOLDING CLASS

// A Holding represents how many shares of a particular stock
// an investor currently owns.

class Holding
{
=======

// 
// HOLDING CLASS
// 
// A Holding represents how many shares of a particular stock
// an investor currently owns.

class Holding {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
private:
    Stock stock;
    int quantity;

public:
<<<<<<< HEAD
=======

>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
    // Constructor
    // Initializer list is used because Stock has no default constructor.
    Holding(Stock s, int q) : stock(s), quantity(q) {}

<<<<<<< HEAD
    Stock getStock() const
    {
        return stock;
    }

    string getSymbol() const
    {
        return stock.getSymbol();
    }

    int getQuantity() const
    {
        return quantity;
    }

    double getValue() const
    {
        return stock.getPrice() * quantity;
    }

    void addQuantity(int q)
    {
        if (q > 0)
        {
=======
    Stock getStock() const {
        return stock;
    }

    string getSymbol() const {
        return stock.getSymbol();
    }

    int getQuantity() const {
        return quantity;
    }

    double getValue() const {
        return stock.getPrice() * quantity;
    }

    void addQuantity(int q) {
        if (q > 0) {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
            quantity += q;
        }
    }

<<<<<<< HEAD
    bool reduceQuantity(int q)
    {
        if (q <= 0 || q > quantity)
        {
=======
    bool reduceQuantity(int q) {
        if (q <= 0 || q > quantity) {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
            return false;
        }

        quantity -= q;
        return true;
    }

<<<<<<< HEAD
    void updatePrice(double newPrice)
    {
        stock.updatePrice(newPrice);
    }

    void displayHolding() const
    {
=======
    void updatePrice(double newPrice) {
        stock.updatePrice(newPrice);
    }

    void displayHolding() const {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
        cout << left << setw(10) << stock.getSymbol()
             << setw(25) << stock.getCompanyName()
             << setw(12) << quantity
             << "Rs. " << fixed << setprecision(2)
             << getValue() << endl;
    }
};

<<<<<<< HEAD
// PORTFOLIO CLASS

// Portfolio keeps track of all the holdings owned by an investor.

class Portfolio
{
=======

// 
// PORTFOLIO CLASS
// 
// Portfolio keeps track of all the holdings owned by an investor.

class Portfolio {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
private:
    // Encapsulation:
    // holdings is private and can only be changed through methods.
    vector<Holding> holdings;

public:
<<<<<<< HEAD
    // Add a stock to the portfolio
    void addHolding(Stock stock, int quantity)
    {

        // If the investor already owns this stock,
        // we increase the quantity instead of creating another entry.
        for (auto &h : holdings)
        {

            if (h.getSymbol() == stock.getSymbol())
            {
=======

    // Add a stock to the portfolio
    void addHolding(Stock stock, int quantity) {

        // If the investor already owns this stock,
        // we increase the quantity instead of creating another entry.
        for (auto &h : holdings) {

            if (h.getSymbol() == stock.getSymbol()) {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
                h.addQuantity(quantity);
                h.updatePrice(stock.getPrice());
                return;
            }
        }

        holdings.push_back(Holding(stock, quantity));
    }

<<<<<<< HEAD
    // Removes shares when the investor sells a stock.
    bool sellHolding(string symbol, int quantity)
    {

        for (auto it = holdings.begin(); it != holdings.end(); ++it)
        {

            if (it->getSymbol() == symbol)
            {

                if (quantity > it->getQuantity())
                {
=======

    // Removes shares when the investor sells a stock.
    bool sellHolding(string symbol, int quantity) {

        for (auto it = holdings.begin(); it != holdings.end(); ++it) {

            if (it->getSymbol() == symbol) {

                if (quantity > it->getQuantity()) {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
                    return false;
                }

                it->reduceQuantity(quantity);

                // Remove the stock completely if no shares are left.
<<<<<<< HEAD
                if (it->getQuantity() == 0)
                {
=======
                if (it->getQuantity() == 0) {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
                    holdings.erase(it);
                }

                return true;
            }
        }

        return false;
    }

<<<<<<< HEAD
    // Calculates the current value of the whole portfolio.
    double getPortfolioValue() const
    {

        double total = 0;

        for (const auto &h : holdings)
        {
=======

    // Calculates the current value of the whole portfolio.
    double getPortfolioValue() const {

        double total = 0;

        for (const auto &h : holdings) {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
            total += h.getValue();
        }

        return total;
    }

<<<<<<< HEAD
    // Returns the number of shares owned for a particular stock.
    int getQuantity(string symbol) const
    {

        for (const auto &h : holdings)
        {

            if (h.getSymbol() == symbol)
            {
=======

    // Returns the number of shares owned for a particular stock.
    int getQuantity(string symbol) const {

        for (const auto &h : holdings) {

            if (h.getSymbol() == symbol) {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
                return h.getQuantity();
            }
        }

        return 0;
    }

<<<<<<< HEAD
    // Used when a stock price changes.
    void updateStockPrice(string symbol, double newPrice)
    {

        for (auto &h : holdings)
        {

            if (h.getSymbol() == symbol)
            {
=======

    // Used when a stock price changes.
    void updateStockPrice(string symbol, double newPrice) {

        for (auto &h : holdings) {

            if (h.getSymbol() == symbol) {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
                h.updatePrice(newPrice);
                return;
            }
        }
    }

<<<<<<< HEAD
    void displayPortfolio() const
    {

        if (holdings.empty())
        {
=======

    void displayPortfolio() const {

        if (holdings.empty()) {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
            cout << "\nYour portfolio is empty.\n";
            return;
        }

<<<<<<< HEAD
        cout << "\nYOUR PORTFOLIO \n";
=======
        cout << "\nYOUR PORTFOLIO\n";
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

        cout << left << setw(10) << "Symbol"
             << setw(25) << "Company"
             << setw(12) << "Quantity"
             << "Value" << endl;

        cout << "\n";

<<<<<<< HEAD
        for (const auto &h : holdings)
        {
=======
        for (const auto &h : holdings) {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
            h.displayHolding();
        }

        cout << "\n";

        cout << "Total Portfolio Value: Rs. "
             << fixed << setprecision(2)
             << getPortfolioValue() << endl;
    }

<<<<<<< HEAD
    // FRIEND FUNCTION + OPERATOR OVERLOADING

    // This allows us to use:
    //
    // portfolio = portfolio + newHolding;
=======

    // 
    // FRIEND FUNCTION + OPERATOR OVERLOADING
    // 
    // This allows us to use:
    //
    // portfolio  portfolio + newHolding;
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
    //
    // instead of calling addHolding() directly.

    friend Portfolio operator+(const Portfolio &p,
                               const Holding &h);
};

<<<<<<< HEAD
// OPERATOR OVERLOADING

// The + operator normally works with numbers.
// Here, we give it a new meaning for Portfolio and Holding.

Portfolio operator+(const Portfolio &p, const Holding &h)
{
=======

// 
// OPERATOR OVERLOADING
// 
// The + operator normally works with numbers.
// Here, we give it a new meaning for Portfolio and Holding.

Portfolio operator+(const Portfolio &p, const Holding &h) {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

    Portfolio result = p;

    result.addHolding(
        h.getStock(),
<<<<<<< HEAD
        h.getQuantity());
=======
        h.getQuantity()
    );
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

    return result;
}

<<<<<<< HEAD
// USER CLASS

// This is the base class for users of our application.

class User
{
=======

// 
// USER CLASS
// 
// This is the base class for users of our application.

class User {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

protected:
    // Protected members can be accessed by this class
    // and its derived classes.
    string name;
    string email;
    string phone;
    int userID;

    // Static member is shared by all User objects.
    static int nextUserID;

public:
<<<<<<< HEAD
    // Constructor
    User(string n, string e, string p)
    {
=======

    // Constructor
    User(string n, string e, string p) {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

        name = n;
        email = e;
        phone = p;

        // ID is generated automatically.
        // User does not choose their own ID.
        userID = nextUserID++;
    }

<<<<<<< HEAD
    void displayUserDetails() const
    {

        cout << "\n USER DETAILS\n";
=======

    void displayUserDetails() const {

        cout << "\n========== USER DETAILS ==========\n";
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
        cout << "Name    : " << name << endl;
        cout << "Email   : " << email << endl;
        cout << "Phone   : " << phone << endl;
        cout << "User ID : " << userID << endl;
    }

<<<<<<< HEAD
    int getUserID() const
    {
=======

    int getUserID() const {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
        return userID;
    }
};

// First user gets 1001, then 1002, 1003 and so on.
int User::nextUserID = 1001;

// INHERITANCE

// Investor inherits the properties and functions of User.
//
// User
//   |
//   ↓
// Investor

class Investor : public User
{

<<<<<<< HEAD
=======
// First user gets 1001, then 1002, 1003 and so on.
int User::nextUserID = 1001;


// 
// INHERITANCE
// 
// Investor inherits the properties and functions of User.
//
// User
//   |
//   ↓
// Investor

class Investor : public User {

>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
private:
    // Encapsulation / Data Hiding
    double virtualMoney;
    Portfolio portfolio;

public:

    // Constructor
    // User's constructor is called using the initializer list.
    Investor(string n, string e, string p, double money)
<<<<<<< HEAD
        : User(n, e, p)
    {
=======
        : User(n, e, p) {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

        virtualMoney = money;
    }

<<<<<<< HEAD
    // FUNCTION OVERLOADING

    // Same function name but different parameters.
    // This is an example of compile-time polymorphism.

    void verifyDetails(string emailInput)
    {

        if (emailInput == email)
        {
            cout << "Email verified successfully.\n";
        }
        else
        {
            cout << "Email verification failed.\n";
        }
    }

    void verifyDetails(string emailInput, string phoneInput)
    {

        if (emailInput == email &&
            phoneInput == phone)
        {

            cout << "Email and phone verified successfully.\n";
        }
        else
        {
            cout << "Verification failed.\n";
        }
    }

    // BUY STOCK

    void buyStock(const Stock &stock, int quantity)
    {

        if (quantity <= 0)
        {
            cout << "Invalid quantity.\n";
            return;
=======

    // 
    // FUNCTION OVERLOADING
    // 
    // Same function name but different parameters.
    // This is an example of compile-time polymorphism.

    void verifyDetails(string emailInput) {

        if (emailInput == email) {
            cout << "Email verified successfully.\n";
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
        }
        else {
            cout << "Email verification failed.\n";
        }
    }


    void verifyDetails(string emailInput, string phoneInput) {

        if (emailInput == email &&
            phoneInput == phone) {

            cout << "Email and phone verified successfully.\n";
        }
        else {
            cout << "Verification failed.\n";
        }
    }


    // 
    // BUY STOCK
    // 

    void buyStock(const Stock &stock, int quantity) {

<<<<<<< HEAD
        double totalCost = stock.getPrice() * quantity;

        // Before buying, check whether the investor
        // has enough virtual money.
        if (totalCost > virtualMoney)
        {

            cout << "\nInsufficient virtual balance!\n";

            cout << "Required : Rs. "
                 << totalCost << endl;

            cout << "Available: Rs. "
                 << virtualMoney << endl;

            return;
        }

        // Money is deducted only after checking the balance.
        virtualMoney -= totalCost;

        Holding newHolding(stock, quantity);

        // Operator Overloading is used here.
        // portfolio + newHolding adds the stock to the portfolio.
        portfolio = portfolio + newHolding;

        cout << "\nStock purchased successfully!\n";

        cout << "Stock    : "
             << stock.getSymbol() << endl;

        cout << "Quantity : "
             << quantity << endl;

        cout << "Amount   : Rs. "
             << totalCost << endl;

        cout << "Remaining Balance: Rs. "
=======
        if (quantity <= 0) {
            cout << "Invalid quantity.\n";
            return;
        }

        double totalCost = stock.getPrice() * quantity;


        // Before buying, check whether the investor
        // has enough virtual money.
        if (totalCost > virtualMoney) {

            cout << "\nInsufficient virtual balance!\n";

            cout << "Required : Rs. "
                 << totalCost << endl;

            cout << "Available: Rs. "
                 << virtualMoney << endl;

            return;
        }


        // Money is deducted only after checking the balance.
        virtualMoney -= totalCost;


        Holding newHolding(stock, quantity);


        // Operator Overloading is used here.
        // portfolio + newHolding adds the stock to the portfolio.
        portfolio = portfolio + newHolding;


        cout << "\nStock purchased successfully!\n";

        cout << "Stock    : "
             << stock.getSymbol() << endl;

        cout << "Quantity : "
             << quantity << endl;

        cout << "Amount   : Rs. "
             << totalCost << endl;

        cout << "Remaining Balance: Rs. "
             << virtualMoney << endl;
    }


    // 
    // SELL STOCK
    // 

    void sellStock(const Stock &stock, int quantity) {

        if (quantity <= 0) {
            cout << "Invalid quantity.\n";
            return;
        }


        int ownedQuantity =
            portfolio.getQuantity(stock.getSymbol());


        // Investor cannot sell a stock they don't own.
        if (ownedQuantity == 0) {

            cout << "\nYou do not own "
                 << stock.getSymbol()
                 << ".\n";

            return;
        }


        // Investor cannot sell more shares than they own.
        if (quantity > ownedQuantity) {

            cout << "\nYou cannot sell "
                 << quantity
                 << " shares.\n";

            cout << "You only own "
                 << ownedQuantity
                 << " shares.\n";

            return;
        }


        double saleAmount =
            stock.getPrice() * quantity;


        bool sold =
            portfolio.sellHolding(
                stock.getSymbol(),
                quantity
            );


        if (sold) {

            // Money received from selling is added to balance.
            virtualMoney += saleAmount;

            cout << "\nStock sold successfully!\n";

            cout << "Stock    : "
                 << stock.getSymbol() << endl;

            cout << "Quantity : "
                 << quantity << endl;

            cout << "Received : Rs. "
                 << saleAmount << endl;

            cout << "New Balance: Rs. "
                 << virtualMoney << endl;
        }
    }


    void displayBalance() const {

        cout << "\nVirtual Balance: Rs. "
             << fixed << setprecision(2)
             << virtualMoney << endl;
    }


    // Shows the total value of the investor's account.
    void displayAccountSummary() const {

        double portfolioValue =
            portfolio.getPortfolioValue();

        double totalAccountValue =
            virtualMoney + portfolioValue;


        displayUserDetails();

        cout << "\n ACCOUNT SUMMARY\n";

        cout << "Available Balance : Rs. "
             << fixed << setprecision(2)
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
             << virtualMoney << endl;

        cout << "Portfolio Value   : Rs. "
             << portfolioValue << endl;

        cout << "Total Account Value: Rs. "
             << totalAccountValue << endl;
    }


    void displayPortfolio() const {
        portfolio.displayPortfolio();
    }


    void updatePortfolioPrice(string symbol,
                              double newPrice) {

        portfolio.updateStockPrice(
            symbol,
            newPrice
        );
    }

    // SELL STOCK

    void sellStock(const Stock &stock, int quantity)
    {

        if (quantity <= 0)
        {
            cout << "Invalid quantity.\n";
            return;
        }

        int ownedQuantity =
            portfolio.getQuantity(stock.getSymbol());

        // Investor cannot sell a stock they don't own.
        if (ownedQuantity == 0)
        {

            cout << "\nYou do not own "
                 << stock.getSymbol()
                 << ".\n";

            return;
        }

        // Investor cannot sell more shares than they own.
        if (quantity > ownedQuantity)
        {

            cout << "\nYou cannot sell "
                 << quantity
                 << " shares.\n";

            cout << "You only own "
                 << ownedQuantity
                 << " shares.\n";

            return;
        }

        double saleAmount =
            stock.getPrice() * quantity;

        bool sold =
            portfolio.sellHolding(
                stock.getSymbol(),
                quantity);

        if (sold)
        {

            // Money received from selling is added to balance.
            virtualMoney += saleAmount;

            cout << "\nStock sold successfully!\n";

            cout << "Stock    : "
                 << stock.getSymbol() << endl;

            cout << "Quantity : "
                 << quantity << endl;

            cout << "Received : Rs. "
                 << saleAmount << endl;

            cout << "New Balance: Rs. "
                 << virtualMoney << endl;
        }
    }

    void displayBalance() const
    {

        cout << "\nVirtual Balance: Rs. "
             << fixed << setprecision(2)
             << virtualMoney << endl;
    }

    // Shows the total value of the investor's account.
    void displayAccountSummary() const
    {

        double portfolioValue =
            portfolio.getPortfolioValue();

        double totalAccountValue =
            virtualMoney + portfolioValue;

        displayUserDetails();

        cout << "\n========== ACCOUNT SUMMARY ==========\n";

        cout << "Available Balance : Rs. "
             << fixed << setprecision(2)
             << virtualMoney << endl;

        cout << "Portfolio Value   : Rs. "
             << portfolioValue << endl;

        cout << "Total Account Value: Rs. "
             << totalAccountValue << endl;
    }

    void displayPortfolio() const
    {
        portfolio.displayPortfolio();
    }

    void updatePortfolioPrice(string symbol,
                              double newPrice)
    {

        portfolio.updateStockPrice(
            symbol,
            newPrice);
    }
};

// NORMAL FUNCTION

<<<<<<< HEAD
// Checks whether the phone number entered by the user is valid.

bool isValidPhone(string phone)
{

    if (phone.length() != 10)
    {
        return false;
    }

    for (char c : phone)
    {

        if (c < '0' || c > '9')
        {
=======
// 
// NORMAL FUNCTION
// 
// Checks whether the phone number entered by the user is valid.

bool isValidPhone(string phone) {

    if (phone.length() != 10) {
        return false;
    }

    for (char c : phone) {

        if (c < '0' || c > '9') {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
            return false;
        }
    }

    return true;
}

// MAIN FUNCTION

<<<<<<< HEAD
int main()
{
=======
// 
// MAIN FUNCTION
// 

int main() {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

    cout << "\n";
    cout << "       VIRTUAL STOCK MARKET SIMULATOR\n";
    cout << "\n";

<<<<<<< HEAD
=======

>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
    // Taking details from the user.
    string name;
    string email;
    string phone;
    double initialMoney;
<<<<<<< HEAD
=======

>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

    cout << "\nEnter your name: ";
    getline(cin, name);


    cout << "Enter your email: ";
    getline(cin, email);

    // Keep asking until a valid phone number is entered.
    while (true)
    {

<<<<<<< HEAD
        cout << "Enter your 10-digit phone number: ";
        cin >> phone;

        if (isValidPhone(phone))
        {
=======
    // Keep asking until a valid phone number is entered.
    while (true) {

        cout << "Enter your 10-digit phone number: ";
        cin >> phone;

        if (isValidPhone(phone)) {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
            break;
        }

        cout << "Invalid phone number. Please try again.\n";
    }

    // Input validation is used so incorrect input
    // doesn't cause the program to get stuck.
    while (true)
    {

<<<<<<< HEAD
        cout << "Enter initial virtual money: ";
=======
    // Input validation is used so incorrect input
    // doesn't cause the program to get stuck.
    while (true) {

        cout << "Enter initial virtual money: ";

        if (cin >> initialMoney) {

            if (initialMoney > 0) {
                break;
            }

            cout << "Amount must be greater than 0.\n";
        }
        else {

            cout << "Invalid input! Please enter a number.\n";

            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

        if (cin >> initialMoney)
        {

<<<<<<< HEAD
            if (initialMoney > 0)
            {
                break;
            }

            cout << "Amount must be greater than 0.\n";
        }
        else
        {

            cout << "Invalid input! Please enter a number.\n";

            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    // Object of Investor class.
    // This also demonstrates inheritance because Investor
    // is derived from User.
    Investor investor(
        name,
        email,
        phone,
        initialMoney);
=======
    // Object of Investor class.
    // This also demonstrates inheritance because Investor
    // is derived from User.
    Investor investor(
        name,
        email,
        phone,
        initialMoney
    );

>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

    cout << "\nAccount created successfully!\n";

    cout << "Your User ID is: "
         << investor.getUserID()
         << endl;

<<<<<<< HEAD
    // MARKET STOCK OBJECTS

=======

    // 
    // MARKET STOCK OBJECTS
    // 
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
    // These are objects of the Stock class.

    Stock tcs(
        "TCS",
        "Tata Consultancy Services",
<<<<<<< HEAD
        3500);
=======
        3500
    );
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

    Stock infy(
        "INFY",
        "Infosys",
<<<<<<< HEAD
        1800);
=======
        1800
    );
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

    Stock reliance(
        "RELIANCE",
        "Reliance Industries",
<<<<<<< HEAD
        2900);
=======
        2900
    );
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

    Stock hdfc(
        "HDFC",
        "HDFC Bank",
<<<<<<< HEAD
        1650);
=======
        1650
    );
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

    Stock wipro(
        "WIPRO",
        "Wipro Limited",
<<<<<<< HEAD
        550);

    // MAIN MENU

    // Loop keeps the application running until Exit is selected.

    while (true)
    {
=======
        550
    );


    // 
    // MAIN MENU
    // 
    // Loop keeps the application running until Exit is selected.

    while (true) {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

        cout << "\n\n";
        cout << "                 MAIN MENU\n";
        cout << "\n";

        cout << "1. View Market\n";
        cout << "2. Buy Stock\n";
        cout << "3. Sell Stock\n";
        cout << "4. View Portfolio\n";
        cout << "5. View Balance\n";
        cout << "6. Update Stock Price\n";
        cout << "7. Account Summary\n";
        cout << "8. Verify Details\n";
        cout << "9. Exit\n";

        cout << "\n";

        cout << "Enter your choice: ";

        int choice;

<<<<<<< HEAD
        // Input validation prevents an infinite loop
        // if the user enters something like "abc".
        if (!(cin >> choice))
        {
=======

        // Input validation prevents an infinite loop
        // if the user enters something like "abc".
        if (!(cin >> choice)) {
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

            cout << "\nInvalid input! Please enter a number from 1 to 9.\n";

            cin.clear();
            cin.ignore(10000, '\n');

            continue;
        }

<<<<<<< HEAD
        switch (choice)
        {
=======

        switch (choice) {

            case 1:

                cout << "\nMARKET \n";

                cout << left
                     << setw(10) << "Symbol"
                     << setw(25) << "Company"
                     << "Price" << endl;

                cout << "\n";

                tcs.displayStock();
                infy.displayStock();
                reliance.displayStock();
                hdfc.displayStock();
                wipro.displayStock();

                break;
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

        case 1:

<<<<<<< HEAD
            cout << "\nMARKET\n";
=======
            case 2:
            {
                int stockChoice;
                int quantity;

                cout << "\nSelect Stock:\n";
                cout << "1. TCS\n";
                cout << "2. INFY\n";
                cout << "3. RELIANCE\n";
                cout << "4. HDFC\n";
                cout << "5. WIPRO\n";

                cout << "Enter stock number: ";

                if (!(cin >> stockChoice)) {

                    cout << "Invalid input.\n";

                    cin.clear();
                    cin.ignore(10000, '\n');

                    break;
                }
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

            cout << left
                 << setw(10) << "Symbol"
                 << setw(25) << "Company"
                 << "Price" << endl;

<<<<<<< HEAD
            cout << "\n";

            tcs.displayStock();
            infy.displayStock();
            reliance.displayStock();
            hdfc.displayStock();
            wipro.displayStock();
=======
                cout << "Enter quantity: ";

                if (!(cin >> quantity)) {

                    cout << "Invalid quantity.\n";

                    cin.clear();
                    cin.ignore(10000, '\n');

                    break;
                }
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

            break;

<<<<<<< HEAD
        case 2:
        {
            int stockChoice;
            int quantity;

            cout << "\nSelect Stock:\n";
            cout << "1. TCS\n";
            cout << "2. INFY\n";
            cout << "3. RELIANCE\n";
            cout << "4. HDFC\n";
            cout << "5. WIPRO\n";

            cout << "Enter stock number: ";

            if (!(cin >> stockChoice))
            {

                cout << "Invalid input.\n";
=======
                switch (stockChoice) {

                    case 1:
                        investor.buyStock(tcs, quantity);
                        break;

                    case 2:
                        investor.buyStock(infy, quantity);
                        break;

                    case 3:
                        investor.buyStock(reliance, quantity);
                        break;

                    case 4:
                        investor.buyStock(hdfc, quantity);
                        break;

                    case 5:
                        investor.buyStock(wipro, quantity);
                        break;

                    default:
                        cout << "Invalid stock selection.\n";
                }

                break;
            }
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

                cin.clear();
                cin.ignore(10000, '\n');

<<<<<<< HEAD
                break;
            }

            cout << "Enter quantity: ";

            if (!(cin >> quantity))
            {
=======
            case 3:
            {
                int stockChoice;
                int quantity;

                cout << "\nSelect Stock to Sell:\n";
                cout << "1. TCS\n";
                cout << "2. INFY\n";
                cout << "3. RELIANCE\n";
                cout << "4. HDFC\n";
                cout << "5. WIPRO\n";

                cout << "Enter stock number: ";

                if (!(cin >> stockChoice)) {

                    cout << "Invalid input.\n";

                    cin.clear();
                    cin.ignore(10000, '\n');

                    break;
                }
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

                cout << "Invalid quantity.\n";

<<<<<<< HEAD
                cin.clear();
                cin.ignore(10000, '\n');

                break;
            }

            switch (stockChoice)
            {

            case 1:
                investor.buyStock(tcs, quantity);
                break;

            case 2:
                investor.buyStock(infy, quantity);
                break;

            case 3:
                investor.buyStock(reliance, quantity);
                break;

            case 4:
                investor.buyStock(hdfc, quantity);
                break;
=======
                cout << "Enter quantity: ";

                if (!(cin >> quantity)) {

                    cout << "Invalid quantity.\n";

                    cin.clear();
                    cin.ignore(10000, '\n');

                    break;
                }
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

            case 5:
                investor.buyStock(wipro, quantity);
                break;

<<<<<<< HEAD
            default:
                cout << "Invalid stock selection.\n";
=======
                switch (stockChoice) {

                    case 1:
                        investor.sellStock(tcs, quantity);
                        break;

                    case 2:
                        investor.sellStock(infy, quantity);
                        break;

                    case 3:
                        investor.sellStock(reliance, quantity);
                        break;

                    case 4:
                        investor.sellStock(hdfc, quantity);
                        break;

                    case 5:
                        investor.sellStock(wipro, quantity);
                        break;

                    default:
                        cout << "Invalid stock selection.\n";
                }

                break;
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
            }

            break;
        }

<<<<<<< HEAD
        case 3:
        {
            int stockChoice;
            int quantity;

            cout << "\nSelect Stock to Sell:\n";
            cout << "1. TCS\n";
            cout << "2. INFY\n";
            cout << "3. RELIANCE\n";
            cout << "4. HDFC\n";
            cout << "5. WIPRO\n";

            cout << "Enter stock number: ";

            if (!(cin >> stockChoice))
            {

                cout << "Invalid input.\n";
=======
            case 4:

                investor.displayPortfolio();

                break;
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

                cin.clear();
                cin.ignore(10000, '\n');

<<<<<<< HEAD
                break;
            }
=======
            case 5:

                investor.displayBalance();

                break;
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

            cout << "Enter quantity: ";

<<<<<<< HEAD
            if (!(cin >> quantity))
            {

                cout << "Invalid quantity.\n";

                cin.clear();
                cin.ignore(10000, '\n');

                break;
            }

            switch (stockChoice)
            {

            case 1:
                investor.sellStock(tcs, quantity);
                break;

            case 2:
                investor.sellStock(infy, quantity);
                break;

            case 3:
                investor.sellStock(reliance, quantity);
                break;
=======
            case 6:
            {
                int stockChoice;
                double newPrice;

                cout << "\nSelect stock:\n";
                cout << "1. TCS\n";
                cout << "2. INFY\n";
                cout << "3. RELIANCE\n";
                cout << "4. HDFC\n";
                cout << "5. WIPRO\n";

                cout << "Enter stock number: ";

                if (!(cin >> stockChoice)) {

                    cout << "Invalid input.\n";

                    cin.clear();
                    cin.ignore(10000, '\n');

                    break;
                }
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de

            case 4:
                investor.sellStock(hdfc, quantity);
                break;

<<<<<<< HEAD
            case 5:
                investor.sellStock(wipro, quantity);
                break;

            default:
                cout << "Invalid stock selection.\n";
            }

            break;
        }

        case 4:

            investor.displayPortfolio();

            break;

        case 5:

            investor.displayBalance();

            break;

        case 6:
        {
            int stockChoice;
            double newPrice;

            cout << "\nSelect stock:\n";
            cout << "1. TCS\n";
            cout << "2. INFY\n";
            cout << "3. RELIANCE\n";
            cout << "4. HDFC\n";
            cout << "5. WIPRO\n";

            cout << "Enter stock number: ";

            if (!(cin >> stockChoice))
            {

                cout << "Invalid input.\n";

                cin.clear();
                cin.ignore(10000, '\n');

                break;
            }

            cout << "Enter new simulated price: ";

            if (!(cin >> newPrice))
            {

                cout << "Invalid price.\n";

                cin.clear();
                cin.ignore(10000, '\n');

                break;
            }

            if (newPrice <= 0)
            {

                cout << "Price must be greater than 0.\n";
                break;
            }

            // Price change is included to simulate
            // changes in the stock market.
            switch (stockChoice)
            {

            case 1:
                tcs.updatePrice(newPrice);
                investor.updatePortfolioPrice("TCS", newPrice);
                break;

            case 2:
                infy.updatePrice(newPrice);
                investor.updatePortfolioPrice("INFY", newPrice);
                break;

            case 3:
                reliance.updatePrice(newPrice);
                investor.updatePortfolioPrice("RELIANCE", newPrice);
                break;

            case 4:
                hdfc.updatePrice(newPrice);
                investor.updatePortfolioPrice("HDFC", newPrice);
                break;

            case 5:
                wipro.updatePrice(newPrice);
                investor.updatePortfolioPrice("WIPRO", newPrice);
                break;

            default:
                cout << "Invalid stock selection.\n";
                continue;
            }

            cout << "Stock price updated successfully.\n";

            break;
        }

        case 7:

            investor.displayAccountSummary();

            break;

        case 8:
        {
            int verificationChoice;

            cout << "\nVerification Options:\n";
            cout << "1. Verify Email\n";
            cout << "2. Verify Email + Phone\n";

            cout << "Enter choice: ";

            if (!(cin >> verificationChoice))
            {

                cout << "Invalid input.\n";

                cin.clear();
                cin.ignore(10000, '\n');

                break;
            }

            if (verificationChoice == 1)
            {

                string inputEmail;

                cout << "Enter email: ";
                cin >> inputEmail;

                // Calls the one-parameter version.
                investor.verifyDetails(inputEmail);
            }

            else if (verificationChoice == 2)
            {

                string inputEmail;
                string inputPhone;

                cout << "Enter email: ";
                cin >> inputEmail;

                cout << "Enter phone: ";
                cin >> inputPhone;

                // Calls the two-parameter version.
                investor.verifyDetails(
                    inputEmail,
                    inputPhone);
            }

            else
            {

                cout << "Invalid verification choice.\n";
            }

            break;
        }

        case 9:

            cout << "\nThank you for using "
                 << "Virtual Stock Market Simulator!\n";

            return 0;

        default:

            cout << "\nInvalid choice!\n";
            cout << "Please select a number from 1 to 9.\n";
        }
    }

    return 0;
}
=======
                cout << "Enter new simulated price: ";

                if (!(cin >> newPrice)) {

                    cout << "Invalid price.\n";

                    cin.clear();
                    cin.ignore(10000, '\n');

                    break;
                }


                if (newPrice <= 0) {

                    cout << "Price must be greater than 0.\n";
                    break;
                }


                // Price change is included to simulate
                // changes in the stock market.
                switch (stockChoice) {

                    case 1:
                        tcs.updatePrice(newPrice);
                        investor.updatePortfolioPrice("TCS", newPrice);
                        break;

                    case 2:
                        infy.updatePrice(newPrice);
                        investor.updatePortfolioPrice("INFY", newPrice);
                        break;

                    case 3:
                        reliance.updatePrice(newPrice);
                        investor.updatePortfolioPrice("RELIANCE", newPrice);
                        break;

                    case 4:
                        hdfc.updatePrice(newPrice);
                        investor.updatePortfolioPrice("HDFC", newPrice);
                        break;

                    case 5:
                        wipro.updatePrice(newPrice);
                        investor.updatePortfolioPrice("WIPRO", newPrice);
                        break;

                    default:
                        cout << "Invalid stock selection.\n";
                        continue;
                }


                cout << "Stock price updated successfully.\n";

                break;
            }


            case 7:

                investor.displayAccountSummary();

                break;


            case 8:
            {
                int verificationChoice;

                cout << "\nVerification Options:\n";
                cout << "1. Verify Email\n";
                cout << "2. Verify Email + Phone\n";

                cout << "Enter choice: ";

                if (!(cin >> verificationChoice)) {

                    cout << "Invalid input.\n";

                    cin.clear();
                    cin.ignore(10000, '\n');

                    break;
                }


                if (verificationChoice == 1) {

                    string inputEmail;

                    cout << "Enter email: ";
                    cin >> inputEmail;

                    // Calls the one-parameter version.
                    investor.verifyDetails(inputEmail);
                }

                else if (verificationChoice == 2) {

                    string inputEmail;
                    string inputPhone;

                    cout << "Enter email: ";
                    cin >> inputEmail;

                    cout << "Enter phone: ";
                    cin >> inputPhone;

                    // Calls the two-parameter version.
                    investor.verifyDetails(
                        inputEmail,
                        inputPhone
                    );
                }

                else {

                    cout << "Invalid verification choice.\n";
                }

                break;
            }


            case 9:

                cout << "\nThank you for using "
                     << "Virtual Stock Market Simulator!\n";

                return 0;


            default:

                cout << "\nInvalid choice!\n";
                cout << "Please select a number from 1 to 9.\n";
        }
    }


    return 0;
}
>>>>>>> 9bc3c05433584cdc0806acc2b080c6fae69954de
