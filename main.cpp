// Stock Market Simulator - C++17 backend (engine + REST API, no external libraries)
// Build: g++ -std=c++17 -pthread main.cpp -o simulator      Run: ./simulator
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>
#include <random>
#include <chrono>
#include <ctime>
#include <stdexcept>
#include <csignal>
#include <algorithm>
#include <cstdlib>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

namespace util {                                   // Namespace
mt19937 rng(random_device{}());
double uni() { return uniform_real_distribution<double>(0, 1)(rng); }
double randn() { return normal_distribution<double>(0, 1)(rng); }
string nowStr() { time_t t = ::time(nullptr); char b[16]; strftime(b, sizeof b, "%H:%M:%S", localtime(&t)); return b; }
}


// ---------- MarketDataProvider: IndianAPI integration ----------

class MarketDataProvider {
    string apiKey;

    static size_t writeCallback(void* contents, size_t size,
                                size_t nmemb, string* output) {
        size_t total = size * nmemb;
        output->append(static_cast<char*>(contents), total);
        return total;
    }

public:
    MarketDataProvider() {
        const char* key = getenv("INDIAN_API_KEY");
        if (key) apiKey = key;
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    ~MarketDataProvider() {
        curl_global_cleanup();
    }

    bool fetchNSEPrice(const string& companyName, double& price) {
        if (apiKey.empty()) return false;

        CURL* curl = curl_easy_init();
        if (!curl) return false;

        string response;
        char* escapedName = curl_easy_escape(
            curl, companyName.c_str(), 0
        );

        if (!escapedName) {
            curl_easy_cleanup(curl);
            return false;
        }

        string url = "https://stock.indianapi.in/stock?name=";
        url += escapedName;
        curl_free(escapedName);

        struct curl_slist* headers = nullptr;
        string apiHeader = "X-Api-Key: " + apiKey;
        headers = curl_slist_append(headers, apiHeader.c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);

        CURLcode result = curl_easy_perform(curl);

        long statusCode = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);

        bool success = false;

        if (result == CURLE_OK && statusCode == 200) {
            try {
                auto data = nlohmann::json::parse(response);
                auto value = data.at("currentPrice").at("NSE");

                double fetchedPrice;

                if (value.is_string())
                    fetchedPrice = stod(value.get<string>());
                else
                    fetchedPrice = value.get<double>();

                if (fetchedPrice > 0) {
                    price = fetchedPrice;
                    success = true;
                }
            }
            catch (const exception&) {
                success = false;
            }
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        return success;
    }
};

// ---------- Templates: fixed-size price history for any type ----------
template <typename T, size_t N = 60>
class History {
    deque<T> d;                                    // STL deque
public:
    void push(const T& v) { d.push_back(v); if (d.size() > N) d.pop_front(); }
    const deque<T>& data() const { return d; }
};

// ---------- Exception handling: user-defined exceptions ----------
class InsufficientFunds : public runtime_error { public: using runtime_error::runtime_error; };
class InvalidOrder : public runtime_error { public: using runtime_error::runtime_error; };

enum class OrderType { BUY, SELL };                // Enumeration
string toStr(OrderType t) { return t == OrderType::BUY ? "BUY" : "SELL"; }

// ---------- Stock: encapsulation (private data, const getters) ----------
class Stock {
    string symbol, name;
    double price, prev;
    History<double> hist;
public:
    Stock(string s, string n, double p) : symbol(move(s)), name(move(n)), price(p), prev(p) { hist.push(p); }  // init list
    void updatePrice(double p) { prev = price; price = max(1.0, p); hist.push(price); }
    const string& getSymbol() const { return symbol; }
    const string& getName() const { return name; }
    double getPrice() const { return price; }
    double getPrev() const { return prev; }
    const History<double>& getHistory() const { return hist; }
};

// ---------- Abstraction + Inheritance + Polymorphism: market events ----------
class MarketEvent {                                // abstract class
protected:
    string name;
public:
    explicit MarketEvent(string n) : name(move(n)) {}
    virtual ~MarketEvent() = default;              // virtual destructor
    virtual double factor() const = 0;             // pure virtual function
    const string& getName() const { return name; }
};
class BullRun : public MarketEvent { public: BullRun() : MarketEvent("Bull run: buyers push prices up") {}
    double factor() const override { return 1.01 + 0.02 * util::uni(); } };
class Crash : public MarketEvent { public: Crash() : MarketEvent("Sell-off: prices drop sharply") {}
    double factor() const override { return 0.94 + 0.03 * util::uni(); } };
class EarningsSeason : public MarketEvent { public: EarningsSeason() : MarketEvent("Earnings season: high volatility") {}
    double factor() const override { return 0.97 + 0.06 * util::uni(); } };

// ---------- Market: aggregation of Stocks (smart pointers, STL vector) ----------
class Market {
    vector<unique_ptr<Stock>> stocks;
    string lastEvent = "Market is calm";
public:
    Market() {
        add("RELIANCE", "Reliance Industries", 2500); add("TCS", "Tata Consultancy Services", 3900);
        add("INFY", "Infosys", 1500); add("HDFCBANK", "HDFC Bank", 1650);
        add("ITC", "ITC Limited", 430); add("TATAMOTORS", "Tata Motors", 950);
    }
    void add(const string& s, const string& n, double p) { stocks.push_back(make_unique<Stock>(s, n, p)); }
    Stock* get(const string& s) { for (auto& x : stocks) if (x->getSymbol() == s) return x.get(); return nullptr; }
    const vector<unique_ptr<Stock>>& all() const { return stocks; }
    const string& event() const { return lastEvent; }

