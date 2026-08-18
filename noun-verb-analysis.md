# Noun-Verb Analysis — Stock Market Simulator

## 1. Purpose

Noun-verb analysis is used to identify the important objects, classes, attributes, actions, and concepts in the Stock Market Simulator.

The analysis is performed using the project requirements, problem statement, use cases, and existing system design.

---

# 2. Raw Candidate Noun List

The initial noun list identified from the specifications is:

1. Trader
2. Stock
3. Order
4. Buy Order
5. Sell Order
6. Quantity
7. Price
8. Cash
9. Portfolio
10. Holding
11. Transaction
12. Market
13. Market Price
14. Simulation
15. Simulation Clock
16. Simulation Tick
17. Market Event
18. Event Probability
19. Market Parameter
20. Price Alert
21. Notification
22. Notification Service
23. Account
24. Login
25. Balance
26. Transaction Value
27. Market Statistics

---

# 3. Verb Analysis

The important actions identified from the specifications include:

- Register trader
- Store trader information
- Login
- Deposit money
- Check cash balance
- Deduct cash
- Add stock
- Find stock
- Update stock price
- Place buy order
- Place sell order
- Validate order
- Execute order
- Cancel order
- Add holding
- Remove holding
- Increase holding quantity
- Decrease holding quantity
- Calculate portfolio value
- Calculate holding value
- Record transaction
- Calculate transaction value
- Start simulation
- Stop simulation
- Advance simulation tick
- Generate market event
- Process market event
- Apply market event
- Set price alert
- Check price alert
- Trigger notification
- Mark notification as read

---

# 4. Four Filters

The candidate nouns are evaluated using four filters:

1. Duplicate/Synonym Filter
2. Attribute Filter
3. Out-of-Scope Filter
4. Implementation/UI Filter

---

# 5. Filter 1 — Duplicate/Synonym Filter

Some nouns represent the same concept and should not become separate classes.

| Candidate | Decision | Reason |
|---|---|---|
| Buy Order | Merge | Represents an Order with type BUY |
| Sell Order | Merge | Represents an Order with type SELL |
| Cash | Merge | Represented by the Account balance |
| Balance | Merge | Represented by Account cashBalance |
| Market Price | Merge | Represented by Stock currentPrice |
| Notification Service | Merge | Notification represents the required notification concept |
| Simulation Clock | Merge | Represents the time/control mechanism of Simulation |
| Simulation Tick | Merge | Represents an action/state within Simulation |
| Transaction Value | Merge | Calculated by Transaction |
| Event Probability | Merge | Can be represented as part of market-event behavior |
| Market Parameter | Merge | Considered part of market/simulation data |

After removing duplicates and synonyms, the important class candidates are:

- Trader
- Stock
- Order
- Portfolio
- Holding
- Transaction
- Market
- Simulation
- MarketEvent
- PriceAlert
- Notification
- Account

---

# 6. Filter 2 — Attribute Filter

Some nouns are properties of other classes rather than independent classes.

| Candidate | Belongs To | Decision |
|---|---|---|
| Quantity | Order / Holding / Transaction | Attribute |
| Price | Stock / Order | Attribute |
| Cash | Account | Attribute |
| Market Price | Stock | Attribute |
| Balance | Account | Attribute |
| Transaction Value | Transaction | Calculated attribute |
| Event Probability | MarketEvent / Simulation | Attribute or simulation parameter |
| Simulation Clock | Simulation | Attribute/control information |
| Simulation Tick | Simulation | Simulation operation/state |
| Market Parameter | Market / Simulation | Attribute/parameter |
| Market Statistics | Market | Derived information |

These concepts therefore do not need to become separate classes.

---

# 7. Filter 3 — Out-of-Scope Filter

Some candidate nouns are not required as independent domain classes for the Stock Market Simulator.

| Candidate | Decision | Reason |
|---|---|---|
| Login | Remove as independent class | Authentication/interface functionality rather than a core trading-domain object |
| Market Statistics | Remove as independent class | Can be calculated from Market and Stock information |
| Event Probability | Remove as independent class | Can be represented as part of market-event behavior |
| Market Parameter | Remove as independent class | Can be represented as data used by Market or Simulation |

---

# 8. Filter 4 — Implementation/UI Filter

Some concepts are implementation or interface details rather than core domain classes.

| Candidate | Decision | Reason |
|---|---|---|
| Notification Service | Remove as independent class | Service implementation detail |
| Login | Remove as independent class | User-interface/authentication functionality |
| Simulation Clock | Remove as independent class | Implementation/control detail of Simulation |
| Simulation Tick | Remove as independent class | Operation/state within Simulation |

---

# 9. Final Candidate Classes

After applying all four filters, the final candidate classes are:

1. Trader
2. Account
3. Stock
4. Market
5. Order
6. Portfolio
7. Holding
8. Transaction
9. Simulation
10. MarketEvent
11. PriceAlert
12. Notification

These classes form the main conceptual model of the Stock Market Simulator.

---

# 10. Final Class Responsibilities

| Class | Main Responsibility |
|---|---|
| Trader | Represents the user/trader and initiates trading activities |
| Account | Manages the trader's available cash |
| Stock | Represents a tradable stock and maintains its price information |
| Market | Maintains available stocks and market state |
| Order | Represents and manages buy/sell requests |
| Portfolio | Maintains the trader's collection of holdings |
| Holding | Represents ownership of a quantity of a particular stock |
| Transaction | Records completed trades |
| Simulation | Controls the simulated passage of time and market activity |
| MarketEvent | Represents events that affect stock prices |
| PriceAlert | Monitors a stock for a target price |
| Notification | Provides information to the trader about relevant events |

---

# 11. Final Result

The noun-verb analysis results in the following core classes:

```text
Trader
Account
Stock
Market
Order
Portfolio
Holding
Transaction
Simulation
MarketEvent
PriceAlert
Notification