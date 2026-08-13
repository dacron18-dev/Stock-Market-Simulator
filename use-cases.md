# Stock Market Simulator System (Use Case Specifications)

## Use Case 1: Place Buy Order

**Primary Actor:** Trader

**Stakeholders:**
- Trader wants to purchase stocks using virtual money.
- Stock Market Simulator must execute valid orders and maintain accurate balances.
- Administrator requires a consistent simulation state.

**Preconditions:**
1. The trader is logged in.
2. The selected stock exists in the simulated market.
3. The market is open for trading.
4. The trader has an active virtual account.

**Postconditions:**
- The buy order is executed successfully.
- The trader's virtual cash balance is reduced.
- The purchased shares are added to the trader's portfolio.
- A transaction record is created.

**Trigger:**  
The trader submits a buy order for a stock.

### Main Flow
1. The trader selects a stock from the market.
2. The system displays the current simulated stock price.
3. The trader enters the quantity to purchase.
4. The trader submits the buy order.
5. The system validates the stock and requested quantity.
6. The system calculates the total order value.
7. The system checks whether the trader has sufficient virtual cash.
8. The system accepts and executes the order.
9. The system deducts the order value from the trader's virtual cash balance.
10. The system adds the purchased shares to the trader's portfolio.
11. The system creates a transaction record.
12. The system displays confirmation of the successful purchase.

### Alternate Flow 1 — Insufficient Funds
1. The system calculates the total order value.
2. The trader's available virtual cash is less than the required amount.
3. The system rejects the order.
4. The system displays an insufficient-funds message.
5. No cash or portfolio values are changed.

### Alternate Flow 2 — Invalid Quantity
1. The trader enters a quantity of zero or a negative number.
2. The system detects the invalid quantity.
3. The system rejects the order.
4. The system asks the trader to enter a valid positive quantity.

---

## Use Case 2: Place Sell Order

**Primary Actor:** Trader

**Stakeholders:**
- Trader wants to sell shares and receive virtual cash.
- Stock Market Simulator must maintain accurate holdings and balances.
- Administrator requires consistent transaction records.

**Preconditions:**
1. The trader is logged in.
2. The selected stock exists in the simulated market.
3. The market is open for trading.
4. The trader owns at least the number of shares being sold.

**Postconditions:**
- The sell order is executed successfully.
- The sold shares are removed from the trader's portfolio.
- The corresponding virtual cash is credited to the trader's account.
- A transaction record is created.

**Trigger:**  
The trader submits a sell order.

### Main Flow
1. The trader selects a stock from the portfolio.
2. The system displays the current price and available holdings.
3. The trader enters the quantity to sell.
4. The trader submits the sell order.
5. The system validates the stock and quantity.
6. The system checks whether the trader owns sufficient shares.
7. The system calculates the transaction value.
8. The system executes the sell order.
9. The system removes the sold shares from the portfolio.
10. The system credits the transaction value to the trader's virtual cash balance.
11. The system creates a transaction record.
12. The system displays confirmation of the successful sale.

### Alternate Flow 1 — Insufficient Holdings
1. The trader requests to sell more shares than they own.
2. The system detects insufficient holdings.
3. The system rejects the order.
4. The system displays the maximum number of shares currently available.

### Alternate Flow 2 — Market Closed
1. The trader submits the sell order.
2. The system checks the market status.
3. The market is closed.
4. The system rejects the order.
5. The system informs the trader that trading is currently unavailable.

---

## Use Case 3: Advance Simulation Tick

**Primary Actor:** Simulation Clock

**Stakeholders:**
- Trader expects stock prices and portfolio values to reflect the latest simulated market state.
- Administrator wants the simulation to progress according to configured parameters.
- Stock Market Simulator must maintain a consistent market state.

**Preconditions:**
1. The simulation has been started.
2. At least one stock is listed in the simulated market.
3. Simulation parameters have been configured.

**Postconditions:**
- Stock prices are updated.
- Any applicable market events are processed.
- Portfolio values can reflect the updated prices.
- Triggered price alerts are identified and notifications are generated.

**Trigger:**  
The simulation clock reaches the next scheduled simulation tick.

### Main Flow
1. The Simulation Clock triggers the next market tick.
2. The system retrieves the currently listed stocks.
3. The system calculates simulated price movements.
4. The system updates the current price of each stock.
5. The system updates relevant market statistics.
6. The system recalculates portfolio valuations using the updated prices.
7. The system checks active price alerts.
8. The system generates notifications for alerts whose conditions are satisfied.
9. The system stores the updated market state.
10. The simulation tick is completed.

### Alternate Flow 1 — Market Event Triggered
1. The system calculates the probability of a simulated market event.
2. The event probability reaches the configured threshold.
3. The system generates a market event.
4. The event modifies the price movement of one or more relevant stocks.
5. The system continues processing the simulation tick.

**Guard Condition:** `[market event probability >= configured event threshold]`

### Alternate Flow 2 — Price Alert Triggered
1. The system updates a stock price.
2. The system checks users who have configured alerts for that stock.
3. A stock price crosses a user's configured alert price.
4. The system creates a notification.
5. The notification service sends the alert to the trader.

**Guard Condition:** `[new stock price crosses configured alert price]`