    int refreshFromAPI(MarketDataProvider& provider) {
        int updated = 0;

        for (auto& stock : stocks) {
            double fetchedPrice = 0;

            if (provider.fetchNSEPrice(stock->getName(), fetchedPrice)) {
                stock->updatePrice(fetchedPrice);
                updated++;
            }
        }

        if (updated > 0)
            lastEvent = "Prices updated from IndianAPI";

        return updated;
    }
    void tick() {                                  // simulation clock step
        unique_ptr<MarketEvent> ev;                // base-class pointer -> dynamic binding
        double r = util::uni();
        if (r < 0.05) ev = make_unique<BullRun>();
        else if (r < 0.09) ev = make_unique<Crash>();
        else if (r < 0.14) ev = make_unique<EarningsSeason>();
        if (ev) lastEvent = ev->getName();
        for (auto& s : stocks) {
            double p = s->getPrice() * (1 + 0.006 * util::randn());   // random walk
            if (ev) p *= ev->factor();
            s->updatePrice(p);
        }
    }
};

// ---------- Account / Holding / Portfolio (composition) ----------
class Account {
    double cash;
public:
    explicit Account(double c) : cash(c) {}
    void deposit(double a) { cash += a; }
    void deduct(double a) { if (a > cash) throw InsufficientFunds("Not enough virtual cash"); cash -= a; }
    double balance() const { return cash; }
};
class Holding {
    int qty = 0; double avg = 0;
public:
    void increase(int q, double p) { avg = (avg * qty + p * q) / (qty + q); qty += q; }
    void decrease(int q) { qty -= q; }
    int getQty() const { return qty; }
    double getAvg() const { return avg; }
    double value(double cur) const { return qty * cur; }
};
class Portfolio {
    map<string, Holding> h;                        // STL map
public:
    void add(const string& s, int q, double p) { h[s].increase(q, p); }
    void remove(const string& s, int q) {
        auto it = h.find(s);
        if (it == h.end() || it->second.getQty() < q) throw InvalidOrder("You do not own enough shares");
        it->second.decrease(q);
        if (it->second.getQty() == 0) h.erase(it);
    }
    const map<string, Holding>& all() const { return h; }
};

// ---------- Transaction: static member, friend function, operator<< ----------
class Transaction {
    int id; OrderType type; string symbol; int qty; double price; string stamp;
    static int counter;                            // static data member
public:
    Transaction(OrderType t, string s, int q, double p) : id(++counter), type(t), symbol(move(s)), qty(q), price(p), stamp(util::nowStr()) {}
    double value() const { return qty * price; }
    friend ostream& operator<<(ostream& os, const Transaction& t);   // friend + operator overloading
    string json() const {
        ostringstream o; o << fixed << setprecision(2);
        o << "{\"id\":" << id << ",\"type\":\"" << toStr(type) << "\",\"symbol\":\"" << symbol << "\",\"qty\":" << qty
          << ",\"price\":" << price << ",\"time\":\"" << stamp << "\"}";
        return o.str();
    }
};
int Transaction::counter = 12000;
ostream& operator<<(ostream& os, const Transaction& t) {          // used for CSV file handling
    return os << t.id << ',' << toStr(t.type) << ',' << t.symbol << ',' << t.qty << ',' << fixed << setprecision(2) << t.price << ',' << t.stamp;
}

// ---------- Orders: abstract base, BuyOrder/SellOrder override execute() ----------
class Order {
protected:
    int id; string symbol; int qty; double price;
    static int counter;
public:
    Order(string s, int q, double p) : id(++counter), symbol(move(s)), qty(q), price(p) {}
    virtual ~Order() = default;
    virtual Transaction execute(Account& a, Portfolio& p) = 0;
    int getId() const { return id; }
};
int Order::counter = 9000;
class BuyOrder : public Order {
public:
    using Order::Order;                            // inherit constructor
    Transaction execute(Account& a, Portfolio& p) override {
        a.deduct(qty * price); p.add(symbol, qty, price);
        return Transaction(OrderType::BUY, symbol, qty, price);
    }
};
class SellOrder : public Order {
public:
    using Order::Order;
    Transaction execute(Account& a, Portfolio& p) override {
        p.remove(symbol, qty); a.deposit(qty * price);
        return Transaction(OrderType::SELL, symbol, qty, price);
    }
};

class PriceAlert {
    string symbol; double target; bool above; bool triggered = false;
public:
    PriceAlert(string s, double t, double cur) : symbol(move(s)), target(t), above(t >= cur) {}
    bool check(double p) { if (!triggered && (above ? p >= target : p <= target)) { triggered = true; return true; } return false; }
    const string& getSymbol() const { return symbol; }
    double getTarget() const { return target; }
    bool isTriggered() const { return triggered; }
};

// ---------- Trader: one per registered user (matches the class diagram) ----------
class Trader {
    int traderId; string name; size_t passHash; bool admin;
    Account account; Portfolio pf; vector<Transaction> txns; vector<PriceAlert> alerts; deque<string> notes;
    static int counter;
public:
    static constexpr double START_CASH = 100000;
    Trader(string n, size_t h, bool a) : traderId(++counter), name(move(n)), passHash(h), admin(a), account(START_CASH) {}
    const string& getName() const { return name; }
    bool isAdmin() const { return admin; }
    bool checkPass(size_t h) const { return h == passHash; }
    Account& acct() { return account; }
    Portfolio& port() { return pf; }
    vector<Transaction>& history() { return txns; }
    vector<PriceAlert>& priceAlerts() { return alerts; }
    deque<string>& inbox() { return notes; }
    void reset() { account = Account(START_CASH); pf = Portfolio(); txns.clear(); alerts.clear(); notes.clear(); }
};
int Trader::counter = 100;

// ---------- Simulator: shared Market + many Traders, clock on a thread ----------
class Simulator {
    mutex mu; Market market; map<string, Trader> users; map<string, string> sessions;   // token -> username
    atomic<bool> running{true}, alive{true};
    ofstream csv; thread worker;
    static string reply(bool ok, const string& msg, const string& extra = "") {
        return string("{\"ok\":") + (ok ? "true" : "false") + ",\"msg\":\"" + msg + "\"" + (extra.empty() ? "" : "," + extra) + "}";
    }
    // NOTE: std::hash is fine for a class project; use bcrypt/argon2 in production.
    static size_t hashPw(const string& p) { return hash<string>{}("papertrade-salt:" + p); }
    static string adminPass() { const char* e = getenv("ADMIN_PASS"); return e ? e : "admin123"; }
    static bool validName(const string& n) {
        if (n.size() < 3 || n.size() > 16) return false;
        for (char c : n) if (!isalnum((unsigned char)c)) return false;
        return true;
    }
    static string newToken() { random_device rd; ostringstream o; o << hex; for (int i = 0; i < 4; ++i) o << setw(8) << setfill('0') << rd(); return o.str(); }
    string startSession(const string& name) {
        string tok = newToken(); sessions[tok] = name;
        return reply(true, "Welcome, " + name, "\"token\":\"" + tok + "\"");
    }
    Trader* auth(const string& tok) { auto it = sessions.find(tok); return it == sessions.end() ? nullptr : &users.at(it->second); }
    void checkAlerts() {
        for (auto& [n, u] : users)
            for (auto& a : u.priceAlerts()) {
                Stock* s = market.get(a.getSymbol());
                if (a.check(s->getPrice())) {
                    ostringstream o; o << fixed << setprecision(2) << "Alert: " << a.getSymbol() << " reached " << a.getTarget();
                    u.inbox().push_front(o.str()); if (u.inbox().size() > 6) u.inbox().pop_back();
                }
            }
    }
    double holdingsValue(Trader& u) {
        double hv = 0;
        for (const auto& [sym, h] : u.port().all()) hv += h.value(market.get(sym)->getPrice());
        return hv;
    }
public:
    Simulator() : csv("transactions.csv", ios::app) {
        worker = thread([this] {
            MarketDataProvider provider;

            const vector<pair<string, string>> companies = {
                {"RELIANCE", "Reliance Industries"},
                {"TCS", "Tata Consultancy Services"},
                {"INFY", "Infosys"},
                {"HDFCBANK", "HDFC Bank"},
                {"ITC", "ITC Limited"},
                {"TATAMOTORS", "Tata Motors"}
            };

            bool apiMode = false;
            auto nextRefresh = chrono::steady_clock::now();

            while (alive) {
                this_thread::sleep_for(chrono::seconds(2));
                if (!running) continue;

                if (chrono::steady_clock::now() >= nextRefresh) {
                    nextRefresh = chrono::steady_clock::now()
                                + chrono::seconds(60);

                    vector<pair<string, double>> fetchedPrices;

                    for (const auto& company : companies) {
                        double price = 0;

                        if (provider.fetchNSEPrice(company.second, price)) {
                            fetchedPrices.push_back({company.first, price});
                        }
                    }

                    lock_guard<mutex> g(mu);

                    for (const auto& item : fetchedPrices) {
                        Stock* stock = market.get(item.first);
                        if (stock) stock->updatePrice(item.second);
                    }

                    apiMode = !fetchedPrices.empty();

                    if (!apiMode) market.tick();

                    checkAlerts();
                }
                else {
                    lock_guard<mutex> g(mu);

                    if (!apiMode) market.tick();

                    checkAlerts();
                }
            }
        });
    }
    ~Simulator() { alive = false; if (worker.joinable()) worker.join(); }

