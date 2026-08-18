# CRC Cards — Stock Market Simulator

## 1. Trader

### Class
Trader

### Responsibilities
- Store trader information.
- Place buy orders.
- Place sell orders.
- View the portfolio.
- Set price alerts.
- Receive notifications.

### Collaborators
- Account
- Portfolio
- Stock
- Order
- PriceAlert
- Notification


---

## 2. Account

### Class
Account

### Responsibilities
- Maintain the trader's cash balance.
- Deposit money into the account.
- Check whether sufficient cash is available.
- Deduct cash when an order is executed.
- Maintain financial information related to transactions.

### Collaborators
- Trader
- Order
- Transaction


---

## 3. Stock

### Class
Stock

### Responsibilities
- Store stock information.
- Store the stock symbol.
- Store the company name.
- Maintain the current stock price.
- Maintain the previous stock price.
- Update the stock price.
- Provide the current stock price.

### Collaborators
- Market
- Holding
- Order
- MarketEvent
- PriceAlert


---

## 4. Market

### Class
Market

### Responsibilities
- Maintain the stocks available in the market.
- Add stocks to the market.
- Find a stock using its symbol.
- Update stock prices.
- Process market events.
- Maintain the market open/closed state.

### Collaborators
- Stock
- MarketEvent
- Simulation


---

## 5. Order

### Class
Order

### Responsibilities
- Store order information.
- Store whether the order is a BUY or SELL order.
- Store the quantity of stock.
- Store the order price.
- Validate the order.
- Execute the order.
- Cancel the order.
- Maintain the order status.
- Create a transaction after successful execution.

### Collaborators
- Trader
- Stock
- Account
- Portfolio
- Transaction


---

## 6. Portfolio

### Class
Portfolio

### Responsibilities
- Maintain the trader's holdings.
- Add holdings to the portfolio.
- Remove holdings from the portfolio.
- Calculate the total portfolio value.
- Update holdings after transactions.

### Collaborators
- Trader
- Holding
- Stock
- Transaction


---

## 7. Holding

### Class
Holding

### Responsibilities
- Store the quantity of a stock owned by the trader.
- Store the average purchase price.
- Increase the quantity of a holding.
- Decrease the quantity of a holding.
- Calculate the current value of the holding.

### Collaborators
- Portfolio
- Stock


---

## 8. Transaction

### Class
Transaction

### Responsibilities
- Record completed trades.
- Store the transaction type.
- Store the quantity traded.
- Store the execution price.
- Store the transaction timestamp.
- Calculate the total transaction value.

### Collaborators
- Order
- Account
- Portfolio


---

## 9. Simulation

### Class
Simulation

### Responsibilities
- Start the stock market simulation.
- Stop the stock market simulation.
- Control the simulation time.
- Advance the simulation by one tick.
- Generate market events.
- Connect the simulation with the market.

### Collaborators
- Market
- MarketEvent


---

## 10. MarketEvent

### Class
MarketEvent

### Responsibilities
- Represent an event that affects the market.
- Store the type of market event.
- Store the impact factor of the event.
- Apply the event to stocks.
- Affect stock prices according to the event.

### Collaborators
- Stock
- Market
- Simulation


---

## 11. PriceAlert

### Class
PriceAlert

### Responsibilities
- Store the target price for a stock.
- Monitor the stock price.
- Check whether the target price has been reached.
- Mark the alert as triggered.
- Generate a notification when the target price is reached.

### Collaborators
- Trader
- Stock
- Notification


---

## 12. Notification

### Class
Notification

### Responsibilities
- Store the notification message.
- Store the notification creation time.
- Track whether the notification has been read.
- Mark the notification as read.
- Inform the trader about relevant events.

### Collaborators
- Trader
- PriceAlert