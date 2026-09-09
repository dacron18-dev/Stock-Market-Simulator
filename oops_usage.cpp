#include <iostream>
#include <string>
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
        cout << "Name: " << name << endl;
        cout << "Email: " << email << endl;
        cout << "Phone: " << phone << endl;
        cout << "User ID: " << userID << endl;
    }
};

class Portfolio {
private:
    double value;

public:
    Portfolio(double v) {
        value = v;
    }

    Portfolio operator+(Portfolio p) {
        return Portfolio(value + p.value);
    }

    void display() {
        cout << "Portfolio Value: " << value << endl;
    }
};

class Investor : public User {
private:
    double virtualMoney;

public:
    Investor(string n, string e, string p, int id, double money)
        : User(n, e, p, id) {
        virtualMoney = money;
    }

    void verifyDetails(string inputEmail) {
        if (inputEmail == email)
            cout << "Email verified" << endl;
        else
            cout << "Email verification failed" << endl;
    }

    void verifyDetails(string inputEmail, string inputPhone) {
        if (inputEmail == email && inputPhone == phone)
            cout << "Email and phone verified" << endl;
        else
            cout << "Verification failed" << endl;
    }

    void buyStock(string stockName, double price, int quantity) {
        double cost = price * quantity;

        if (cost <= virtualMoney) {
            virtualMoney -= cost;
            cout << quantity << " shares of " << stockName << " purchased" << endl;
        } else {
            cout << "Insufficient virtual money" << endl;
        }
    }

    void buyStock(string stockName, int quantity) {
        cout << "Buying " << quantity << " shares of " << stockName << endl;
    }
};

int main() {
    Investor investor("Saanvi", "saanvi@gmail.com", "9876543210", 101, 100000);

    investor.displayUserDetails();

    investor.verifyDetails("saanvi@gmail.com");
    investor.verifyDetails("saanvi@gmail.com", "9876543210");

    investor.buyStock("TCS", 3500, 10);
    investor.buyStock("INFY", 5);

    Portfolio p1(50000);
    Portfolio p2(30000);

    Portfolio p3 = p1 + p2;

    p3.display();

    return 0;
}