    string signup(const string& name, const string& pass) {
        lock_guard<mutex> g(mu);
        if (!validName(name) || pass.size() < 4) return reply(false, "Use a 3-16 letter or number name and a password of 4+ characters");
        bool admin = (name == "admin");
        if (admin && pass != adminPass()) return reply(false, "Wrong admin password");
        if (users.count(name)) return reply(false, "That name is taken. Log in instead");
        users.emplace(name, Trader(name, hashPw(pass), admin));
        return startSession(name);
    }
    string login(const string& name, const string& pass) {
        lock_guard<mutex> g(mu);
        auto it = users.find(name);
        if (it == users.end() || !it->second.checkPass(hashPw(pass))) return reply(false, "Wrong name or password");
        return startSession(name);
    }
    string placeOrder(const string& tok, const string& side, const string& sym, int qty) {
        lock_guard<mutex> g(mu);
        Trader* u = auth(tok); if (!u) return reply(false, "Please log in");
        try {                                                         // exception handling
            Stock* s = market.get(sym);
            if (!s || qty <= 0) throw InvalidOrder("Choose a stock and a quantity above zero");
            unique_ptr<Order> o;
            if (side == "BUY") o = make_unique<BuyOrder>(sym, qty, s->getPrice());
            else o = make_unique<SellOrder>(sym, qty, s->getPrice());
            Transaction t = o->execute(u->acct(), u->port());         // dynamic binding
            csv << u->getName() << ',' << t << endl; u->history().push_back(t);
            return reply(true, side + " done: " + to_string(qty) + " x " + sym);
        } catch (const exception& e) { return reply(false, e.what()); }
    }
    string setAlert(const string& tok, const string& sym, double price) {
        lock_guard<mutex> g(mu);
        Trader* u = auth(tok); if (!u) return reply(false, "Please log in");
        Stock* s = market.get(sym);
        if (!s || price <= 0) return reply(false, "Enter a valid alert price");
        u->priceAlerts().emplace_back(sym, price, s->getPrice());
        return reply(true, "Alert set for " + sym);
    }
    string admin(const string& tok, const string& cmd) {
        lock_guard<mutex> g(mu);
        Trader* u = auth(tok); if (!u || !u->isAdmin()) return reply(false, "Only the admin can do this");
        if (cmd == "start") running = true;
        else if (cmd == "stop") running = false;
        else if (cmd == "reset") { market = Market(); for (auto& [n, t] : users) t.reset(); }
        else return reply(false, "Unknown command");
        return reply(true, cmd == "reset" ? "Simulation reset" : cmd == "start" ? "Simulation started" : "Simulation paused");
    }
    string state(const string& tok) {
        lock_guard<mutex> g(mu);
        Trader* u = auth(tok); if (!u) return reply(false, "login");
        ostringstream o; o << fixed << setprecision(2);
        o << "{\"ok\":true,\"me\":\"" << u->getName() << "\",\"admin\":" << (u->isAdmin() ? "true" : "false")
          << ",\"running\":" << (running ? "true" : "false") << ",\"event\":\"" << market.event() << "\",\"stocks\":[";
        bool first = true;
        for (auto& s : market.all()) {
            if (!first) o << ","; first = false;
            o << "{\"symbol\":\"" << s->getSymbol() << "\",\"name\":\"" << s->getName() << "\",\"price\":" << s->getPrice()
              << ",\"prev\":" << s->getPrev() << ",\"history\":[";
            const auto& h = s->getHistory().data();
            for (size_t i = 0; i < h.size(); ++i) { if (i) o << ","; o << h[i]; }
            o << "]}";
        }
        double hv = holdingsValue(*u), total = u->acct().balance() + hv;
        o << "],\"cash\":" << u->acct().balance() << ",\"holdingsValue\":" << hv << ",\"total\":" << total
          << ",\"pnl\":" << total - Trader::START_CASH << ",\"holdings\":[";
        first = true;
        for (const auto& [sym, h] : u->port().all()) {
            double cur = market.get(sym)->getPrice();
            if (!first) o << ","; first = false;
            o << "{\"symbol\":\"" << sym << "\",\"qty\":" << h.getQty() << ",\"avg\":" << h.getAvg() << ",\"price\":" << cur << ",\"value\":" << h.value(cur) << "}";
        }
        o << "],\"transactions\":[";
        auto& tx = u->history(); first = true;
        for (size_t i = tx.size(); i-- > 0 && tx.size() - i <= 12;) { if (!first) o << ","; first = false; o << tx[i].json(); }
        o << "],\"alerts\":[";
        first = true;
        for (auto& a : u->priceAlerts()) { if (!first) o << ","; first = false;
            o << "{\"symbol\":\"" << a.getSymbol() << "\",\"target\":" << a.getTarget() << ",\"triggered\":" << (a.isTriggered() ? "true" : "false") << "}"; }
        o << "],\"notifications\":[";
        first = true;
        for (auto& n : u->inbox()) { if (!first) o << ","; first = false; o << "\"" << n << "\""; }
        vector<pair<double, string>> board;                           // leaderboard
        for (auto& [n, t] : users) if (!t.isAdmin()) board.push_back({t.acct().balance() + holdingsValue(t), n});
        sort(board.rbegin(), board.rend());
        o << "],\"leaderboard\":[";
        first = true;
        for (size_t i = 0; i < board.size() && i < 5; ++i) { if (!first) o << ","; first = false;
            o << "{\"name\":\"" << board[i].second << "\",\"total\":" << board[i].first << "}"; }
        o << "]}";
        return o.str();
    }
};

// ---------- Minimal HTTP server (POSIX sockets) ----------
string urlDecode(const string& s) {
    string r;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '%' && i + 2 < s.size()) { r += (char)stoi(s.substr(i + 1, 2), nullptr, 16); i += 2; }
        else r += (s[i] == '+' ? ' ' : s[i]);
    }
    return r;
}
map<string, string> parseQuery(const string& q) {
    map<string, string> m; stringstream ss(q); string kv;
    while (getline(ss, kv, '&')) { auto p = kv.find('='); if (p != string::npos) m[kv.substr(0, p)] = urlDecode(kv.substr(p + 1)); }
    return m;
}
string readFile(const string& p) { ifstream f(p, ios::binary); stringstream b; b << f.rdbuf(); return b.str(); }
void respond(int fd, int code, const string& type, const string& body) {
    string r = "HTTP/1.1 " + to_string(code) + " OK\r\nContent-Type: " + type + "\r\nContent-Length: " + to_string(body.size()) +
               "\r\nAccess-Control-Allow-Origin: *\r\nConnection: close\r\n\r\n" + body;
    ::send(fd, r.data(), r.size(), 0);
}
void handle(int fd, const string& req, Simulator& sim) {
    istringstream is(req); string method, target; is >> method >> target;
    string path = target, query;
    auto q = target.find('?');
    if (q != string::npos) { path = target.substr(0, q); query = target.substr(q + 1); }
    auto p = parseQuery(query);
    const string J = "application/json";
    try {
        if (path == "/" || path == "/index.html") respond(fd, 200, "text/html; charset=utf-8", readFile("index.html"));
        else if (path == "/api/register") respond(fd, 200, J, sim.signup(p["name"], p["pass"]));
        else if (path == "/api/login") respond(fd, 200, J, sim.login(p["name"], p["pass"]));
        else if (path == "/api/state") respond(fd, 200, J, sim.state(p["token"]));
        else if (path == "/api/order") respond(fd, 200, J, sim.placeOrder(p["token"], p["side"], p["symbol"], stoi(p["qty"])));
        else if (path == "/api/alert") respond(fd, 200, J, sim.setAlert(p["token"], p["symbol"], stod(p["price"])));
        else if (path == "/api/admin") respond(fd, 200, J, sim.admin(p["token"], p["cmd"]));
        else respond(fd, 404, J, "{\"ok\":false,\"msg\":\"Not found\"}");
    } catch (...) { respond(fd, 400, J, "{\"ok\":false,\"msg\":\"Enter valid numbers\"}"); }
}

int main() {
    signal(SIGPIPE, SIG_IGN);
    Simulator sim;
    const char* envPort = getenv("PORT");                      // cloud hosts set PORT
    const int port = envPort ? atoi(envPort) : 8080;
    int srv = ::socket(AF_INET, SOCK_STREAM, 0), yes = 1;
    setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
    sockaddr_in addr{}; addr.sin_family = AF_INET; addr.sin_addr.s_addr = INADDR_ANY; addr.sin_port = htons(port);
    if (::bind(srv, (sockaddr*)&addr, sizeof addr) < 0 || ::listen(srv, 16) < 0) { perror("server"); return 1; }
    cout << "Stock Market Simulator listening on port " << port << endl;
    while (true) {
        int fd = ::accept(srv, nullptr, nullptr);
        if (fd < 0) continue;
        char buf[4096]; ssize_t n = ::recv(fd, buf, sizeof buf - 1, 0);
        if (n > 0) { buf[n] = 0; handle(fd, string(buf), sim); }
        ::close(fd);
    }
}
